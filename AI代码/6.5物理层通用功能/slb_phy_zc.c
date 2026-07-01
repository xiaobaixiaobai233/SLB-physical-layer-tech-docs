/**
 * @file slb_phy_zc.c
 * @brief 6.5.9 ZC 序列与组跳实现
 */
#include "slb_phy_zc.h"
#include "slb_phy_common.h"

#include <math.h>

/* 表 16：m=2 时 phi(n)，30 组 × 12 样点 */
static const int8_t slb_zc_table16[30][12] = {
    {-1, 1, 3, -3, 3, 3, 1, 1, 3, 1, -3, 3},
    {1, 1, 3, 3, 3, -1, 1, -3, -3, 1, -3, 3},
    {1, 1, -3, -3, -3, -1, -3, -3, 1, -3, 1, -1},
    {-1, 1, 1, 1, 1, -1, -3, -3, 1, -3, 3, -1},
    {-1, 3, 1, -1, 1, -1, -3, -1, 1, -1, 1, 3},
    {1, -3, 3, -1, -1, 1, 1, -1, -1, 3, -3, 1},
    {-1, 3, -3, -3, -3, 3, 1, -1, 3, 3, -3, 1},
    {-3, -1, -1, -1, 1, -3, 3, -1, 1, -3, 3, 1},
    {1, -3, 3, 1, -1, -1, -1, 1, 1, 3, -1, 1},
    {1, -3, -1, 3, 3, -1, -3, 1, 1, 1, 1, 1},
    {-1, 3, -1, 1, 1, -3, -3, -1, -3, -3, 3, -1},
    {3, 1, -1, -1, 3, 3, -3, 1, 3, 1, 3, 3},
    {1, -3, 1, 1, -3, 1, 1, 1, -3, -3, -3, 1},
    {3, 3, -3, 3, -3, 1, 1, 3, -1, -3, 3, 3},
    {-3, 1, -1, -3, -1, 3, 1, 3, 3, 3, -1, 1},
    {3, -1, 1, -3, -1, -1, 1, 1, 3, 1, -1, -3},
    {1, 3, 1, -1, 1, 3, 3, 3, -1, -1, 3, -1},
    {-3, 1, 1, 3, -3, 3, -3, -3, 3, 1, 3, -1},
    {-3, 3, 1, 1, -3, 1, -3, -3, -1, -1, 1, -3},
    {-1, 3, 1, 3, 1, -1, -1, 3, -3, -1, -3, -1},
    {-1, -3, 1, 1, 1, 1, 3, 1, -1, 1, -3, -1},
    {-1, 3, -1, 1, -3, -3, -3, -3, -3, 1, -1, -3},
    {1, 1, -3, -3, -3, -3, -1, 3, -3, 1, -3, 3},
    {1, 1, -1, -3, -1, -3, 1, -1, 1, 3, -1, 1},
    {1, 1, 3, 1, 3, 3, -1, 1, -1, -3, -3, 1},
    {1, -3, 3, 3, 1, 3, 3, 1, -3, -1, -1, 3},
    {1, 3, -3, -3, 3, -3, 1, -1, -1, 3, -1, -3},
    {-3, -1, -3, -1, -3, 3, 1, -1, 1, 3, -3, -3},
    {-1, 3, -3, 3, -1, 3, 3, -3, 3, 3, -1, -1},
    {3, -3, -3, -1, -1, -3, -1, 3, -3, 3, 1, -1}
};

static int slb_is_prime(uint16_t n)
{
    uint16_t d;
    if (n < 2u) {
        return 0;
    }
    if (n == 2u) {
        return 1;
    }
    if ((n % 2u) == 0u) {
        return 0;
    }
    for (d = 3u; d * d <= n; d += 2u) {
        if ((n % d) == 0u) {
            return 0;
        }
    }
    return 1;
}

