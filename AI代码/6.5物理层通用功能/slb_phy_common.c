/**
 * @file slb_phy_common.c
 * @brief 6.5.1 CRC / 6.5.2 Gold / 6.5.3 QPSK 参考信号实现
 */
#include "slb_phy_common.h"

#include <math.h>
#include <string.h>

typedef struct {
    uint32_t poly; /* 低 L 位（不含 D^L 项） */
    uint32_t L;
} slb_crc_desc_t;

static slb_crc_desc_t slb_crc_desc(slb_crc_poly_t poly)
{
    switch (poly) {
    case SLB_CRC24A:
        return (slb_crc_desc_t){0x864CFBu, 24u};
    case SLB_CRC24B:
        return (slb_crc_desc_t){0x800063u, 24u};
    case SLB_CRC16:
        return (slb_crc_desc_t){0x1021u, 16u};
    case SLB_CRC8:
        return (slb_crc_desc_t){0x9Bu, 8u};
    case SLB_CRC6:
        return (slb_crc_desc_t){0x21u, 6u};
    default:
        return (slb_crc_desc_t){0u, 0u};
    }
}

uint32_t slb_crc_get_len(slb_crc_poly_t poly)
{
    slb_crc_desc_t d = slb_crc_desc(poly);
    return d.L;
}

static void slb_crc_remainder(const uint8_t *bits, uint32_t n, uint32_t poly, uint32_t L,
                              uint8_t *parity)
{
    uint32_t reg = 0u;
    uint32_t mask = (L >= 32u) ? 0xFFFFFFFFu : ((1u << L) - 1u);

    for (uint32_t i = 0u; i < n + L; i++) {
        uint32_t bit = (i < n) ? (uint32_t)(bits[i] & 1u) : 0u;
        uint32_t top = (reg >> (L - 1u)) & 1u;
        reg = ((reg << 1u) | bit) & mask;
        if (top ^ bit) {
            reg ^= poly;
        }
    }

    for (uint32_t j = 0u; j < L; j++) {
        parity[j] = (uint8_t)((reg >> (L - 1u - j)) & 1u);
    }
}

int slb_crc_encode(const slb_crc_encode_t *cfg)
{
    slb_crc_desc_t d;
    uint8_t        parity[24];

    if (cfg == NULL || cfg->a == NULL || cfg->b == NULL) {
        return SLB_PHY_ERR_PARAM;
    }

    d = slb_crc_desc(cfg->poly);
    if (d.L == 0u) {
        return SLB_PHY_ERR_PARAM;
    }
    if (cfg->b_cap < cfg->A + d.L) {
        return SLB_PHY_ERR_BUF;
    }
    if (d.L == 24u && cfg->mask == NULL) {
        /* mask 可选 */
    }

    memcpy(cfg->b, cfg->a, cfg->A);
    slb_crc_remainder(cfg->a, cfg->A, d.poly, d.L, parity);

    for (uint32_t i = 0u; i < d.L; i++) {
        uint8_t p = parity[i];
        if (d.L == 24u && cfg->mask != NULL) {
            p = (uint8_t)((p + (cfg->mask[i] & 1u)) & 1u);
        }
        cfg->b[cfg->A + i] = p;
    }
    return SLB_PHY_OK;
}

int slb_crc_check(const slb_crc_check_t *cfg)
{
    slb_crc_desc_t d;
    uint32_t       reg = 0u;
    uint32_t       mask;

    if (cfg == NULL || cfg->b == NULL || cfg->pass == NULL) {
        return SLB_PHY_ERR_PARAM;
    }

    d = slb_crc_desc(cfg->poly);
    if (d.L == 0u || cfg->B == 0u) {
        return SLB_PHY_ERR_PARAM;
    }

    mask = (d.L >= 32u) ? 0xFFFFFFFFu : ((1u << d.L) - 1u);
    for (uint32_t i = 0u; i < cfg->B; i++) {
        uint32_t bit = (uint32_t)(cfg->b[i] & 1u);
        uint32_t top = (reg >> (d.L - 1u)) & 1u;
        reg          = ((reg << 1u) | bit) & mask;
        if (top ^ bit) {
            reg ^= d.poly;
        }
    }
    *cfg->pass = (reg == 0u) ? 1u : 0u;
    return SLB_PHY_OK;
}

