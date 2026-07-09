/**
 * @file demo-6-2-3-8-pas.c
 * @brief Verify 6.2.3.8 PAS
 */
#include <stdio.h>
#include "slb-phy-signal.h"
#include "demo-test-common.h"
#include "demo-ref-formula.h"

int main(void)
{
    demoInitConsole();
    gDemoPassCount = 0;
    gDemoFailCount = 0;

    printf("=== Demo 6.2.3.8 PAS ===\n");
    slbInitPhySignalModule();

    SlbRnLContext context = {
        .radioFrameIndex = 4u,
        .symbolIndex = 0u,
        .nId = 7u,
        .nCp = 3u
    };

    SlbPasSubcarrierIndices indices = slbCalculatePasSubcarrierIndices(
        20u, 40u,
        100u, 140u
    );

    demoPrintSection("k0 / k1 calculation");
    demoAssertTrue(indices.k0SubcarrierIndex == 40u,
                   "k0 = max SC in min group = 40");
    demoAssertTrue(indices.k1SubcarrierIndex == 100u,
                   "k1 = min SC in max group = 100");

    SlbPasConfig pasConfig = {
        .rnLContext = context,
        .subcarrierIndices = indices,
        .skipDmrsDRadioFrame = false
    };
    SlbSubcarrierBuffer pasSequence;
    SlbSubcarrierBuffer mapped;
    SlbSubcarrierBuffer refRnL;

    slbGeneratePasSequence(&pasConfig, pasSequence);
    refGenerateRnLSequence(&context, refRnL);
    slbMapPasToResourceGrid(&pasConfig, pasSequence, mapped);

    demoPrintSection("PAS RE mapping");
    demoAssertComplexNear(&mapped[40], &refRnL[40], DEMO_QPSK_TOL,
                          "PAS k0=40 maps r_{n,l}(40)");
    demoAssertComplexNear(&mapped[100], &refRnL[100], DEMO_QPSK_TOL,
                          "PAS k1=100 maps r_{n,l}(100)");
    demoAssertNear(mapped[41].re, 0.0f, DEMO_MAG_TOL,
                   "PAS non-k0/k1 subcarrier is zero");
    demoAssertNear(mapped[SLB_DC_SUBCARRIER_INDEX].re, 0.0f, DEMO_MAG_TOL,
                   "PAS DC subcarrier is zero");

    demoAssertTrue(demoCountNonZeroRe(mapped) == 2,
                   "PAS has exactly 2 non-zero REs");

    pasConfig.skipDmrsDRadioFrame = true;
    slbMapPasToResourceGrid(&pasConfig, pasSequence, mapped);
    demoAssertTrue(demoCountNonZeroRe(mapped) == 0,
                   "PAS skipped on DMRS-D radio frame");

    return demoPrintSummary("6.2.3.8 PAS");
}
