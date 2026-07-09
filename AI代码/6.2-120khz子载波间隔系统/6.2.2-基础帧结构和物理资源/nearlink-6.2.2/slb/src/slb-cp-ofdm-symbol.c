/**
 * @file slb-cp-ofdm-symbol.c
 * @brief 6.2.2.1 表 1 / 表 2 / 表 3 查表与校验
 */
#include "slb-cp-ofdm-symbol.h"

#include "slb-phy-constants.h"

static const SlbSymbolTimingParams SLB_SYMBOL_TIMING_TABLE[] = {
    /* TYPE_1A: CP=18, sym=274, GAP1=20, boundary=276 */
    {18U, 274U, 20U, 276U, true},
    /* TYPE_1B: GAP1=22, boundary=278 */
    {18U, 274U, 22U, 278U, true},
    /* TYPE_2 */
    {39U, 295U, 44U, 300U, true},
    /* TYPE_3: 无单独边界/GAP2 */
    {64U, 320U, 0U, 320U, false},
    /* TYPE_4 */
    {128U, 384U, 0U, 384U, false}
};

static const uint8_t SLB_SYMBOL_COUNT_TABLE[] = {14U, 14U, 13U, 12U, 10U};

static const uint8_t SLB_NCP_INDEX_TABLE[] = {3U, 3U, 2U, 1U, 0U};

bool slbSelectSymbolTimingParams(SlbCpOfdmSymbolType symbolType,
                                 SlbSymbolTimingParams *timingParams)
{
    if (timingParams == NULL || symbolType > SLB_CP_OFDM_SYMBOL_TYPE_4) {
        return false;
    }
    *timingParams = SLB_SYMBOL_TIMING_TABLE[symbolType];
    return true;
}

uint8_t slbCalculateSymbolCountPerRadioFrame(SlbCpOfdmSymbolType symbolType)
{
    if (symbolType > SLB_CP_OFDM_SYMBOL_TYPE_4) {
        return 0U;
    }
    return SLB_SYMBOL_COUNT_TABLE[symbolType];
}

bool slbValidateIsBoundarySymbol(SlbCpOfdmSymbolType symbolType, uint8_t symbolIndex)
{
    switch (symbolType) {
    case SLB_CP_OFDM_SYMBOL_TYPE_1A:
        return (symbolIndex == 0U || symbolIndex == 7U);
    case SLB_CP_OFDM_SYMBOL_TYPE_1B:
    case SLB_CP_OFDM_SYMBOL_TYPE_2:
        return (symbolIndex == 0U);
    case SLB_CP_OFDM_SYMBOL_TYPE_3:
    case SLB_CP_OFDM_SYMBOL_TYPE_4:
        return false;
    default:
        return false;
    }
}

uint8_t slbSelectNcPIndex(SlbCpOfdmSymbolType symbolType)
{
    if (symbolType > SLB_CP_OFDM_SYMBOL_TYPE_4) {
        return 0U;
    }
    return SLB_NCP_INDEX_TABLE[symbolType];
}
