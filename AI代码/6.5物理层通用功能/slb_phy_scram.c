/**
 * @file slb_phy_scram.c
 * @brief 6.5.7 比特加扰实现
 */
#include "slb_phy_scram.h"
#include "slb_phy_common.h"

int slb_scram_cinit_data(uint8_t n_f_low7, uint32_t n_g_pid)
{
    return (uint32_t)n_f_low7 * (1u << 24) + (n_g_pid & 0xFFFFFFu);
}

int slb_scram_apply(const uint8_t *bits_in, uint8_t *bits_out, uint32_t N_bit,
                    uint32_t c_init)
{
    slb_pn_seq_t pn;
    uint8_t      gold[8192];
    uint32_t     i;

    if (bits_in == NULL || bits_out == NULL || N_bit == 0u) {
        return SLB_PHY_ERR_PARAM;
    }
    if (N_bit > sizeof(gold)) {
        return SLB_PHY_ERR_BUF;
    }

    pn.c_init = c_init;
    pn.M_PN   = N_bit;
    pn.c      = gold;
    pn.c_cap  = N_bit;
    if (slb_pn_gold_generate(&pn) != SLB_PHY_OK) {
        return SLB_PHY_ERR_PARAM;
    }

    for (i = 0u; i < N_bit; i++) {
        bits_out[i] = (uint8_t)((bits_in[i] & 1u) ^ gold[i]);
    }
    return SLB_PHY_OK;
}
