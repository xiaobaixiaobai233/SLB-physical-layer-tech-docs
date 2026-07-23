/**
 * @file nl_phy_types.h
 * @brief NearLink 物理层通用功能 — 公共类型与错误码
 * @note 依据 T/XS 10001-2025 第 6.5 节
 */
#ifndef NL_PHY_TYPES_H
#define NL_PHY_TYPES_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NL_PHY_OK           0
#define NL_PHY_ERR_PARAM   -1
#define NL_PHY_ERR_BUF     -2
#define NL_PHY_ERR_MEAS    -3
#define NL_PHY_ERR_MODE    -3

/** 复数符号（float I/Q） */
typedef struct {
    float re;
    float im;
} nl_cplx_t;

/** 与 nl_cplx_t 同义，部分接口沿用此命名 */
typedef nl_cplx_t nl_cpx_f32_t;

#ifdef __cplusplus
}
#endif

#endif /* NL_PHY_TYPES_H */
