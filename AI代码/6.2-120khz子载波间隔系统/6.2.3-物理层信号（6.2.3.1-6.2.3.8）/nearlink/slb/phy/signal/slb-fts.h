/**
 * @file slb-fts.h
 * @brief 第一训练信号 FTS（6.2.3.1.1）
 * airMode = slb
 */
#ifndef SLB_FTS_H
#define SLB_FTS_H

#include "slb-phy-types.h"
#include "slb-zc-seq.h"

#ifdef __cplusplus
extern "C" {
#endif

/** FTS 场景 → 根索引 u 选择 */
typedef enum SlbFtsScenario {
    SLB_FTS_SCENARIO_COMM_SAB_TYPE_AB4 = 0,
    SLB_FTS_SCENARIO_COMM_SAB_TYPE_23  = 1,
    SLB_FTS_SCENARIO_RAB              = 2,
    SLB_FTS_SCENARIO_PASSTHROUGH_SAB   = 3
} SlbFtsScenario;

typedef struct SlbFtsConfig {
    SlbFtsScenario scenario;
    uint16_t       rootIndexU;      /**< 可直接指定 u，或 0 由 scenario 推导 */
    bool           useLookupTable;  /**< true: 查预计算表；false: 实时生成 */
} SlbFtsConfig;

typedef struct SlbFtsSymbolPair {
    SlbSubcarrierBuffer symbol0;    /**< 首符号：加长 CP */
    SlbSubcarrierBuffer symbol1;    /**< 次符号：无 CP */
} SlbFtsSymbolPair;

/** 由场景选择 FTS 根索引 u */
uint16_t slbSelectFtsRootIndex(SlbFtsScenario scenario);

/** 生成 FTS 频域序列 d_FTS(k) */
int slbGenerateFtsSequence(const SlbFtsConfig *config,
                           SlbSubcarrierBuffer ftsSequence);

/**
 * @brief 映射 FTS 到连续两符号（#1、#2）
 * @param ftsSequence  已生成的 d_FTS(k)
 * @param symbolPair   输出双符号 RE
 */
void slbMapFtsToSymbolPair(const SlbSubcarrierBuffer ftsSequence,
                           SlbFtsSymbolPair *symbolPair);

#ifdef __cplusplus
}
#endif

#endif /* SLB_FTS_H */
