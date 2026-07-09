/**
 * @file demo-6-2-3-1-sync.c
 * @brief Verify 6.2.3.1 FTS / STS (T/XS 10001-2025)
 */
#include <stdio.h>
#include <string.h>
#include "slb-phy-signal.h"
#include "demo-test-common.h"
#include "demo-ref-formula.h"

static void verifyFtsRootIndex(uint16_t rootIndexU, const char *label)
{
    SlbSubcarrierBuffer implSequence;
    SlbSubcarrierBuffer refSequence;
    SlbSubcarrierBuffer lutSequence;
    SlbFtsConfig config = {
        .rootIndexU = rootIndexU,
        .useLookupTable = false
    };
    SlbFtsSymbolPair symbolPair;
    char testName[128];

    slbGenerateFtsSequence(&config, implSequence);
    refGenerateFtsSequence(rootIndexU, refSequence);

    snprintf(testName, sizeof(testName), "FTS u=%u matches formula (%s)",
             (unsigned)rootIndexU, label);
    demoAssertTrue(demoMaxComplexError(implSequence, refSequence) <= DEMO_PHASE_TOL,
                   testName);

    demoAssertNear(implSequence[SLB_DC_SUBCARRIER_INDEX].re, 0.0f,
                   DEMO_MAG_TOL, "FTS DC subcarrier re=0");
    demoAssertNear(implSequence[SLB_DC_SUBCARRIER_INDEX].im, 0.0f,
                   DEMO_MAG_TOL, "FTS DC subcarrier im=0");

    const uint16_t spotK[] = { 0u, 1u, 79u, 81u, 160u };
    for (uint8_t i = 0; i < 5u; i++) {
        uint16_t n = spotK[i];
        snprintf(testName, sizeof(testName), "FTS u=%u k=%u unit magnitude |d|~1",
                 (unsigned)rootIndexU, (unsigned)n);
        demoAssertNear(demoComplexMagnitude(&implSequence[n]), 1.0f,
                       DEMO_MAG_TOL, testName);
    }

    config.useLookupTable = true;
    slbGenerateFtsSequence(&config, lutSequence);
    snprintf(testName, sizeof(testName), "FTS u=%u LUT matches runtime", (unsigned)rootIndexU);
    demoAssertTrue(memcmp(lutSequence, implSequence, sizeof(SlbSubcarrierBuffer)) == 0,
                   testName);

    slbMapFtsToSymbolPair(implSequence, &symbolPair);
    demoAssertTrue(memcmp(symbolPair.symbol0, symbolPair.symbol1,
                          sizeof(SlbSubcarrierBuffer)) == 0,
                   "FTS dual-symbol mapping identical");
}

static void verifyStsRootIndex(uint8_t rootIndexU)
{
    SlbSubcarrierBuffer implSequence;
    SlbSubcarrierBuffer refSequence;
    SlbSubcarrierBuffer lutSequence;
    SlbStsConfig config = {
        .rootIndexU = rootIndexU,
        .useLookupTable = false
    };
    char testName[128];

    slbGenerateStsSequence(&config, implSequence);
    refGenerateStsSequence(rootIndexU, refSequence);

    snprintf(testName, sizeof(testName), "STS u=%u matches formula", (unsigned)rootIndexU);
    demoAssertTrue(demoMaxComplexError(implSequence, refSequence) <= DEMO_PHASE_TOL,
                   testName);

    demoAssertNear(demoComplexMagnitude(&implSequence[0]), SLB_STS_POWER_SCALE,
                   DEMO_MAG_TOL, "STS k=0 magnitude 2*sqrt(2)");
    demoAssertNear(demoComplexMagnitude(&implSequence[16]), SLB_STS_POWER_SCALE,
                   DEMO_MAG_TOL, "STS k=16 magnitude 2*sqrt(2)");
    demoAssertNear(implSequence[SLB_DC_SUBCARRIER_INDEX].re, 0.0f,
                   DEMO_MAG_TOL, "STS DC is zero");
    demoAssertNear(implSequence[4].re, 0.0f, DEMO_MAG_TOL, "STS off-comb k=4 is zero");

    config.useLookupTable = true;
    slbGenerateStsSequence(&config, lutSequence);
    snprintf(testName, sizeof(testName), "STS u=%u LUT matches runtime", (unsigned)rootIndexU);
    demoAssertTrue(memcmp(lutSequence, implSequence, sizeof(SlbSubcarrierBuffer)) == 0,
                   testName);
}

int main(void)
{
    demoInitConsole();
    gDemoPassCount = 0;
    gDemoFailCount = 0;

    printf("=== Demo 6.2.3.1 FTS / STS ===\n");
    slbInitPhySignalModule();

    demoPrintSection("FTS root index u (4 scenarios)");
    verifyFtsRootIndex(1u,   "comm SAB type 1A/1B/4");
    verifyFtsRootIndex(160u, "comm SAB type 2/3");
    verifyFtsRootIndex(2u,   "comm RAB");
    verifyFtsRootIndex(159u, "passthrough SAB");

    demoPrintSection("STS node id u=1,8,16");
    verifyStsRootIndex(1u);
    verifyStsRootIndex(8u);
    verifyStsRootIndex(16u);

    demoAssertTrue(slbCalculateStsRootIndexFromNodeId(5u) == 6u,
                   "STS u = nodeIdLow4 + 1 (ID=5 -> u=6)");

    return demoPrintSummary("6.2.3.1 FTS/STS");
}
