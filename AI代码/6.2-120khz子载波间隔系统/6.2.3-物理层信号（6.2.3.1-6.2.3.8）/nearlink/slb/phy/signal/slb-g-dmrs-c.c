/**
 * @file slb-g-dmrs-c.c
 * airMode = slb
 */
#include "slb-g-dmrs-c.h"

#include <string.h>

uint8_t slbSelectGDmrsCSymbolIndex(bool hasSabOnCarrier)
{
    return hasSabOnCarrier ? (uint8_t)SLB_G_DMRS_C_SYMBOL_6
                         : (uint8_t)SLB_G_DMRS_C_SYMBOL_0;
}

void slbGenerateGDmrsCSequence(const SlbGDmrsCConfig *config,
                               SlbSubcarrierBuffer gDmrsCSequence)
{
    slbGenerateRnLSequence(&config->rnLContext, gDmrsCSequence);
}

void slbMapGDmrsCToResourceGrid(const SlbGDmrsCConfig *config,
                                const SlbSubcarrierBuffer gDmrsCSequence,
                                SlbSubcarrierBuffer symbolBuffer)
{
    if (!config->hasSabOnCarrier) {
        slbClearSubcarrierBuffer(symbolBuffer);
        return;
    }
    memcpy(symbolBuffer, gDmrsCSequence, sizeof(SlbSubcarrierBuffer));
}
