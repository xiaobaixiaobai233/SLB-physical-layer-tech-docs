/**
 * @file nl_phy_mod.h
 * @brief 6.5.6 调制方式（QPSK ~ 4096QAM）
 */
#ifndef NL_PHY_MOD_H
#define NL_PHY_MOD_H

#include "nl_phy_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    NL_MOD_QPSK = 0,
    NL_MOD_16QAM,
    NL_MOD_64QAM,
    NL_MOD_256QAM,
    NL_MOD_1024QAM,
    NL_MOD_4096QAM
} nl_mod_type_e;

uint8_t    nl_mod_bits_per_symbol(nl_mod_type_e mod_type);
int        nl_mod_map(const uint8_t *bits, uint32_t N_bit, nl_mod_type_e mod_type,
                      nl_cplx_t *symbols, uint32_t *N_sym);
nl_cplx_t  nl_mod_qpsk_one(uint8_t b0, uint8_t b1);
nl_cplx_t  nl_mod_16qam_one(const uint8_t b[4]);
nl_cplx_t  nl_mod_64qam_one(const uint8_t b[6]);
nl_cplx_t  nl_mod_256qam_one(const uint8_t b[8]);
nl_cplx_t  nl_mod_1024qam_one(const uint8_t b[10]);
nl_cplx_t  nl_mod_4096qam_one(const uint8_t b[12]);

#ifdef __cplusplus
}
#endif

#endif /* NL_PHY_MOD_H */
