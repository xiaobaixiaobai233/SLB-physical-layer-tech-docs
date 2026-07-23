/**
 * @file nl_phy_scfdma.c
 * @brief 6.5.8 SC-FDMA DFT 预编码
 */
#include "nl_phy_scfdma.h"

#include <math.h>

static void nl_dft_segment(const nl_cplx_t *x, nl_cplx_t *y, uint16_t M)
{
    uint16_t k;
    uint16_t i;
    float    norm = 1.0f / sqrtf((float)M);

    for (k = 0u; k < M; k++) {
        float yr = 0.0f;
        float yi = 0.0f;
        for (i = 0u; i < M; i++) {
            float ang = -2.0f * 3.14159265f * (float)i * (float)k / (float)M;
            float c   = cosf(ang);
            float s   = sinf(ang);
            yr += x[i].re * c - x[i].im * s;
            yi += x[i].re * s + x[i].im * c;
        }
        y[k].re = norm * yr;
        y[k].im = norm * yi;
    }
}

int nl_scfdma_precode(const nl_cplx_t *x, nl_cplx_t *y, uint32_t M_symb,
                      uint16_t M_sc_total)
{
    uint32_t l;
    uint32_t L;

    if (x == NULL || y == NULL || M_sc_total == 0u || M_symb == 0u) {
        return NL_PHY_ERR_PARAM;
    }
    if ((M_symb % M_sc_total) != 0u) {
        return NL_PHY_ERR_PARAM;
    }

    L = M_symb / M_sc_total;
    for (l = 0u; l < L; l++) {
        nl_dft_segment(x + l * M_sc_total, y + l * M_sc_total, M_sc_total);
    }
    return NL_PHY_OK;
}
