/**
 * @file demo-rx-resource-locate.c
 * @brief Demo：接收侧资源定位 Step 1~6（技术文档 §处理步骤）
 *
 * 模拟广播中符号类型=1A、N_TTI=2、N_CH=8 的典型接收流程。
 */
#include "slb-test.h"

#include "slb-cot-tti.h"
#include "slb-cp-ofdm-symbol.h"
#include "slb-phy-constants.h"
#include "slb-resource-grid.h"
#include "slb-rx-resource-locate.h"
#include "slb-superframe.h"

unsigned int slbRunDemoRxResourceLocate(void)
{
    SlbRxResourceLocateContext rxContext;
    SlbComplex gridBuffer[SLB_SUBCARRIER_COUNT_PER_BASE_CARRIER * 14U];
    SlbResourceGrid resourceGrid;
    SlbResourceElementIndex reIndex = {10U, 3U, 0U};
    SlbComplex payload = {0.707f, 0.707f};
    SlbChannelOccupancyTime cot = {50U, 16U};
    SlbRadioFrameSymbolConfig commDomainFrame[14U];
    uint8_t i;

    SLB_TEST_SUITE("Demo Rx Resource Locate (Steps 1~6)");
    SLB_TEST_BEGIN();

    memset(&rxContext, 0, sizeof(rxContext));
    rxContext.frameIndex.superframeIndex = 0U;
    rxContext.frameIndex.radioFrameIndex = 0U;
    rxContext.frameIndex.symbolIndex = 0U;

    slbInitResourceGrid(&resourceGrid, gridBuffer,
                        SLB_CP_OFDM_SYMBOL_TYPE_1A, 0U, 0U);
    (void)slbMapResourceElementValue(resourceGrid.values,
                                     resourceGrid.subcarrierCount,
                                     resourceGrid.symbolCount,
                                     &reIndex, payload);
    rxContext.resourceGrid = &resourceGrid;

    for (i = 0U; i < 14U; ++i) {
        commDomainFrame[i].symbolKind = SLB_LINK_SYMBOL_G;
    }
    commDomainFrame[13U].symbolKind = SLB_LINK_SYMBOL_GAP;

    SLB_TEST_CASE("Step 1: select symbol count from type 1A -> 14");
    SLB_TEST_TRUE(slbRxLocateStep1SelectSymbolCount(SLB_CP_OFDM_SYMBOL_TYPE_1A,
                                                    &rxContext) ==
                  SLB_RX_LOCATE_STEP_OK);
    SLB_TEST_EQ_U8(rxContext.symbolCountPerRadioFrame, 14U);

    SLB_TEST_CASE("Step 2: mark boundary symbols #0 and #7 for type 1A");
    SLB_TEST_TRUE(slbRxLocateStep2MarkBoundarySymbols(&rxContext) ==
                  SLB_RX_LOCATE_STEP_OK);

    SLB_TEST_CASE("Step 3: TTI range inside COT (N_TTI=2, TTI#1)");
    SLB_TEST_TRUE(slbRxLocateStep3CalculateTtiRange(&cot, 2U, 1U, &rxContext) ==
                  SLB_RX_LOCATE_STEP_OK);
    SLB_TEST_EQ_U32(rxContext.currentTti.radioFrameCount, 4U);

    SLB_TEST_CASE("Step 4: parse comm domain symbol sequence (no D symbol)");
    SLB_TEST_TRUE(slbRxLocateStep4ParseLinkSymbolSequence(commDomainFrame, 14U,
                                                          &rxContext) ==
                  SLB_RX_LOCATE_STEP_OK);

    SLB_TEST_CASE("Step 4 rejects D symbol in comm domain frame");
    commDomainFrame[5U].symbolKind = SLB_LINK_SYMBOL_D;
    SLB_TEST_TRUE(slbRxLocateStep4ParseLinkSymbolSequence(commDomainFrame, 14U,
                                                          &rxContext) ==
                  SLB_RX_LOCATE_STEP_CONSTRAINT_VIOLATION);
    commDomainFrame[5U].symbolKind = SLB_LINK_SYMBOL_G;

    SLB_TEST_CASE("Step 5: select working carrier N_CH=8, L_CH=8, carrier #2");
    SLB_TEST_TRUE(slbRxLocateStep5SelectWorkingCarrier(8U, 8U, 2U, &rxContext) ==
                  SLB_RX_LOCATE_STEP_OK);
    SLB_TEST_EQ_U8(rxContext.targetBaseCarrierIndex, 2U);

    SLB_TEST_CASE("Step 5 rejects wrong N_CH/L_CH pairing");
    SLB_TEST_TRUE(slbRxLocateStep5SelectWorkingCarrier(8U, 4U, 0U, &rxContext) ==
                  SLB_RX_LOCATE_STEP_CONSTRAINT_VIOLATION);

    SLB_TEST_CASE("Step 6: validate mapped RE (k=10,l=3) has non-zero payload");
    SLB_TEST_TRUE(slbRxLocateStep6ValidateResourceElement(&reIndex, &rxContext) ==
                  SLB_RX_LOCATE_STEP_OK);

    SLB_TEST_CASE("Step 6 rejects idle RE (value=0)");
    {
        SlbResourceElementIndex idleRe = {20U, 4U, 0U};
        SLB_TEST_TRUE(slbRxLocateStep6ValidateResourceElement(&idleRe, &rxContext) ==
                      SLB_RX_LOCATE_STEP_CONSTRAINT_VIOLATION);
    }

    SLB_TEST_CASE("Step 6 rejects DC subcarrier k=80");
    {
        SlbResourceElementIndex dcRe = {80U, 3U, 0U};
        SLB_TEST_TRUE(slbRxLocateStep6ValidateResourceElement(&dcRe, &rxContext) ==
                      SLB_RX_LOCATE_STEP_CONSTRAINT_VIOLATION);
    }

    SLB_TEST_END();
    return slbTestFailCount(&slbTestStats);
}