uint16_t slb_zc_largest_prime_less(uint16_t n)
{
    uint16_t p;
    if (n <= 2u) {
        return 0u;
    }
    for (p = n - 1u; p >= 2u; p--) {
        if (slb_is_prime(p)) {
            return p;
        }
    }
    return 0u;
}

int slb_zc_group_hop(uint32_t n_s, uint32_t n_id_x, uint8_t *u_out)
{
    slb_pn_seq_t pn;
    uint32_t     f_gh = 0u;
    uint32_t     f_ss;
    uint32_t     i;
    uint32_t     need;
    uint8_t      seq[512];

    if (u_out == NULL) {
        return SLB_PHY_ERR_PARAM;
    }

    need     = 8u * n_s + 8u;
    pn.c_init = n_id_x / 30u;
    pn.M_PN   = need;
    pn.c      = seq;
    pn.c_cap  = (uint32_t)sizeof(seq);
    if (need > sizeof(seq)) {
        return SLB_PHY_ERR_BUF;
    }
    if (slb_pn_gold_generate(&pn) != SLB_PHY_OK) {
        return SLB_PHY_ERR_PARAM;
    }

    for (i = 0u; i < 8u; i++) {
        f_gh += (uint32_t)seq[8u * n_s + i] << i;
    }

    f_gh  %= 30u;
    f_ss   = (n_id_x / 16u) % 30u;
    *u_out = (uint8_t)((f_gh + f_ss) % 30u);
    return SLB_PHY_OK;
}

int slb_zc_gen_m2_table(uint8_t u, float alpha, slb_cplx_t *seq, uint16_t M_rs)
{
    uint16_t n;

    if (seq == NULL || M_rs == 0u || u >= 30u) {
        return SLB_PHY_ERR_PARAM;
    }

    for (n = 0u; n < M_rs; n++) {
        int8_t  phi = slb_zc_table16[u][n % 12u];
        float   ang = -((float)phi * 3.14159265f / 4.0f) + alpha * (float)n;
        seq[n].re   = cosf(ang);
        seq[n].im   = sinf(ang);
    }
    return SLB_PHY_OK;
}

static int slb_zc_gen_zc(uint8_t u, uint8_t v, float alpha, uint16_t M_rs,
                         slb_cplx_t *seq)
{
    uint16_t N_zc;
    float    q_bar;
    int32_t  q;
    uint16_t n;

    N_zc = slb_zc_largest_prime_less(M_rs);
    if (N_zc == 0u) {
        return SLB_PHY_ERR_PARAM;
    }

    q_bar = (float)N_zc * (float)(u + 1u) / 31.0f;
    q     = (int32_t)floorf(q_bar + 0.5f);
    if (((int32_t)floorf(2.0f * q_bar)) % 2 != 0) {
        q += (int32_t)v * (-1);
    } else {
        q += (int32_t)v;
    }

    for (n = 0u; n < M_rs; n++) {
        uint16_t m = n % N_zc;
        float    ang_zc =
            -3.14159265f * (float)q * (float)m * (float)(m + 1u) / (float)N_zc;
        float ang = ang_zc + alpha * (float)n;
        seq[n].re = cosf(ang);
        seq[n].im = sinf(ang);
    }
    return SLB_PHY_OK;
}

int slb_zc_gen(uint8_t u, uint8_t v, float alpha, uint8_t m, slb_cplx_t *seq,
               uint16_t M_rs)
{
    if (seq == NULL || M_rs == 0u || u >= 30u) {
        return SLB_PHY_ERR_PARAM;
    }

    if (m == 2u) {
        if (v != 0u) {
            return SLB_PHY_ERR_MODE;
        }
        return slb_zc_gen_m2_table(u, alpha, seq, M_rs);
    }
    if (m <= 3u) {
        return SLB_PHY_ERR_MODE;
    }
    if (m <= 5u && v != 0u) {
        return SLB_PHY_ERR_PARAM;
    }
    if (v > 1u) {
        return SLB_PHY_ERR_PARAM;
    }
    return slb_zc_gen_zc(u, v, alpha, M_rs, seq);
}
