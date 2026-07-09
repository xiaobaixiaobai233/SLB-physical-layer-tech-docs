/**
 * @file slb-fts.c
 * airMode = slb
 */
#include "slb-fts.h"

#include <string.h>

uint16_t slbSelectFtsRootIndex(SlbFtsScenario scenario)
{
    switch (scenario) {
    case SLB_FTS_SCENARIO_COMM_SAB_TYPE_AB4: return SLB_FTS_ROOT_COMM_SAB_TYPE_AB4;
    case SLB_FTS_SCENARIO_COMM_SAB_TYPE_23: return SLB_FTS_ROOT_COMM_SAB_TYPE_23;
    case SLB_FTS_SCENARIO_RAB:              return SLB_FTS_ROOT_RAB;
    case SLB_FTS_SCENARIO_PASSTHROUGH_SAB:  return SLB_FTS_ROOT_PASSTHROUGH_SAB;
    default:                                return SLB_FTS_ROOT_COMM_SAB_TYPE_AB4;
    }
}

int slbGenerateFtsSequence(const SlbFtsConfig *config,
                           SlbSubcarrierBuffer ftsSequence)
{
    uint16_t rootIndexU = config->rootIndexU;
    if (rootIndexU == 0u) {
        rootIndexU = slbSelectFtsRootIndex(config->scenario);
    }

    if (config->useLookupTable) {
        const SlbComplex *lutEntry = slbSelectFtsFromLookupTable(rootIndexU);
        if (lutEntry == NULL) {
            return -1;
        }
        memcpy(ftsSequence, lutEntry, sizeof(SlbSubcarrierBuffer));
        return 0;
    }

    slbGenerateFtsZcSequence(rootIndexU, ftsSequence);
    return 0;
}

void slbMapFtsToSymbolPair(const SlbSubcarrierBuffer ftsSequence,
                           SlbFtsSymbolPair *symbolPair)
{
    memcpy(symbolPair->symbol0, ftsSequence, sizeof(SlbSubcarrierBuffer));
    memcpy(symbolPair->symbol1, ftsSequence, sizeof(SlbSubcarrierBuffer));
}
