/**
 * @file nl_phy_scfdma.h
 * @brief 6.5.8 SC-FDMA 波形生成（DFT 预编码）
 */
#ifndef NL_PHY_SCFDMA_H
#define NL_PHY_SCFDMA_H

#include "nl_phy_types.h"

#ifdef __cplusplus
extern "C" {
#endif

int nl_scfdma_precode(const nl_cplx_t *x, nl_cplx_t *y, uint32_t M_symb,
                      uint16_t M_sc_total);

#ifdef __cplusplus
}
#endif

#endif /* NL_PHY_SCFDMA_H */
