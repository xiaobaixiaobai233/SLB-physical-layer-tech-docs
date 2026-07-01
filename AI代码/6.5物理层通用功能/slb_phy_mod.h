/**
 * @file slb_phy_mod.h
 * @brief 6.5.6 调制方式（QPSK ~ 4096QAM）
 */
#ifndef SLB_PHY_MOD_H
#define SLB_PHY_MOD_H

#include "slb_phy_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    SLB_MOD_QPSK = 0,
    SLB_MOD_16QAM,
    SLB_MOD_64QAM,
    SLB_MOD_256QAM,
    SLB_MOD_1024QAM,
    SLB_MOD_4096QAM
} slb_mod_type_e;

uint8_t    slb_mod_bits_per_symbol(slb_mod_type_e mod_type);
int        slb_mod_map(const uint8_t *bits, uint32_t N_bit, slb_mod_type_e mod_type,
                       slb_cplx_t *symbols, uint32_t *N_sym);
slb_cplx_t slb_mod_qpsk_one(uint8_t b0, uint8_t b1);
slb_cplx_t slb_mod_16qam_one(const uint8_t b[4]);
slb_cplx_t slb_mod_64qam_one(const uint8_t b[6]);
slb_cplx_t slb_mod_256qam_one(const uint8_t b[8]);
slb_cplx_t slb_mod_1024qam_one(const uint8_t b[10]);
slb_cplx_t slb_mod_4096qam_one(const uint8_t b[12]);

#ifdef __cplusplus
}
#endif

#endif /* SLB_PHY_MOD_H */
