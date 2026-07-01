/**
 * @file slb_phy_zc.h
 * @brief 6.5.9 Zadoff-Chu 序列 / 6.5.9.1 序列组跳
 */
#ifndef SLB_PHY_ZC_H
#define SLB_PHY_ZC_H

#include "slb_phy_types.h"

#ifdef __cplusplus
extern "C" {
#endif

int slb_zc_group_hop(uint32_t n_s, uint32_t n_id_x, uint8_t *u_out);

int slb_zc_gen(uint8_t u, uint8_t v, float alpha, uint8_t m, slb_cplx_t *seq,
               uint16_t M_rs);

int slb_zc_gen_m2_table(uint8_t u, float alpha, slb_cplx_t *seq, uint16_t M_rs);

uint16_t slb_zc_largest_prime_less(uint16_t n);

#ifdef __cplusplus
}
#endif

#endif /* SLB_PHY_ZC_H */
