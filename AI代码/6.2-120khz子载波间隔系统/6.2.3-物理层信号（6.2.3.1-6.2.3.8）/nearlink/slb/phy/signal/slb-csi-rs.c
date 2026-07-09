/**
 * @file slb-csi-rs.c
 * airMode = slb
 */
#include "slb-csi-rs.h"

SlbCsiRsStartSymbolRule slbSelectCsiRsStartSymbolRule(bool hasSabOnCarrier,
                                                      bool isStsOnTtiFirstSymbol)
{
    if (hasSabOnCarrier) {
        return SLB_CSI_RS_START_AFTER_G_PCIB;
    }
    if (isStsOnTtiFirstSymbol) {
        return SLB_CSI_RS_START_AFTER_STS;
    }
    return SLB_CSI_RS_START_TTI_FIRST;
}

void slbGenerateCsiRsSequence(const SlbCsiRsConfig *config,
                              SlbSubcarrierBuffer csiRsSequence)
{
    slbGenerateRnLSequence(&config->rnLContext, csiRsSequence);
}

void slbMapCsiRsToResourceGrid(const SlbCsiRsConfig *config,
                               const SlbSubcarrierBuffer csiRsSequence,
                               SlbComplex antennaPortBuffers[][SLB_SUBCARRIER_COUNT])
{
    const SlbSubcarrierGroupConfig *groupConfig =
        config->hasSubcarrierGroup ? &config->subcarrierGroup : NULL;

    slbMapRnLToAntennaPorts(csiRsSequence,
                            groupConfig,
                            &config->combConfig,
                            &config->antennaPortConfig,
                            antennaPortBuffers);
}
