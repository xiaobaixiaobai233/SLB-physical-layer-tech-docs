/**
 * @file slb-srs.c
 * airMode = slb
 */
#include "slb-srs.h"

void slbGenerateSrsSequence(const SlbSrsConfig *config,
                            SlbSubcarrierBuffer srsSequence)
{
    slbGenerateRnLSequence(&config->rnLContext, srsSequence);
}

void slbMapSrsToResourceGrid(const SlbSrsConfig *config,
                             const SlbSubcarrierBuffer srsSequence,
                             SlbComplex antennaPortBuffers[][SLB_SUBCARRIER_COUNT])
{
    const SlbSubcarrierGroupConfig *groupConfig =
        config->hasSubcarrierGroup ? &config->subcarrierGroup : NULL;

    slbMapRnLToAntennaPorts(srsSequence,
                            groupConfig,
                            &config->combConfig,
                            &config->antennaPortConfig,
                            antennaPortBuffers);
}
