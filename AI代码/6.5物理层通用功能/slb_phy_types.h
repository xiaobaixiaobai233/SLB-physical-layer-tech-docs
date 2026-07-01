/**
 * @file slb_phy_types.h
 * @brief SLB 物理层 6.5 通用功能 — 公共类型与错误码
 * @note 依据 T/XS 10001-2025 第 6.5 节
 */
#ifndef SLB_PHY_TYPES_H
#define SLB_PHY_TYPES_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SLB_PHY_OK           0
#define SLB_PHY_ERR_PARAM   -1
#define SLB_PHY_ERR_BUF     -2
#define SLB_PHY_ERR_MEAS    -3
#define SLB_PHY_ERR_MODE    -3

/** 复数符号（float I/Q） */
typedef struct {
    float re;
    float im;
} slb_cplx_t;

/** 与 slb_cplx_t 同义，部分接口沿用此命名 */
typedef slb_cplx_t slb_cpx_f32_t;

#ifdef __cplusplus
}
#endif

#endif /* SLB_PHY_TYPES_H */
