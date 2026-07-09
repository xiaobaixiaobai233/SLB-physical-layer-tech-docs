/**
 * @file slb-superframe.h
 * @brief 6.2.2.2 超帧、无线帧与链路符号类型（G/T/A/D）
 */
#ifndef SLB_SUPERFRAME_H
#define SLB_SUPERFRAME_H

#include <stdint.h>

#include "slb-cp-ofdm-symbol.h"

#ifdef __cplusplus
extern "C" {
#endif

/** 无线帧内链路符号类型（6.2.2.2） */
typedef enum SlbLinkSymbolKind {
    SLB_LINK_SYMBOL_G = 0,  /**< G 符号，gLink 下行 */
    SLB_LINK_SYMBOL_T = 1,  /**< T 符号，tLink 上行 */
    SLB_LINK_SYMBOL_A = 2,  /**< A 符号，auxLink 附链路 */
    SLB_LINK_SYMBOL_D = 3,  /**< D 符号，passthroughLink 直通链路 */
    SLB_LINK_SYMBOL_GAP = 4 /**< 切换间隔符号 */
} SlbLinkSymbolKind;

/** 超帧时间坐标 */
typedef struct SlbSuperframeIndex {
    uint16_t superframeIndex; /**< #0 ~ #65535 循环 */
    uint8_t  radioFrameIndex; /**< 超帧内 #0 ~ #7 */
    uint8_t  symbolIndex;     /**< 无线帧内符号 #l */
} SlbSuperframeIndex;

/** 无线帧符号配置条目（由高层/MAC 配置） */
typedef struct SlbRadioFrameSymbolConfig {
    SlbLinkSymbolKind symbolKind;
} SlbRadioFrameSymbolConfig;

/**
 * @brief 计算超帧时长（Ts）
 */
uint32_t slbCalculateSuperframeDurationTs(void);

/**
 * @brief 校验超帧/无线帧/符号索引是否在合法范围
 */
bool slbValidateSuperframeIndex(const SlbSuperframeIndex *index,
                                SlbCpOfdmSymbolType symbolType);

#ifdef __cplusplus
}
#endif

#endif /* SLB_SUPERFRAME_H */
