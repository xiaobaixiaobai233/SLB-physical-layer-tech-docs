/**
 * @file slb-resource-grid.c
 * @brief 6.2.2.6 资源网格初始化与访问
 */
#include "slb-resource-grid.h"

#include <string.h>

#include "slb-phy-constants.h"
#include "slb-resource-element.h"

void slbInitResourceGrid(SlbResourceGrid *resourceGrid,
                         SlbComplex *valueBuffer,
                         SlbCpOfdmSymbolType symbolType,
                         uint8_t antennaPortIndex,
                         uint8_t baseCarrierIndex)
{
    if (resourceGrid == NULL) {
        return;
    }
    resourceGrid->values = valueBuffer;
    resourceGrid->subcarrierCount = SLB_SUBCARRIER_COUNT_PER_BASE_CARRIER;
    resourceGrid->symbolCount = slbCalculateSymbolCountPerRadioFrame(symbolType);
    resourceGrid->antennaPortIndex = antennaPortIndex;
    resourceGrid->baseCarrierIndex = baseCarrierIndex;
    slbClearResourceGrid(resourceGrid);
}

void slbClearResourceGrid(SlbResourceGrid *resourceGrid)
{
    size_t elementCount;

    if (resourceGrid == NULL || resourceGrid->values == NULL) {
        return;
    }
    elementCount = (size_t)resourceGrid->subcarrierCount * (size_t)resourceGrid->symbolCount;
    memset(resourceGrid->values, 0, elementCount * sizeof(SlbComplex));
    slbEnforceDcSubcarrierZero(resourceGrid->values, resourceGrid->symbolCount);
}

SlbComplex slbReadResourceElement(const SlbResourceGrid *resourceGrid,
                                  const SlbResourceElementIndex *reIndex)
{
    SlbComplex zero = {0.0f, 0.0f};
    size_t flatIndex;

    if (resourceGrid == NULL || reIndex == NULL || resourceGrid->values == NULL) {
        return zero;
    }
    if (!slbValidateResourceElementIndex(reIndex, resourceGrid->symbolCount)) {
        return zero;
    }
    flatIndex = ((size_t)reIndex->symbolIndex * (size_t)resourceGrid->subcarrierCount) +
                (size_t)reIndex->subcarrierIndex;
    return resourceGrid->values[flatIndex];
}
