/**
 * @file slb-sts.h
 * @brief 第二训练信号 STS（6.2.3.1.2）
 * airMode = slb
 */
#ifndef SLB_STS_H
#define SLB_STS_H

#include "slb-phy-types.h"
#include "slb-zc-seq.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct SlbStsConfig {
    uint8_t  rootIndexU;        /**< u = 1..16，G-ID 低 4 位 + 1 */
    bool     useLookupTable;
} SlbStsConfig;

/**
 * @brief 由 G 节点标识或先发节点 ID 低 4 位计算 STS 根索引
 * @param nodeIdLow4Bits  标识低 4 位（0..15）
 */
uint8_t slbCalculateStsRootIndexFromNodeId(uint8_t nodeIdLow4Bits);

/** 生成 STS 频域序列 2√2·d_STS(k) */
int slbGenerateStsSequence(const SlbStsConfig *config,
                           SlbSubcarrierBuffer stsSequence);

/** 映射 STS 到 SAB/RAB #0 单符号 */
void slbMapStsToSymbol(const SlbSubcarrierBuffer stsSequence,
                       SlbSubcarrierBuffer symbolBuffer);

#ifdef __cplusplus
}
#endif

#endif /* SLB_STS_H */
