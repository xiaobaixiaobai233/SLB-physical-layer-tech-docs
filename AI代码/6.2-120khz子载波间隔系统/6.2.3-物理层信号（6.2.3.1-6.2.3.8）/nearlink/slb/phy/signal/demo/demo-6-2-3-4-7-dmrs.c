/**
 * @file demo-6-2-3-4-7-dmrs.c
 * @brief Verify 6.2.3.4~6.2.3.7 G/P/T-DMRS-C and DMRS-D
 */
#include <stdio.h>
#include <string.h>
#include "slb-phy-signal.h"
#include "demo-test-common.h"
#include "demo-ref-formula.h"

static const SlbRnLContext gDemoContext = {
    .radioFrameIndex = 1u,
    .symbolIndex = 6u,
    .nId = 10u,
    .nCp = 3u
};

static void verifyGDmrsC(void)
{
    SlbGDmrsCConfig config = {
        .rnLContext = gDemoContext,
        .hasSabOnCarrier = true
    };
    SlbSubcarrierBuffer sequence;
    SlbSubcarrierBuffer mapped;
    SlbSubcarrierBuffer refRnL;

    slbGenerateGDmrsCSequence(&config, sequence);
    refGenerateRnLSequence(&gDemoContext, refRnL);

    demoAssertTrue(slbSelectGDmrsCSymbolIndex(true) == 6u,
                   "G-DMRS-C with SAB -> symbol #6");
    demoAssertTrue(slbSelectGDmrsCSymbolIndex(false) == 0u,
                   "G-DMRS-C without SAB TTI -> symbol #0");

    slbMapGDmrsCToResourceGrid(&config, sequence, mapped);
    demoAssertTrue(memcmp(mapped, sequence, sizeof(SlbSubcarrierBuffer)) == 0,
                   "G-DMRS-C maps on SAB carrier");

    config.hasSabOnCarrier = false;
    slbMapGDmrsCToResourceGrid(&config, sequence, mapped);
    demoAssertTrue(demoCountNonZeroRe(mapped) == 0,
                   "G-DMRS-C no map on carrier without SAB");

    demoAssertComplexNear(&sequence[10], &refRnL[10], DEMO_QPSK_TOL,
                          "G-DMRS-C sequence equals r_{n,l}(k)");
}

static void verifyPDmrsC(void)
{
    SlbPDmrsCConfig config = {
        .rnLContext = gDemoContext,
        .hasSabOnCarrier = true
    };
    SlbSubcarrierBuffer sequence;
    SlbSubcarrierBuffer mapped;

    slbGeneratePDmrsCSequence(&config, sequence);
    slbMapPDmrsCToResourceGrid(&config, sequence, mapped);
    demoAssertTrue(memcmp(mapped, sequence, sizeof(SlbSubcarrierBuffer)) == 0,
                   "P-DMRS-C maps on SAB carrier symbol #6");

    config.hasSabOnCarrier = false;
    slbMapPDmrsCToResourceGrid(&config, sequence, mapped);
    demoAssertTrue(demoCountNonZeroRe(mapped) == 0,
                   "P-DMRS-C no map without SAB carrier");
}

static void verifyDmrsD(void)
{
    SlbDmrsDConfig config = {
        .rnLContext = gDemoContext,
        .combMode = SLB_DMRS_D_COMB_SPARSE,
        .antennaPortCount = 4u,
        .dmrsSymbolCount = 2u,
        .hasSubcarrierGroup = false
    };
    SlbSubcarrierBuffer sequence;
    SlbComplex symbolBuffers[4][SLB_SUBCARRIER_COUNT];

    slbGenerateDmrsDSequence(&config, sequence);

    demoAssertTrue(slbCalculateDmrsDSymbolCount(4u, SLB_DMRS_D_COMB_SPARSE) == 2u,
                   "DMRS-D N = N_port/N_comb = 4/2 = 2");

    slbMapDmrsDToResourceGrid(&config, 0u, sequence, symbolBuffers);
    demoAssertComplexNear(&symbolBuffers[0][2], &sequence[2],
                          DEMO_QPSK_TOL, "DMRS-D M=0 port0 even subcarrier");
    demoAssertNear(symbolBuffers[1][2].re, 0.0f, DEMO_MAG_TOL,
                   "DMRS-D M=0 port1 not transmitted");

    slbMapDmrsDToResourceGrid(&config, 1u, sequence, symbolBuffers);
    demoAssertComplexNear(&symbolBuffers[3][3], &sequence[3],
                          DEMO_QPSK_TOL, "DMRS-D M=1 port3 odd subcarrier");
    demoAssertNear(symbolBuffers[2][3].re, 0.0f, DEMO_MAG_TOL,
                   "DMRS-D M=1 port2 no odd subcarrier");

    demoAssertTrue(slbIsDmrsDSubcarrierForPort(4u, 0u, SLB_DMRS_D_COMB_SPARSE),
                   "comb even port -> even subcarrier");
    demoAssertTrue(!slbIsDmrsDSubcarrierForPort(5u, 0u, SLB_DMRS_D_COMB_SPARSE),
                   "comb even port excludes odd subcarrier");
}

static void verifyTDmrsC(void)
{
    SlbTDmrsCConfig config = {
        .rnLContext = gDemoContext,
        .subcarrierGroup = { .startSubcarrierIndex = 0u, .endSubcarrierIndex = 160u },
        .combConfig = { .combSpacing = 4u, .combOffset = 2u },
        .antennaPortConfig = { .activeAntennaPort = 0u, .antennaPortCount = 1u },
        .hasSubcarrierGroup = true
    };
    SlbSubcarrierBuffer sequence;
    SlbComplex portBuffers[1][SLB_SUBCARRIER_COUNT];

    slbGenerateTDmrsCSequence(&config, sequence);
    slbMapTDmrsCToResourceGrid(&config, sequence, portBuffers);

    demoAssertComplexNear(&portBuffers[0][2], &sequence[2],
                          DEMO_QPSK_TOL, "T-DMRS-C same comb as TCI k=2");
    demoAssertNear(portBuffers[0][0].re, 0.0f, DEMO_MAG_TOL,
                   "T-DMRS-C off-comb k=0 is zero");
}

int main(void)
{
    demoInitConsole();
    gDemoPassCount = 0;
    gDemoFailCount = 0;

    printf("=== Demo 6.2.3.4~6.2.3.7 DMRS ===\n");
    slbInitPhySignalModule();

    demoPrintSection("6.2.3.4 G-DMRS-C");
    verifyGDmrsC();

    demoPrintSection("6.2.3.5 P-DMRS-C");
    verifyPDmrsC();

    demoPrintSection("6.2.3.6 DMRS-D");
    verifyDmrsD();

    demoPrintSection("6.2.3.7 T-DMRS-C");
    verifyTDmrsC();

    return demoPrintSummary("6.2.3.4~6.2.3.7 DMRS");
}
