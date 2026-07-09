/**
 * @file slb-dmrs-d.h
 * @brief 数据信息解调参考信号 DMRS-D（6.2.3.6）
 * airMode = slb
 */
#ifndef SLB_DMRS_D_H
#define SLB_DMRS_D_H

#include "slb-ref-signal-common.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum SlbDmrsDCombMode {
    SLB_DMRS_D_COMB_CONTINUOUS = 1,
    SLB_DMRS_D_COMB_SPARSE     = 2
} SlbDmrsDCombMode;

typedef struct SlbDmrsDConfig {
    SlbRnLContext            rnLContext;
    SlbSubcarrierGroupConfig subcarrierGroup;
    SlbDmrsDCombMode         combMode;          /**< N_comb: 1 或 2 */
    uint8_t                  antennaPortCount;  /**< N_port */
    uint8_t                  dmrsSymbolCount; /**< N = N_port / N_comb */
    uint8_t                  dmrsTimeGranularityRadioFrames; /**< 默认 1 */
    bool                     hasSubcarrierGroup;
} SlbDmrsDConfig;

void slbGenerateDmrsDSequence(const SlbDmrsDConfig *config,
                              SlbSubcarrierBuffer dmrsDSequence);

/**
 * @brief 计算 DMRS-D 符号数 N = N_port / N_comb
 */
uint8_t slbCalculateDmrsDSymbolCount(uint8_t antennaPortCount,
                                     SlbDmrsDCombMode combMode);

/**
 * @brief 映射第 M 个 DMRS-D 符号到端口 [M·N_comb, M·N_comb + N_comb)
 * @param dmrsSymbolIndex  M ∈ [0, N)
 * @param symbolBuffers    [portCount][161]
 */
void slbMapDmrsDToResourceGrid(const SlbDmrsDConfig *config,
                               uint8_t dmrsSymbolIndex,
                               const SlbSubcarrierBuffer dmrsDSequence,
                               SlbComplex symbolBuffers[][SLB_SUBCARRIER_COUNT]);

/** 判断梳齿模式下子载波是否属于指定天线端口 */
bool slbIsDmrsDSubcarrierForPort(uint16_t subcarrierIndex,
                                 uint8_t antennaPort,
                                 SlbDmrsDCombMode combMode);

#ifdef __cplusplus
}
#endif

#endif /* SLB_DMRS_D_H */
