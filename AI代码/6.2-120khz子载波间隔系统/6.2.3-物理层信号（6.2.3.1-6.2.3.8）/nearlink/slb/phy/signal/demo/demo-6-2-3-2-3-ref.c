/**
 * @file demo-6-2-3-2-3-ref.c
 * @brief Verify 6.2.3.2 SRS / 6.2.3.3 CSI-RS
 */
#include <stdio.h>
#include <string.h>
#include "slb-phy-signal.h"
#include "demo-test-common.h"
#include "demo-ref-formula.h"

static bool isValidQpsk(const SlbComplex *value)
{
    const float s = SLB_QPSK_NORM_SCALE;
    const float candidates[4][2] = {
        { s,  s}, { s, -s}, {-s,  s}, {-s, -s}
    };
    for (int i = 0; i < 4; i++) {
        if (fabsf(value->re - candidates[i][0]) <= DEMO_QPSK_TOL &&
            fabsf(value->im - candidates[i][1]) <= DEMO_QPSK_TOL) {
            return true;
        }
    }
    return false;
}

static void verifyGoldAndRnL(const SlbRnLContext *context, const char *label)
{
    uint8_t implGold[322];
    uint8_t refGold[322];
    SlbSubcarrierBuffer implRnL;
    SlbSubcarrierBuffer refRnL;
    char testName[128];

    uint32_t implCInit = slbCalculateCInitFromContext(context);
    uint32_t refCInit  = refCalculateCInit(context->radioFrameIndex,
                                           context->symbolIndex,
                                           context->nId,
                                           context->nCp);

    snprintf(testName, sizeof(testName), "c_init matches ref (%s)", label);
    demoAssertTrue(implCInit == refCInit, testName);

    slbGenerateGoldSequenceBits(implCInit, 322u, implGold);
    refGenerateGoldSequenceBits(refCInit, 322u, refGold);
    demoAssertTrue(memcmp(implGold, refGold, 322u) == 0,
                   "Gold c(i) bits match reference");

    slbGenerateRnLSequence(context, implRnL);
    refGenerateRnLSequence(context, refRnL);

    snprintf(testName, sizeof(testName), "r_{n,l}(k) matches ref (%s)", label);
    demoAssertTrue(demoMaxComplexError(implRnL, refRnL) <= DEMO_QPSK_TOL, testName);

    const uint16_t spotK[] = { 0u, 10u, 79u, 81u, 160u };
    for (uint8_t i = 0; i < 5u; i++) {
        uint16_t k = spotK[i];
        snprintf(testName, sizeof(testName), "r_{n,l}(%u) valid QPSK (%s)",
                 (unsigned)k, label);
        demoAssertTrue(isValidQpsk(&implRnL[k]), testName);
    }

    demoAssertNear(implRnL[SLB_DC_SUBCARRIER_INDEX].re, 0.0f,
                   DEMO_MAG_TOL, "r_{n,l}(80) re=0");
    demoAssertNear(implRnL[SLB_DC_SUBCARRIER_INDEX].im, 0.0f,
                   DEMO_MAG_TOL, "r_{n,l}(80) im=0");
}

