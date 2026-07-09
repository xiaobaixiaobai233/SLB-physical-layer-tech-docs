/**
 * @file slb-rx-resource-locate.h
 * @brief 接收侧资源定位六步流程（技术文档 §处理步骤）
 */
#ifndef SLB_RX_RESOURCE_LOCATE_H
#define SLB_RX_RESOURCE_LOCATE_H

#include <stdbool.h>
#include <stdint.h>

#include "slb-carrier.h"
#include "slb-cot-tti.h"
#include "slb-cp-ofdm-symbol.h"
#include "slb-resource-grid.h"
#include "slb-superframe.h"

#ifdef __cplusplus
extern "C" {
#endif

/** 接收侧资源定位上下文（Step 1~6 的中间状态） */
typedef struct SlbRxResourceLocateContext {
    SlbCpOfdmSymbolType          symbolType;
    uint8_t                      symbolCountPerRadioFrame;
    SlbSuperframeIndex           frameIndex;
    SlbTransmissionTimeInterval  currentTti;
    SlbNodeWorkingChannelConfig  channelConfig;
    uint8_t                      targetBaseCarrierIndex;
    SlbResourceGrid             *resourceGrid;
} SlbRxResourceLocateContext;

/** 单步执行结果 */
typedef enum SlbRxLocateStepResult {
    SLB_RX_LOCATE_STEP_OK = 0,
    SLB_RX_LOCATE_STEP_INVALID_PARAM = 1,
    SLB_RX_LOCATE_STEP_CONSTRAINT_VIOLATION = 2
} SlbRxLocateStepResult;

SlbRxLocateStepResult slbRxLocateStep1SelectSymbolCount(
    SlbCpOfdmSymbolType symbolType,
    SlbRxResourceLocateContext *context);

SlbRxLocateStepResult slbRxLocateStep2MarkBoundarySymbols(
    SlbRxResourceLocateContext *context);

SlbRxLocateStepResult slbRxLocateStep3CalculateTtiRange(
    const SlbChannelOccupancyTime *cot,
    uint8_t ttiExponent,
    uint32_t ttiSequenceIndex,
    SlbRxResourceLocateContext *context);

SlbRxLocateStepResult slbRxLocateStep4ParseLinkSymbolSequence(
    const SlbRadioFrameSymbolConfig *symbolConfig,
    uint8_t symbolCount,
    SlbRxResourceLocateContext *context);

SlbRxLocateStepResult slbRxLocateStep5SelectWorkingCarrier(
    uint8_t nodeChannelCount,
    uint8_t subcarrierGroupMergeCount,
    uint8_t targetBaseCarrierIndex,
    SlbRxResourceLocateContext *context);

SlbRxLocateStepResult slbRxLocateStep6ValidateResourceElement(
    const SlbResourceElementIndex *reIndex,
    SlbRxResourceLocateContext *context);

#ifdef __cplusplus
}
#endif

#endif /* SLB_RX_RESOURCE_LOCATE_H */
