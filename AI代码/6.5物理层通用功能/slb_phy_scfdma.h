/**
 * @file slb_phy_scfdma.h
 * @brief 6.5.8 SC-FDMA 波形生成（DFT 预编码）
 */
#ifndef SLB_PHY_SCFDMA_H
#define SLB_PHY_SCFDMA_H

#include "slb_phy_types.h"

#ifdef __cplusplus
extern "C" {
#endif

int slb_scfdma_precode(const slb_cplx_t *x, slb_cplx_t *y, uint32_t M_symb,
                       uint16_t M_sc_total);

#ifdef __cplusplus
}
#endif

#endif /* SLB_PHY_SCFDMA_H */
