/**
 * @file slb-resource-element.c
 * @brief 6.2.2.7 RE 索引校验与 a_{k,l} 映射
 */
#include "slb-resource-element.h"

#include "slb-carrier.h"
#include "slb-phy-constants.h"

static size_t slbCalculateResourceElementFlatIndex(uint16_t subcarrierCount,
                                                   uint8_t symbolCount,
                                                   uint16_t subcarrierIndex,
                                                   uint8_t symbolIndex)
{
    return ((size_t)symbolIndex * (size_t)subcarrierCount) + (size_t)subcarrierIndex;
}

bool slbValidateResourceElementIndex(const SlbResourceElementIndex *reIndex,
                                     uint8_t symbolCountPerRadioFrame)
{
    if (reIndex == NULL || symbolCountPerRadioFrame == 0U) {
        return false;
    }
    if (reIndex->subcarrierIndex >= SLB_SUBCARRIER_COUNT_PER_BASE_CARRIER) {
        return false;
    }
    if (reIndex->symbolIndex >= symbolCountPerRadioFrame) {
        return false;
    }
    return slbValidateMappableSubcarrierIndex(reIndex->subcarrierIndex);
}

void slbEnforceDcSubcarrierZero(SlbComplex *gridValues,
                                uint8_t symbolCountPerRadioFrame)
{
    size_t flatIndex;
    uint8_t symbolIndex;

    if (gridValues == NULL) {
        return;
    }
    for (symbolIndex = 0U; symbolIndex < symbolCountPerRadioFrame; ++symbolIndex) {
        flatIndex = slbCalculateResourceElementFlatIndex(
            SLB_SUBCARRIER_COUNT_PER_BASE_CARRIER,
            symbolCountPerRadioFrame,
            SLB_DC_SUBCARRIER_INDEX,
            symbolIndex);
        gridValues[flatIndex].real = 0.0f;
        gridValues[flatIndex].imag = 0.0f;
    }
}

bool slbMapResourceElementValue(SlbComplex *gridValues,
                                uint16_t subcarrierCount,
                                uint8_t symbolCountPerRadioFrame,
                                const SlbResourceElementIndex *reIndex,
                                SlbComplex value)
{
    size_t flatIndex;

    if (gridValues == NULL || reIndex == NULL) {
        return false;
    }
    if (!slbValidateResourceElementIndex(reIndex, symbolCountPerRadioFrame)) {
        return false;
    }
    flatIndex = slbCalculateResourceElementFlatIndex(
        subcarrierCount,
        symbolCountPerRadioFrame,
        reIndex->subcarrierIndex,
        reIndex->symbolIndex);
    gridValues[flatIndex] = value;
    return true;
}
