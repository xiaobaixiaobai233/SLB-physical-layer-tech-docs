/**
 * @file slb-ref-signal-common.c
 * airMode = slb
 */
#include "slb-ref-signal-common.h"

bool slbIsSubcarrierInCombSet(uint16_t subcarrierIndex,
                              const SlbSubcarrierGroupConfig *groupConfig,
                              const SlbCombSubcarrierConfig *combConfig)
{
    if (subcarrierIndex == SLB_DC_SUBCARRIER_INDEX) {
        return false;
    }

    if (groupConfig != NULL) {
        if (subcarrierIndex < groupConfig->startSubcarrierIndex ||
            subcarrierIndex > groupConfig->endSubcarrierIndex) {
            return false;
        }
    }

    if (combConfig == NULL || combConfig->combSpacing <= 1u) {
        return true;
    }

    return (uint8_t)((subcarrierIndex - combConfig->combOffset) %
                     combConfig->combSpacing) == 0u;
}

void slbMapRnLToAntennaPorts(const SlbSubcarrierBuffer rnLSequence,
                             const SlbSubcarrierGroupConfig *groupConfig,
                             const SlbCombSubcarrierConfig *combConfig,
                             const SlbAntennaPortConfig *antennaPortConfig,
                             SlbComplex antennaPortBuffers[][SLB_SUBCARRIER_COUNT])
{
    const uint8_t portCount = antennaPortConfig->antennaPortCount;
    const uint8_t activePort  = antennaPortConfig->activeAntennaPort;

    for (uint8_t antennaPort = 0; antennaPort < portCount; antennaPort++) {
        slbClearSubcarrierBuffer(antennaPortBuffers[antennaPort]);
    }

    for (uint16_t subcarrierIndex = 0;
         subcarrierIndex < SLB_SUBCARRIER_COUNT;
         subcarrierIndex++) {
        if (!slbIsSubcarrierInCombSet(subcarrierIndex, groupConfig, combConfig)) {
            continue;
        }
        antennaPortBuffers[activePort][subcarrierIndex] = rnLSequence[subcarrierIndex];
    }
}
