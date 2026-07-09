/**
 * @file slb-zc-seq.h
 * @brief Zadoff-Chu 专用序列生成与 FTS/STS 预计算查表（6.2.3.1）
 * airMode = slb
 */
#ifndef SLB_ZC_SEQ_H
#define SLB_ZC_SEQ_H

#include "slb-phy-types.h"

#ifdef __cplusplus
extern "C" {
#endif

/** FTS 合法根索引 */
typedef enum SlbFtsRootIndex {
    SLB_FTS_ROOT_COMM_SAB_TYPE_AB4 = 1,
    SLB_FTS_ROOT_RAB              = 2,
    SLB_FTS_ROOT_PASSTHROUGH_SAB    = 159,
    SLB_FTS_ROOT_COMM_SAB_TYPE_23  = 160
} SlbFtsRootIndex;

/**
 * @brief 按闭式公式生成 ZC 序列 x_u(n) = exp(-jπ u n(n+1)/N)
 * @param sequenceLengthN  ZC 长度 N
 * @param rootIndexU       根索引 u
 * @param dcSubcarrierIndex DC 子载波（无则传 UINT16_MAX）
 * @param sequenceOut      输出，长度 sequenceLengthN
 */
void slbGenerateZcSequence(uint16_t sequenceLengthN,
                           uint16_t rootIndexU,
                           uint16_t dcSubcarrierIndex,
                           SlbComplex *sequenceOut);

/**
 * @brief 生成 FTS 专用 ZC 序列 d_FTS(n)，N=161，n=80 为 0
 */
void slbGenerateFtsZcSequence(uint16_t rootIndexU,
                              SlbSubcarrierBuffer ftsSequence);

/**
 * @brief 生成 STS 梳状 ZC 序列 d_STS(n)，每 8 子载波 1 个有效 RE
 */
void slbGenerateStsZcSequence(uint8_t rootIndexU,
                              SlbSubcarrierBuffer stsSequence);

/** 初始化 FTS 查表（4 个 u × 161） */
int slbInitFtsLookupTables(void);

/** 初始化 STS 查表（16 个 u × 161） */
int slbInitStsLookupTables(void);

/** 从查表读取 FTS 序列；须先 slbInitFtsLookupTables */
const SlbComplex *slbSelectFtsFromLookupTable(uint16_t rootIndexU);

/** 从查表读取 STS 序列（含 2√2 缩放）；须先 slbInitStsLookupTables */
const SlbComplex *slbSelectStsFromLookupTable(uint8_t rootIndexU);

#ifdef __cplusplus
}
#endif

#endif /* SLB_ZC_SEQ_H */
