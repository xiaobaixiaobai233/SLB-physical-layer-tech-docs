/**
 * @file slb-t-dmrs-c.c
 * airMode = slb
 */
#include "slb-t-dmrs-c.h"

void slbGenerateTDmrsCSequence(const SlbTDmrsCConfig *config,
                               SlbSubcarrierBuffer tDmrsCSequence)
{
    slbGenerateRnLSequence(&config->rnLContext, tDmrsCSequence);
}

void slbMapTDmrsCToResourceGrid(const SlbTDmrsCConfig *config,
                                const SlbSubcarrierBuffer tDmrsCSequence,
                                SlbComplex antennaPortBuffers[][SLB_SUBCARRIER_COUNT])
{
    const SlbSubcarrierGroupConfig *groupConfig =
        config->hasSubcarrierGroup ? &config->subcarrierGroup : NULL;

    slbMapRnLToAntennaPorts(tDmrsCSequence,
                            groupConfig,
                            &config->combConfig,
                            &config->antennaPortConfig,
                            antennaPortBuffers);
}
