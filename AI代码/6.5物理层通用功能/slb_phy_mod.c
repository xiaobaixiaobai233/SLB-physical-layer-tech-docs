/**
 * @file slb_phy_mod.c
 * @brief 6.5.6 调制实现
 */
#include "slb_phy_mod.h"

#include <math.h>

static float slb_gray_level(const uint8_t *b, uint8_t n_bits)
{
    float a = 2.0f - (1.0f - 2.0f * (float)(b[n_bits - 1u] & 1u));
    int   i;

    for (i = (int)n_bits - 2; i >= 0; i--) {
        float layer = 2.0f - (1.0f - 2.0f * (float)(b[i] & 1u));
        a           = layer * a;
    }
    return a;
}

static slb_cplx_t slb_mod_qam_one(const uint8_t *b, uint8_t m, float norm)
{
    slb_cplx_t s;
    float      re_lvl;
    float      im_lvl;
    uint8_t    half = m / 2u;

    re_lvl     = (2.0f * (float)(b[0] & 1u) - 1.0f) * slb_gray_level(b + 2u, half - 1u);
    im_lvl     = (2.0f * (float)(b[1] & 1u) - 1.0f) * slb_gray_level(b + half + 1u, half - 1u);
    s.re       = -norm * re_lvl;
    s.im       = -norm * im_lvl;
    return s;
}

uint8_t slb_mod_bits_per_symbol(slb_mod_type_e mod_type)
{
    switch (mod_type) {
    case SLB_MOD_QPSK:
        return 2u;
    case SLB_MOD_16QAM:
        return 4u;
    case SLB_MOD_64QAM:
        return 6u;
    case SLB_MOD_256QAM:
        return 8u;
    case SLB_MOD_1024QAM:
        return 10u;
    case SLB_MOD_4096QAM:
        return 12u;
    default:
        return 0u;
    }
}

slb_cplx_t slb_mod_qpsk_one(uint8_t b0, uint8_t b1)
{
    slb_cplx_t s;
    float      n = 1.0f / sqrtf(2.0f);
    s.re         = -n * (2.0f * (float)(b0 & 1u) - 1.0f);
    s.im         = -n * (2.0f * (float)(b1 & 1u) - 1.0f);
    return s;
}

slb_cplx_t slb_mod_16qam_one(const uint8_t b[4])
{
    return slb_mod_qam_one(b, 4u, 1.0f / sqrtf(10.0f));
}

slb_cplx_t slb_mod_64qam_one(const uint8_t b[6])
{
    return slb_mod_qam_one(b, 6u, 1.0f / sqrtf(42.0f));
}

slb_cplx_t slb_mod_256qam_one(const uint8_t b[8])
{
    return slb_mod_qam_one(b, 8u, 1.0f / sqrtf(170.0f));
}

slb_cplx_t slb_mod_1024qam_one(const uint8_t b[10])
{
    return slb_mod_qam_one(b, 10u, 1.0f / sqrtf(682.0f));
}

slb_cplx_t slb_mod_4096qam_one(const uint8_t b[12])
{
    return slb_mod_qam_one(b, 12u, 1.0f / sqrtf(2730.0f));
}

int slb_mod_map(const uint8_t *bits, uint32_t N_bit, slb_mod_type_e mod_type,
                slb_cplx_t *symbols, uint32_t *N_sym)
{
    uint8_t m;
    uint32_t i;
    uint32_t n_sym;

    if (bits == NULL || symbols == NULL || N_sym == NULL) {
        return SLB_PHY_ERR_PARAM;
    }

    m = slb_mod_bits_per_symbol(mod_type);
    if (m == 0u || (N_bit % m) != 0u) {
        return SLB_PHY_ERR_PARAM;
    }

    n_sym = N_bit / m;
    for (i = 0u; i < n_sym; i++) {
        const uint8_t *bp = bits + i * m;
        switch (mod_type) {
        case SLB_MOD_QPSK:
            symbols[i] = slb_mod_qpsk_one(bp[0], bp[1]);
            break;
        case SLB_MOD_16QAM:
            symbols[i] = slb_mod_16qam_one(bp);
            break;
        case SLB_MOD_64QAM:
            symbols[i] = slb_mod_64qam_one(bp);
            break;
        case SLB_MOD_256QAM:
            symbols[i] = slb_mod_256qam_one(bp);
            break;
        case SLB_MOD_1024QAM:
            symbols[i] = slb_mod_1024qam_one(bp);
            break;
        case SLB_MOD_4096QAM:
            symbols[i] = slb_mod_4096qam_one(bp);
            break;
        default:
            return SLB_PHY_ERR_PARAM;
        }
    }
    *N_sym = n_sym;
    return SLB_PHY_OK;
}
