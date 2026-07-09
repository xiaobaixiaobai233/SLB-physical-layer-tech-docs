/**
 * @file slb-cot-tti.c
 * @brief 6.2.2.3 COT / TTI 计算与通信域 TTI 规则校验
 */
#include "slb-cot-tti.h"

#include "slb-link.h"
#include "slb-phy-constants.h"

uint32_t slbCalculateTtiRadioFrameCount(uint8_t ttiExponent)
{
    if (!slbValidateTtiExponent(ttiExponent)) {
        return 0U;
    }
    return 1U << ttiExponent;
}

bool slbValidateTtiExponent(uint8_t ttiExponent)
{
    return (ttiExponent <= SLB_TTI_EXPONENT_MAX);
}

bool slbValidateCommDomainTtiStructure(const uint8_t *linkSymbolKinds,
                                       uint8_t symbolCount)
{
    uint8_t i;

    if (linkSymbolKinds == NULL || symbolCount == 0U) {
        return false;
    }
    /* gLink (G 符号) 必须在 TTI 开头 */
    if (linkSymbolKinds[0] != (uint8_t)SLB_LINK_SYMBOL_G) {
        return false;
    }
    /* 末符号必须为切换间隔 */
    if (linkSymbolKinds[symbolCount - 1U] != (uint8_t)SLB_LINK_SYMBOL_GAP) {
        return false;
    }
    /* 至少含一种链路（G 已满足） */
    for (i = 0U; i < symbolCount; ++i) {
        if (linkSymbolKinds[i] > (uint8_t)SLB_LINK_SYMBOL_GAP) {
            return false;
        }
    }
    return true;
}

bool slbCalculateTtiRadioFrameRange(const SlbChannelOccupancyTime *cot,
                                    uint8_t ttiExponent,
                                    uint32_t ttiSequenceIndex,
                                    uint32_t *startRadioFrameOffset,
                                    uint32_t *radioFrameCount)
{
    uint32_t ttiFrameCount;
    uint32_t ttiStart;
    uint32_t ttiEnd;

    if (cot == NULL || startRadioFrameOffset == NULL || radioFrameCount == NULL) {
        return false;
    }
    ttiFrameCount = slbCalculateTtiRadioFrameCount(ttiExponent);
    if (ttiFrameCount == 0U) {
        return false;
    }
    ttiStart = cot->startRadioFrameOffset + ttiSequenceIndex * ttiFrameCount;
    ttiEnd = ttiStart + ttiFrameCount;
    if (ttiEnd > cot->startRadioFrameOffset + cot->radioFrameCount) {
        return false;
    }
    *startRadioFrameOffset = ttiStart;
    *radioFrameCount = ttiFrameCount;
    return true;
}
