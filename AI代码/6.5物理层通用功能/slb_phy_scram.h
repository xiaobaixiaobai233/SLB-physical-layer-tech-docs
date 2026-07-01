/**
 * @file slb_phy_scram.h
 * @brief 6.5.7 比特加扰
 */
#ifndef SLB_PHY_SCRAM_H
#define SLB_PHY_SCRAM_H

#include "slb_phy_types.h"

#ifdef __cplusplus
extern "C" {
#endif

int      slb_scram_apply(const uint8_t *bits_in, uint8_t *bits_out, uint32_t N_bit,
                         uint32_t c_init);
uint32_t slb_scram_cinit_data(uint8_t n_f_low7, uint32_t n_g_pid);

#ifdef __cplusplus
}
#endif

#endif /* SLB_PHY_SCRAM_H */
