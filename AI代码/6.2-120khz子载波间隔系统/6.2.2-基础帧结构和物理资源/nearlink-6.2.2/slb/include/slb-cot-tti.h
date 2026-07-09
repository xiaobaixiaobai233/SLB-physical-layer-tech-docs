/**
 * @file slb-cot-tti.h
 * @brief 6.2.2.3 信道占用时间（COT）与传输时间间隔（TTI）
 */
#ifndef SLB_COT_TTI_H
#define SLB_COT_TTI_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** 传输模式（6.2.2.3） */
typedef enum SlbTransmissionMode {
    SLB_TRANSMISSION_MODE_DISCONTINUOUS = 0, /**< 非连续：竞争抢占 */
    SLB_TRANSMISSION_MODE_CONTINUOUS    = 1  /**< 连续传输 */
} SlbTransmissionMode;

/** COT 描述：竞争成功后连续占用的无线帧块 */
typedef struct SlbChannelOccupancyTime {
    uint32_t startRadioFrameOffset; /**< COT 起始无线帧（相对计数） */
    uint32_t radioFrameCount;       /**< COT 包含的无线帧数（整数个） */
} SlbChannelOccupancyTime;

/** TTI 描述 */
typedef struct SlbTransmissionTimeInterval {
    uint8_t  ttiExponent;      /**< N_TTI，0 ~ 6 */
    uint32_t radioFrameCount;  /**< 2^N_TTI 个无线帧 */
    uint32_t ttiSequenceIndex; /**< COT 内 TTI 顺序号 #0, #1, ... */
} SlbTransmissionTimeInterval;

/**
 * @brief 计算 TTI 无线帧数 = 2^N_TTI
 */
uint32_t slbCalculateTtiRadioFrameCount(uint8_t ttiExponent);

/**
 * @brief 校验 N_TTI 是否在 0 ~ 6
 */
bool slbValidateTtiExponent(uint8_t ttiExponent);

/**
 * @brief 校验通信域 TTI 结构规则：
 *        - 至少含 gLink 且 gLink 在 TTI 开头
 *        - TTI 末符号必须为切换间隔
 * @param linkSymbolKinds 本 TTI 内按时间顺序的链路符号类型数组
 * @param symbolCount 数组长度
 */
bool slbValidateCommDomainTtiStructure(const uint8_t *linkSymbolKinds,
                                       uint8_t symbolCount);

/**
 * @brief 根据 COT 起始与 TTI 指数，计算第 ttiSequenceIndex 个 TTI 的无线帧范围
 */
bool slbCalculateTtiRadioFrameRange(const SlbChannelOccupancyTime *cot,
                                    uint8_t ttiExponent,
                                    uint32_t ttiSequenceIndex,
                                    uint32_t *startRadioFrameOffset,
                                    uint32_t *radioFrameCount);

#ifdef __cplusplus
}
#endif

#endif /* SLB_COT_TTI_H */
