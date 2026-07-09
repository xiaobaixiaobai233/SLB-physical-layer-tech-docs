/**
 * @file slb-superframe.c
 * @brief 6.2.2.2 超帧与无线帧
 */
#include "slb-superframe.h"

#include "slb-phy-constants.h"

uint32_t slbCalculateSuperframeDurationTs(void)
{
    return SLB_SUPERFRAME_DURATION_TS;
}

bool slbValidateSuperframeIndex(const SlbSuperframeIndex *index,
                                SlbCpOfdmSymbolType symbolType)
{
    uint8_t symbolCount;

    if (index == NULL || symbolType > SLB_CP_OFDM_SYMBOL_TYPE_4) {
        return false;
    }
    if (index->superframeIndex > SLB_SUPERFRAME_INDEX_MAX) {
        return false;
    }
    if (index->radioFrameIndex >= SLB_RADIO_FRAME_COUNT_PER_SUPERFRAME) {
        return false;
    }
    symbolCount = slbCalculateSymbolCountPerRadioFrame(symbolType);
    if (symbolCount == 0U || index->symbolIndex >= symbolCount) {
        return false;
    }
    return true;
}
