/**
 * @file slb-pas.h
 * @brief 物理层数据信息相位调整信号 PAS（6.2.3.8）
 * airMode = slb
 */
#ifndef SLB_PAS_H
#define SLB_PAS_H

#include "slb-gold-seq.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct SlbPasSubcarrierIndices {
    uint16_t k0SubcarrierIndex;  /**< 被调度最小子载波组中最大子载波 */
    uint16_t k1SubcarrierIndex;  /**< 被调度最大子载波组中最小子载波 */
} SlbPasSubcarrierIndices;

typedef struct SlbPasConfig {
    SlbRnLContext            rnLContext;
    SlbPasSubcarrierIndices subcarrierIndices;
    bool                     skipDmrsDRadioFrame; /**< 含 DMRS-D 的无线帧不映射 */
} SlbPasConfig;

void slbGeneratePasSequence(const SlbPasConfig *config,
                            SlbSubcarrierBuffer pasSequence);

/**
 * @brief 映射 PAS：a_{k,l}^{(0)} = r_{n,l}(k) 仅 k=k0,k1；其他为 0
 */
void slbMapPasToResourceGrid(const SlbPasConfig *config,
                             const SlbSubcarrierBuffer pasSequence,
                             SlbSubcarrierBuffer port0SymbolBuffer);

/**
 * @brief 由调度子载波组计算 k0、k1
 */
SlbPasSubcarrierIndices slbCalculatePasSubcarrierIndices(
    uint16_t minGroupStartSubcarrierIndex,
    uint16_t minGroupEndSubcarrierIndex,
    uint16_t maxGroupStartSubcarrierIndex,
    uint16_t maxGroupEndSubcarrierIndex);

#ifdef __cplusplus
}
#endif

#endif /* SLB_PAS_H */