static void verifySrsMapping(void)
{
    SlbRnLContext context = {
        .radioFrameIndex = 2u,
        .symbolIndex = 5u,
        .nId = 100u,
        .nCp = 3u
    };
    SlbSrsConfig srsConfig = {
        .rnLContext = context,
        .subcarrierGroup = { .startSubcarrierIndex = 16u, .endSubcarrierIndex = 144u },
        .combConfig = { .combSpacing = 4u, .combOffset = 0u },
        .antennaPortConfig = { .activeAntennaPort = 0u, .antennaPortCount = 2u },
        .hasSubcarrierGroup = true
    };
    SlbSubcarrierBuffer srsSequence;
    SlbComplex portBuffers[2][SLB_SUBCARRIER_COUNT];
    int activeReCount = 0;

    slbGenerateSrsSequence(&srsConfig, srsSequence);
    slbMapSrsToResourceGrid(&srsConfig, srsSequence, portBuffers);

    for (uint16_t k = 16u; k <= 144u; k++) {
        if (k == SLB_DC_SUBCARRIER_INDEX || (k % 4u) != 0u) {
            continue;
        }
        demoAssertComplexNear(&portBuffers[0][k], &srsSequence[k],
                              DEMO_QPSK_TOL, "SRS port0 maps r_{n,l}(k)");
        activeReCount++;
    }
    demoAssertTrue(activeReCount > 0, "SRS comb has active REs");
    demoAssertNear(portBuffers[0][17].re, 0.0f, DEMO_MAG_TOL, "SRS off-comb k=17 is zero");
    demoAssertNear(portBuffers[1][32].re, 0.0f, DEMO_MAG_TOL, "SRS inactive port is zero");
}

static void verifyCsiRsMapping(void)
{
    SlbRnLContext context = {
        .radioFrameIndex = 0u,
        .symbolIndex = 1u,
        .nId = 42u,
        .nCp = 2u
    };
    SlbCsiRsConfig csiConfig = {
        .rnLContext = context,
        .combConfig = { .combSpacing = 2u, .combOffset = 1u },
        .antennaPortConfig = { .activeAntennaPort = 0u, .antennaPortCount = 1u },
        .startSymbolRule = SLB_CSI_RS_START_AFTER_G_PCIB,
        .hasSubcarrierGroup = false
    };
    SlbSubcarrierBuffer csiSequence;
    SlbComplex portBuffers[1][SLB_SUBCARRIER_COUNT];

    demoAssertTrue(
        slbSelectCsiRsStartSymbolRule(true, false) == SLB_CSI_RS_START_AFTER_G_PCIB,
        "CSI-RS start rule: carrier with SAB");
    demoAssertTrue(
        slbSelectCsiRsStartSymbolRule(false, true) == SLB_CSI_RS_START_AFTER_STS,
        "CSI-RS start rule: no SAB, STS on #0");

    slbGenerateCsiRsSequence(&csiConfig, csiSequence);
    slbMapCsiRsToResourceGrid(&csiConfig, csiSequence, portBuffers);

    demoAssertComplexNear(&portBuffers[0][1], &csiSequence[1],
                          DEMO_QPSK_TOL, "CSI-RS odd comb k=1");
    demoAssertNear(portBuffers[0][0].re, 0.0f, DEMO_MAG_TOL,
                   "CSI-RS even comb k=0 is zero");
}

int main(void)
{
    demoInitConsole();
    gDemoPassCount = 0;
    gDemoFailCount = 0;

    printf("=== Demo 6.2.3.2 SRS / 6.2.3.3 CSI-RS ===\n");
    slbInitPhySignalModule();

    demoPrintSection("Gold / r_{n,l} (6.5.2 / 6.5.3)");
    SlbRnLContext ctxA = { 0, 0, 0, 3 };
    verifyGoldAndRnL(&ctxA, "n=0,l=0,N_ID=0,N_CP=3");

    SlbRnLContext ctxB = { 3, 7, 255, 1 };
    verifyGoldAndRnL(&ctxB, "n=3,l=7,N_ID=255,N_CP=1");

    demoAssertTrue(slbCalculateCInit(0, 0, 0, 3) == 16387u,
                   "c_init n=0,l=0,N_ID=0,N_CP=3 -> 16387");

    demoPrintSection("SRS RE mapping (T-link, srs-Set-Conf comb)");
    verifySrsMapping();

    demoPrintSection("CSI-RS RE mapping (G-link, csi-RS-Set-Conf comb)");
    verifyCsiRsMapping();

    return demoPrintSummary("6.2.3.2 SRS / 6.2.3.3 CSI-RS");
}
