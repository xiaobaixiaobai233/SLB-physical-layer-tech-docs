/**
 * @file slb-pas.c
 * airMode = slb
 */
#include "slb-pas.h"

SlbPasSubcarrierIndices slbCalculatePasSubcarrierIndices(
    uint16_t minGroupStartSubcarrierIndex,
    uint16_t minGroupEndSubcarrierIndex,
    uint16_t maxGroupStartSubcarrierIndex,
    uint16_t maxGroupEndSubcarrierIndex)
{
    SlbPasSubcarrierIndices indices;
    indices.k0SubcarrierIndex = minGroupEndSubcarrierIndex;
    indices.k1SubcarrierIndex = maxGroupStartSubcarrierIndex;
    (void)minGroupStartSubcarrierIndex;
    return indices;
}

void slbGeneratePasSequence(const SlbPasConfig *config,
                            SlbSubcarrierBuffer pasSequence)
{
    slbGenerateRnLSequence(&config->rnLContext, pasSequence);
}

void slbMapPasToResourceGrid(const SlbPasConfig *config,
                             const SlbSubcarrierBuffer pasSequence,
                             SlbSubcarrierBuffer port0SymbolBuffer)
{
    slbClearSubcarrierBuffer(port0SymbolBuffer);

    if (config->skipDmrsDRadioFrame) {
        return;
    }

    const uint16_t k0 = config->subcarrierIndices.k0SubcarrierIndex;
    const uint16_t k1 = config->subcarrierIndices.k1SubcarrierIndex;

    if (k0 < SLB_SUBCARRIER_COUNT && k0 != SLB_DC_SUBCARRIER_INDEX) {
        port0SymbolBuffer[k0] = pasSequence[k0];
    }
    if (k1 < SLB_SUBCARRIER_COUNT && k1 != SLB_DC_SUBCARRIER_INDEX) {
        port0SymbolBuffer[k1] = pasSequence[k1];
    }
}
