/**
 * @file slb-zc-seq.c
 * airMode = slb
 */
#include "slb-zc-seq.h"

#include <math.h>
#include <string.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static SlbComplex slbFtsLut[SLB_FTS_ROOT_INDEX_COUNT][SLB_SUBCARRIER_COUNT];
static SlbComplex slbStsLut[SLB_STS_ROOT_INDEX_MAX][SLB_SUBCARRIER_COUNT];
static bool slbFtsLutReady = false;
static bool slbStsLutReady = false;

static const uint16_t slbFtsRootIndexList[SLB_FTS_ROOT_INDEX_COUNT] = {
    1, 2, 159, 160
};

static uint8_t slbFindFtsLutIndex(uint16_t rootIndexU)
{
    for (uint8_t lutIndex = 0; lutIndex < SLB_FTS_ROOT_INDEX_COUNT; lutIndex++) {
        if (slbFtsRootIndexList[lutIndex] == rootIndexU) {
            return lutIndex;
        }
    }
    return 0xFFu;
}

void slbGenerateZcSequence(uint16_t sequenceLengthN,
                           uint16_t rootIndexU,
                           uint16_t dcSubcarrierIndex,
                           SlbComplex *sequenceOut)
{
    const float phaseScale = -(float)M_PI * (float)rootIndexU / (float)sequenceLengthN;

    for (uint16_t n = 0; n < sequenceLengthN; n++) {
        if (n == dcSubcarrierIndex) {
            sequenceOut[n].re = 0.0f;
            sequenceOut[n].im = 0.0f;
            continue;
        }
        float phaseRad = phaseScale * (float)n * (float)(n + 1u);
        sequenceOut[n].re = cosf(phaseRad);
        sequenceOut[n].im = sinf(phaseRad);
    }
}

void slbGenerateFtsZcSequence(uint16_t rootIndexU,
                            SlbSubcarrierBuffer ftsSequence)
{
    slbGenerateZcSequence(SLB_FTS_ZC_LENGTH,
                          rootIndexU,
                          SLB_DC_SUBCARRIER_INDEX,
                          ftsSequence);
}

void slbGenerateStsZcSequence(uint8_t rootIndexU,
                                SlbSubcarrierBuffer stsSequence)
{
    slbClearSubcarrierBuffer(stsSequence);

    const float phaseScale = -(float)M_PI * (float)rootIndexU / (float)SLB_STS_ZC_LENGTH;

    for (uint16_t subcarrierIndex = 0;
         subcarrierIndex < SLB_SUBCARRIER_COUNT;
         subcarrierIndex++) {
        if (subcarrierIndex == SLB_DC_SUBCARRIER_INDEX) {
            continue;
        }
        if ((subcarrierIndex % SLB_STS_COMB_SPACING) != 0u) {
            continue;
        }

        uint16_t combGroupIndex = subcarrierIndex / SLB_STS_COMB_SPACING;
        float phaseRad = phaseScale * (float)combGroupIndex * (float)(combGroupIndex + 1u);
        stsSequence[subcarrierIndex].re = cosf(phaseRad);
        stsSequence[subcarrierIndex].im = sinf(phaseRad);
    }
}

int slbInitFtsLookupTables(void)
{
    for (uint8_t lutIndex = 0; lutIndex < SLB_FTS_ROOT_INDEX_COUNT; lutIndex++) {
        slbGenerateFtsZcSequence(slbFtsRootIndexList[lutIndex], slbFtsLut[lutIndex]);
    }
    slbFtsLutReady = true;
    return 0;
}

int slbInitStsLookupTables(void)
{
    for (uint8_t rootIndexU = SLB_STS_ROOT_INDEX_MIN;
         rootIndexU <= SLB_STS_ROOT_INDEX_MAX;
         rootIndexU++) {
        SlbSubcarrierBuffer stsZcSequence;
        slbGenerateStsZcSequence(rootIndexU, stsZcSequence);

        for (uint16_t subcarrierIndex = 0;
             subcarrierIndex < SLB_SUBCARRIER_COUNT;
             subcarrierIndex++) {
            slbStsLut[rootIndexU - 1u][subcarrierIndex].re =
                SLB_STS_POWER_SCALE * stsZcSequence[subcarrierIndex].re;
            slbStsLut[rootIndexU - 1u][subcarrierIndex].im =
                SLB_STS_POWER_SCALE * stsZcSequence[subcarrierIndex].im;
        }
    }
    slbStsLutReady = true;
    return 0;
}

const SlbComplex *slbSelectFtsFromLookupTable(uint16_t rootIndexU)
{
    if (!slbFtsLutReady) {
        slbInitFtsLookupTables();
    }
    uint8_t lutIndex = slbFindFtsLutIndex(rootIndexU);
    if (lutIndex == 0xFFu) {
        return NULL;
    }
    return slbFtsLut[lutIndex];
}

const SlbComplex *slbSelectStsFromLookupTable(uint8_t rootIndexU)
{
    if (rootIndexU < SLB_STS_ROOT_INDEX_MIN || rootIndexU > SLB_STS_ROOT_INDEX_MAX) {
        return NULL;
    }
    if (!slbStsLutReady) {
        slbInitStsLookupTables();
    }
    return slbStsLut[rootIndexU - 1u];
}
