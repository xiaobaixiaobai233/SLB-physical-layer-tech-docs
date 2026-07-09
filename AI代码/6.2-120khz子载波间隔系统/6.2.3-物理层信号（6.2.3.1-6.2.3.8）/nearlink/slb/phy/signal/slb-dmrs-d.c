/**
 * @file slb-dmrs-d.c
 * airMode = slb
 */
#include "slb-dmrs-d.h"

uint8_t slbCalculateDmrsDSymbolCount(uint8_t antennaPortCount,
                                     SlbDmrsDCombMode combMode)
{
    return (uint8_t)(antennaPortCount / (uint8_t)combMode);
}

bool slbIsDmrsDSubcarrierForPort(uint16_t subcarrierIndex,
                                 uint8_t antennaPort,
                                 SlbDmrsDCombMode combMode)
{
    if (subcarrierIndex == SLB_DC_SUBCARRIER_INDEX) {
        return false;
    }
    if (combMode == SLB_DMRS_D_COMB_CONTINUOUS) {
        return true;
    }
    return (uint8_t)(subcarrierIndex % 2u) == (uint8_t)(antennaPort % 2u);
}

void slbGenerateDmrsDSequence(const SlbDmrsDConfig *config,
                              SlbSubcarrierBuffer dmrsDSequence)
{
    slbGenerateRnLSequence(&config->rnLContext, dmrsDSequence);
}

void slbMapDmrsDToResourceGrid(const SlbDmrsDConfig *config,
                               uint8_t dmrsSymbolIndex,
                               const SlbSubcarrierBuffer dmrsDSequence,
                               SlbComplex symbolBuffers[][SLB_SUBCARRIER_COUNT])
{
    const uint8_t combCount = (uint8_t)config->combMode;
    const uint8_t portStart = (uint8_t)(dmrsSymbolIndex * combCount);
    const uint8_t portEnd   = (uint8_t)(portStart + combCount);

    for (uint8_t antennaPort = 0;
         antennaPort < config->antennaPortCount;
         antennaPort++) {
        slbClearSubcarrierBuffer(symbolBuffers[antennaPort]);
    }

    for (uint16_t subcarrierIndex = 0;
         subcarrierIndex < SLB_SUBCARRIER_COUNT;
         subcarrierIndex++) {
        if (config->hasSubcarrierGroup) {
            if (subcarrierIndex < config->subcarrierGroup.startSubcarrierIndex ||
                subcarrierIndex > config->subcarrierGroup.endSubcarrierIndex) {
                continue;
            }
        }

        for (uint8_t antennaPort = portStart; antennaPort < portEnd; antennaPort++) {
            if (!slbIsDmrsDSubcarrierForPort(subcarrierIndex,
                                               antennaPort,
                                               config->combMode)) {
                continue;
            }
            symbolBuffers[antennaPort][subcarrierIndex] =
                dmrsDSequence[subcarrierIndex];
        }
    }
}
