/**
 * @file slb-sts.c
 * airMode = slb
 */
#include "slb-sts.h"

#include <string.h>

uint8_t slbCalculateStsRootIndexFromNodeId(uint8_t nodeIdLow4Bits)
{
    return (uint8_t)((nodeIdLow4Bits & 0x0Fu) + 1u);
}

int slbGenerateStsSequence(const SlbStsConfig *config,
                           SlbSubcarrierBuffer stsSequence)
{
    if (config->rootIndexU < SLB_STS_ROOT_INDEX_MIN ||
        config->rootIndexU > SLB_STS_ROOT_INDEX_MAX) {
        return -1;
    }

    if (config->useLookupTable) {
        const SlbComplex *lutEntry = slbSelectStsFromLookupTable(config->rootIndexU);
        if (lutEntry == NULL) {
            return -1;
        }
        memcpy(stsSequence, lutEntry, sizeof(SlbSubcarrierBuffer));
        return 0;
    }

    SlbSubcarrierBuffer stsZcSequence;
    slbGenerateStsZcSequence(config->rootIndexU, stsZcSequence);
    for (uint16_t subcarrierIndex = 0;
         subcarrierIndex < SLB_SUBCARRIER_COUNT;
         subcarrierIndex++) {
        stsSequence[subcarrierIndex].re =
            SLB_STS_POWER_SCALE * stsZcSequence[subcarrierIndex].re;
        stsSequence[subcarrierIndex].im =
            SLB_STS_POWER_SCALE * stsZcSequence[subcarrierIndex].im;
    }
    return 0;
}

void slbMapStsToSymbol(const SlbSubcarrierBuffer stsSequence,
                       SlbSubcarrierBuffer symbolBuffer)
{
    memcpy(symbolBuffer, stsSequence, sizeof(SlbSubcarrierBuffer));
}
