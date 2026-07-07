/**
 * @file nl_phy_scram.h
 * @brief 6.5.7 比特加扰
 */
#ifndef NL_PHY_SCRAM_H
#define NL_PHY_SCRAM_H

#include "nl_phy_types.h"

#ifdef __cplusplus
extern "C" {
#endif

int      nl_scram_apply(const uint8_t *bits_in, uint8_t *bits_out, uint32_t N_bit,
                        uint32_t c_init);
uint32_t nl_scram_calc_cinit_data(uint8_t n_f_low7, uint32_t n_g_pid);

#ifdef __cplusplus
}
#endif

#endif /* NL_PHY_SCRAM_H */
