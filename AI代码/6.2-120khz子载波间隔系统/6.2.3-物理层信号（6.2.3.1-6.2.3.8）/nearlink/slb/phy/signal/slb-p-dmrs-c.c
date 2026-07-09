/**
 * @file slb-p-dmrs-c.c
 * airMode = slb
 */
#include "slb-p-dmrs-c.h"

#include <string.h>

void slbGeneratePDmrsCSequence(const SlbPDmrsCConfig *config,
                              SlbSubcarrierBuffer pDmrsCSequence)
{
    slbGenerateRnLSequence(&config->rnLContext, pDmrsCSequence);
}

void slbMapPDmrsCToResourceGrid(const SlbPDmrsCConfig *config,
                                const SlbSubcarrierBuffer pDmrsCSequence,
                                SlbSubcarrierBuffer symbolBuffer)
{
    if (!config->hasSabOnCarrier) {
        slbClearSubcarrierBuffer(symbolBuffer);
        return;
    }
    memcpy(symbolBuffer, pDmrsCSequence, sizeof(SlbSubcarrierBuffer));
}
