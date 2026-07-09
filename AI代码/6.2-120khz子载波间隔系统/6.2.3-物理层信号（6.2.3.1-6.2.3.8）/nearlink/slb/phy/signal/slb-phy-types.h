/**
 * @file slb-phy-types.h
 * @brief SLB 120 kHz 物理层信号公共类型与常量（6.2.2 / 6.2.3）
 * airMode = slb
 */
#ifndef SLB_PHY_TYPES_H
#define SLB_PHY_TYPES_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/** 子载波总数（含 DC），k = 0..160 */
#define SLB_SUBCARRIER_COUNT           161

/** DC 子载波索引，a_{80,l} ≡ 0 */
#define SLB_DC_SUBCARRIER_INDEX        80

/** 超帧内无线帧数 */
#define SLB_RADIO_FRAME_COUNT          8

/** QPSK 序列 r_{n,l}(m) 中 m 的最大索引 */
#define SLB_QPSK_SEQUENCE_LENGTH       161

/** Gold 序列生成所需最大下标（c(321) 需 x(i+1600)，i=321 → 1921） */
#define SLB_GOLD_X1_LUT_SIZE           1922

/** FTS ZC 序列长度 N */
#define SLB_FTS_ZC_LENGTH              161

/** STS ZC 梳齿组数（每 8 子载波 1 个有效 RE） */
#define SLB_STS_ZC_LENGTH              21

/** STS 梳齿间隔（子载波） */
#define SLB_STS_COMB_SPACING           8

/** FTS 合法根索引 u 个数 */
#define SLB_FTS_ROOT_INDEX_COUNT       4

/** STS 合法根索引 u：1..16 */
#define SLB_STS_ROOT_INDEX_MIN         1
#define SLB_STS_ROOT_INDEX_MAX         16

/** STS 功率缩放因子 2√2 */
#define SLB_STS_POWER_SCALE            2.8284271247461903f

/** QPSK 归一化因子 1/√2 */
#define SLB_QPSK_NORM_SCALE            0.7071067811865476f

/** 最大天线端口数（实现上限，依高层配置） */
#define SLB_ANTENNA_PORT_MAX           8

/** 复数资源元素值 a_{k,l}^{(p)} */
typedef struct SlbComplex {
    float re;
    float im;
} SlbComplex;

/** 符号类型 → N_CP 映射（6.5.3） */
typedef enum SlbSymbolType {
    SLB_SYMBOL_TYPE_ONE_A = 0,
    SLB_SYMBOL_TYPE_ONE_B = 1,
    SLB_SYMBOL_TYPE_TWO   = 2,
    SLB_SYMBOL_TYPE_THREE = 3,
    SLB_SYMBOL_TYPE_FOUR  = 4
} SlbSymbolType;

/** 根据符号类型返回 N_CP（6.5.3 表） */
static inline uint8_t slbSelectNCpFromSymbolType(SlbSymbolType symbolType)
{
    switch (symbolType) {
    case SLB_SYMBOL_TYPE_ONE_A:
    case SLB_SYMBOL_TYPE_ONE_B: return 3;
    case SLB_SYMBOL_TYPE_TWO:   return 2;
    case SLB_SYMBOL_TYPE_THREE: return 1;
    case SLB_SYMBOL_TYPE_FOUR:  return 0;
    default:                    return 3;
    }
}

/** 单符号频域 RE 缓冲（161 子载波） */
typedef SlbComplex SlbSubcarrierBuffer[SLB_SUBCARRIER_COUNT];

/** 将缓冲清零 */
void slbClearSubcarrierBuffer(SlbSubcarrierBuffer buffer);

/** DC 子载波置零 */
void slbZeroDcSubcarrier(SlbSubcarrierBuffer buffer);

#ifdef __cplusplus
}
#endif

#endif /* SLB_PHY_TYPES_H */
