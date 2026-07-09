/**
 * @file slb-rx-resource-locate.c
 * @brief 接收侧资源定位 Step 1~6（技术文档 §处理步骤）
 */
#include "slb-rx-resource-locate.h"

#include "slb-carrier.h"
#include "slb-cp-ofdm-symbol.h"
#include "slb-resource-element.h"

SlbRxLocateStepResult slbRxLocateStep1SelectSymbolCount(
    SlbCpOfdmSymbolType symbolType,
    SlbRxResourceLocateContext *context)
{
    uint8_t symbolCount;

    if (context == NULL || symbolType > SLB_CP_OFDM_SYMBOL_TYPE_4) {
        return SLB_RX_LOCATE_STEP_INVALID_PARAM;
    }
    symbolCount = slbCalculateSymbolCountPerRadioFrame(symbolType);
    if (symbolCount == 0U) {
        return SLB_RX_LOCATE_STEP_CONSTRAINT_VIOLATION;
    }
    context->symbolType = symbolType;
    context->symbolCountPerRadioFrame = symbolCount;
    return SLB_RX_LOCATE_STEP_OK;
}

SlbRxLocateStepResult slbRxLocateStep2MarkBoundarySymbols(
    SlbRxResourceLocateContext *context)
{
    uint8_t symbolIndex;
    uint8_t symbolCount;

    if (context == NULL) {
        return SLB_RX_LOCATE_STEP_INVALID_PARAM;
    }
    symbolCount = context->symbolCountPerRadioFrame;
    for (symbolIndex = 0U; symbolIndex < symbolCount; ++symbolIndex) {
        if (!slbValidateIsBoundarySymbol(context->symbolType, symbolIndex)) {
            continue;
        }
        /* 边界 CP 标记由上层符号定时模块消费；此处仅校验索引合法 */
        if (!slbValidateSuperframeIndex(&context->frameIndex, context->symbolType)) {
            return SLB_RX_LOCATE_STEP_CONSTRAINT_VIOLATION;
        }
    }
    return SLB_RX_LOCATE_STEP_OK;
}

SlbRxLocateStepResult slbRxLocateStep3CalculateTtiRange(
    const SlbChannelOccupancyTime *cot,
    uint8_t ttiExponent,
    uint32_t ttiSequenceIndex,
    SlbRxResourceLocateContext *context)
{
    uint32_t startOffset;
    uint32_t frameCount;

    if (context == NULL || cot == NULL) {
        return SLB_RX_LOCATE_STEP_INVALID_PARAM;
    }
    if (!slbCalculateTtiRadioFrameRange(cot, ttiExponent, ttiSequenceIndex,
                                        &startOffset, &frameCount)) {
        return SLB_RX_LOCATE_STEP_CONSTRAINT_VIOLATION;
    }
    context->currentTti.ttiExponent = ttiExponent;
    context->currentTti.ttiSequenceIndex = ttiSequenceIndex;
    context->currentTti.radioFrameCount = frameCount;
    return SLB_RX_LOCATE_STEP_OK;
}

SlbRxLocateStepResult slbRxLocateStep4ParseLinkSymbolSequence(
    const SlbRadioFrameSymbolConfig *symbolConfig,
    uint8_t symbolCount,
    SlbRxResourceLocateContext *context)
{
    uint8_t i;

    if (context == NULL || symbolConfig == NULL || symbolCount == 0U) {
        return SLB_RX_LOCATE_STEP_INVALID_PARAM;
    }
    if (symbolCount != context->symbolCountPerRadioFrame) {
        return SLB_RX_LOCATE_STEP_CONSTRAINT_VIOLATION;
    }
    for (i = 0U; i < symbolCount; ++i) {
        if (symbolConfig[i].symbolKind == SLB_LINK_SYMBOL_D) {
            return SLB_RX_LOCATE_STEP_CONSTRAINT_VIOLATION;
        }
    }
    return SLB_RX_LOCATE_STEP_OK;
}

SlbRxLocateStepResult slbRxLocateStep5SelectWorkingCarrier(
    uint8_t nodeChannelCount,
    uint8_t subcarrierGroupMergeCount,
    uint8_t targetBaseCarrierIndex,
    SlbRxResourceLocateContext *context)
{
    if (context == NULL) {
        return SLB_RX_LOCATE_STEP_INVALID_PARAM;
    }
    if (!slbValidateNodeChannelPairing(nodeChannelCount, subcarrierGroupMergeCount)) {
        return SLB_RX_LOCATE_STEP_CONSTRAINT_VIOLATION;
    }
    if (targetBaseCarrierIndex >= nodeChannelCount) {
        return SLB_RX_LOCATE_STEP_CONSTRAINT_VIOLATION;
    }
    context->channelConfig.nodeChannelCount = nodeChannelCount;
    context->channelConfig.subcarrierGroupMergeCount = subcarrierGroupMergeCount;
    context->targetBaseCarrierIndex = targetBaseCarrierIndex;
    return SLB_RX_LOCATE_STEP_OK;
}

SlbRxLocateStepResult slbRxLocateStep6ValidateResourceElement(
    const SlbResourceElementIndex *reIndex,
    SlbRxResourceLocateContext *context)
{
    if (context == NULL || reIndex == NULL) {
        return SLB_RX_LOCATE_STEP_INVALID_PARAM;
    }
    if (!slbValidateResourceElementIndex(reIndex, context->symbolCountPerRadioFrame)) {
        return SLB_RX_LOCATE_STEP_CONSTRAINT_VIOLATION;
    }
    if (context->resourceGrid != NULL) {
        SlbComplex value = slbReadResourceElement(context->resourceGrid, reIndex);
        if (value.real == 0.0f && value.imag == 0.0f) {
            /* 空闲 RE 映射 0，接收端不得当作有效载荷（约束 §实现要点） */
            return SLB_RX_LOCATE_STEP_CONSTRAINT_VIOLATION;
        }
    }
    return SLB_RX_LOCATE_STEP_OK;
}
