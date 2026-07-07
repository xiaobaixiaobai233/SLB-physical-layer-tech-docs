/**
 * @file nl_phy_zc.h
 * @brief 6.5.9 Zadoff-Chu 序列 / 6.5.9.1 序列组跳
 */
#ifndef NL_PHY_ZC_H
#define NL_PHY_ZC_H

#include "nl_phy_types.h"

#ifdef __cplusplus
extern "C" {
#endif

int nl_zc_group_hop(uint32_t n_s, uint32_t n_id_x, uint8_t *u_out);

int nl_zc_gen(uint8_t u, uint8_t v, float alpha, uint8_t m, nl_cplx_t *seq,
              uint16_t M_rs);

int nl_zc_gen_m2_table(uint8_t u, float alpha, nl_cplx_t *seq, uint16_t M_rs);

uint16_t nl_zc_largest_prime_less(uint16_t n);

#ifdef __cplusplus
}
#endif

#endif /* NL_PHY_ZC_H */