int slb_pn_gold_generate(const slb_pn_seq_t *cfg)
{
    uint32_t x1;
    uint32_t x2;
    uint32_t n;
    uint32_t i;

    if (cfg == NULL || cfg->c == NULL || cfg->M_PN == 0u) {
        return SLB_PHY_ERR_PARAM;
    }
    if (cfg->c_cap < cfg->M_PN) {
        return SLB_PHY_ERR_BUF;
    }

    x1 = 1u;
    x2 = cfg->c_init & 0x7FFFFFFFu;

    for (i = 0u; i < 1600u; i++) {
        x1 = (x1 >> 1u) | ((((x1 >> 3u) ^ x1) & 1u) << 30u);
        x2 = (x2 >> 1u)
             | ((((x2 >> 3u) ^ (x2 >> 2u) ^ (x2 >> 1u) ^ x2) & 1u) << 30u);
    }

    for (n = 0u; n < cfg->M_PN; n++) {
        cfg->c[n] = (uint8_t)(((x1 >> 30u) ^ (x2 >> 30u)) & 1u);
        x1      = (x1 >> 1u) | ((((x1 >> 3u) ^ x1) & 1u) << 30u);
        x2      = (x2 >> 1u)
             | ((((x2 >> 3u) ^ (x2 >> 2u) ^ (x2 >> 1u) ^ x2) & 1u) << 30u);
    }
    return SLB_PHY_OK;
}

uint32_t slb_qpsk_calc_cinit(const slb_qpsk_cinit_param_t *param)
{
    uint32_t term1;
    uint32_t term2;

    if (param == NULL) {
        return 0u;
    }

    term1 = (uint32_t)(15u * ((uint32_t)param->n + 1u) + (uint32_t)param->l + 1u);
    term2 = (uint32_t)(4u * (uint32_t)param->N_ID + 1u);
    return (uint32_t)((1u << 10) * term1 * term2 + 4u * (uint32_t)param->N_ID
                      + (uint32_t)param->N_CP);
}

int slb_qpsk_ref_generate(const slb_qpsk_seq_t *cfg)
{
    uint32_t     c_init;
    uint8_t      gold[322];
    slb_pn_seq_t pn;
    float        scale;
    uint32_t     m;

    if (cfg == NULL || cfg->r == NULL) {
        return SLB_PHY_ERR_PARAM;
    }
    if (cfg->r_cap < SLB_QPSK_N_SC) {
        return SLB_PHY_ERR_BUF;
    }

    c_init = slb_qpsk_calc_cinit(&cfg->param);
    pn.c_init = c_init;
    pn.M_PN   = 322u;
    pn.c      = gold;
    pn.c_cap  = 322u;
    if (slb_pn_gold_generate(&pn) != SLB_PHY_OK) {
        return SLB_PHY_ERR_PARAM;
    }

    scale = 1.0f / sqrtf(2.0f);
    for (m = 0u; m < SLB_QPSK_N_SC; m++) {
        float i_val = scale * (2.0f * (float)gold[2u * m] - 1.0f);
        float q_val = scale * (2.0f * (float)gold[2u * m + 1u] - 1.0f);
        cfg->r[m].re = i_val;
        cfg->r[m].im = q_val;
    }
    return SLB_PHY_OK;
}

int slb_qpsk_map_to_re(const slb_cplx_t *r, slb_cplx_t *a_kl, uint32_t k)
{
    if (r == NULL || a_kl == NULL) {
        return SLB_PHY_ERR_PARAM;
    }
    if (k >= SLB_QPSK_N_SC) {
        return SLB_PHY_ERR_PARAM;
    }
    if (k == SLB_QPSK_DC_K) {
        a_kl->re = 0.0f;
        a_kl->im = 0.0f;
    } else {
        *a_kl = r[k];
    }
    return SLB_PHY_OK;
}
