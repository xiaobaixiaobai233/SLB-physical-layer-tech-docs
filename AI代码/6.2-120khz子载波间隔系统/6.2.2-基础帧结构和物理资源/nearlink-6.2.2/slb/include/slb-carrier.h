/**
 * @file slb-carrier.h
 * @brief 6.2.2.4 基础信道、载波、子载波组与表 4（N_CH / L_CH）
 */
#ifndef SLB_CARRIER_H
#define SLB_CARRIER_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** 节点工作信道配置（高层/MAC 输入） */
typedef struct SlbNodeWorkingChannelConfig {
    uint8_t nodeChannelCount;          /**< N_CH：连续基础信道数 */
    uint8_t subcarrierGroupMergeCount; /**< L_CH：每工作子载波组含基础子载波组数 */
} SlbNodeWorkingChannelConfig;

/**
 * @brief 按表 4 由 N_CH 查 L_CH
 * @return true 成功；false N_CH 不在表 4 支持列表
 */
bool slbSelectSubcarrierGroupMergeCount(uint8_t nodeChannelCount,
                                        uint8_t *subcarrierGroupMergeCount);

/**
 * @brief 校验 N_CH 与 L_CH 是否满足表 4 配对
 */
bool slbValidateNodeChannelPairing(uint8_t nodeChannelCount,
                                   uint8_t subcarrierGroupMergeCount);

/**
 * @brief 计算工作子载波组编号上界：floor(16 * N_CH / L_CH) - 1
 */
uint16_t slbCalculateWorkingSubcarrierGroupIndexMax(
    const SlbNodeWorkingChannelConfig *config);

/**
 * @brief 校验子载波索引 k 是否可映射（k != 80 DC）
 */
bool slbValidateMappableSubcarrierIndex(uint16_t subcarrierIndex);

/**
 * @brief 由子载波索引 k 计算基础子载波组编号（跳过 DC #80）
 * @param subcarrierIndex 0 ~ 160
 * @param groupIndex 输出 #0 ~ #15（单基础载波内）
 */
bool slbCalculateBaseSubcarrierGroupIndex(uint16_t subcarrierIndex,
                                          uint8_t *groupIndex);

#ifdef __cplusplus
}
#endif

#endif /* SLB_CARRIER_H */
