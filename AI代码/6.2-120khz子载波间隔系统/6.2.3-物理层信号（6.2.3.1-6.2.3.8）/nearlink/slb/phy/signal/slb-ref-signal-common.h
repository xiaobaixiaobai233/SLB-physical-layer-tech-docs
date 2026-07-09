/**
 * @file slb-ref-signal-common.h
 * @brief SRS/CSI-RS/DMRS/PAS 等参考信号公共 RE 映射辅助
 * airMode = slb
 */
#ifndef SLB_REF_SIGNAL_COMMON_H
#define SLB_REF_SIGNAL_COMMON_H

#include "slb-phy-types.h"
#include "slb-gold-seq.h"

#ifdef __cplusplus
extern "C" {
#endif

/** 梳齿配置：在工作子载波组内按间隔选取 RE */
typedef struct SlbCombSubcarrierConfig {
    uint8_t  combSpacing;       /**< 梳齿间隔（子载波数），1=连续 */
    uint8_t  combOffset;        /**< 梳齿偏移 0..combSpacing-1 */
} SlbCombSubcarrierConfig;

/** 频域工作子载波组 */
typedef struct SlbSubcarrierGroupConfig {
    uint16_t startSubcarrierIndex;
    uint16_t endSubcarrierIndex; /**< 含端点 */
} SlbSubcarrierGroupConfig;

/** 多天线端口映射配置 */
typedef struct SlbAntennaPortConfig {
    uint8_t activeAntennaPort;  /**< p_0：唯一映射序列的端口 */
    uint8_t antennaPortCount;
} SlbAntennaPortConfig;

/** 判断子载波是否在工作组且满足梳齿 */
bool slbIsSubcarrierInCombSet(uint16_t subcarrierIndex,
                              const SlbSubcarrierGroupConfig *groupConfig,
                              const SlbCombSubcarrierConfig *combConfig);

/**
 * @brief 将 r_{n,l}(k) 按梳齿/端口规则映射到 a_{k,l}^{(p)}
 * @param rnLSequence       已生成的 161 点 QPSK 序列
 * @param groupConfig       工作子载波组（NULL 表示全带宽 0..160）
 * @param combConfig        梳齿配置（NULL 或 combSpacing=1 表示连续）
 * @param antennaPortConfig 端口配置
 * @param antennaPortBuffers 输出 [portCount][161]
 */
void slbMapRnLToAntennaPorts(const SlbSubcarrierBuffer rnLSequence,
                             const SlbSubcarrierGroupConfig *groupConfig,
                             const SlbCombSubcarrierConfig *combConfig,
                             const SlbAntennaPortConfig *antennaPortConfig,
                             SlbComplex antennaPortBuffers[][SLB_SUBCARRIER_COUNT]);

#ifdef __cplusplus
}
#endif

#endif /* SLB_REF_SIGNAL_COMMON_H */
