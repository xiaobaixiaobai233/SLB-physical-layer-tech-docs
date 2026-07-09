/**
 * @file demo-6222-superframe.c
 * @brief Demo：6.2.2.2 超帧与无线帧 — Tf=1ms, 8 无线帧, 三级索引
 *
 * 标准依据：T/XS 10001-2025 §6.2.2.2
 */
#include "slb-test.h"

#include "slb-cp-ofdm-symbol.h"
#include "slb-phy-constants.h"
#include "slb-superframe.h"

unsigned int slbRunDemo6222Superframe(void)
{
    SlbSuperframeIndex validIndex;
    SlbSuperframeIndex invalidIndex;

    SLB_TEST_SUITE("Demo 6.2.2.2 Superframe / Radio Frame");
    SLB_TEST_BEGIN();

    SLB_TEST_CASE("Superframe duration Tf = 30720 Ts = 1 ms");
    SLB_TEST_EQ_U32(slbCalculateSuperframeDurationTs(), SLB_SUPERFRAME_DURATION_TS);

    SLB_TEST_CASE("8 radio frames per superframe constant");
    SLB_TEST_EQ_U32(SLB_RADIO_FRAME_COUNT_PER_SUPERFRAME, 8U);

    validIndex.superframeIndex = 0U;
    validIndex.radioFrameIndex = 7U;
    validIndex.symbolIndex = 13U;
    SLB_TEST_CASE("Valid index at superframe boundary (type 1A, l=13)");
    SLB_TEST_TRUE(slbValidateSuperframeIndex(&validIndex, SLB_CP_OFDM_SYMBOL_TYPE_1A));

    validIndex.symbolIndex = 0U;
    SLB_TEST_CASE("Valid index type 4 with l=0 (10 symbols/frame)");
    SLB_TEST_TRUE(slbValidateSuperframeIndex(&validIndex, SLB_CP_OFDM_SYMBOL_TYPE_4));

    invalidIndex = validIndex;
    invalidIndex.radioFrameIndex = 8U;
    SLB_TEST_CASE("Invalid radio frame index #8");
    SLB_TEST_FALSE(slbValidateSuperframeIndex(&invalidIndex, SLB_CP_OFDM_SYMBOL_TYPE_1A));

    invalidIndex = validIndex;
    invalidIndex.superframeIndex = SLB_SUPERFRAME_INDEX_MAX;
    SLB_TEST_CASE("Valid superframe index at max #65535");
    SLB_TEST_TRUE(slbValidateSuperframeIndex(&invalidIndex, SLB_CP_OFDM_SYMBOL_TYPE_1A));

    invalidIndex.symbolIndex = 14U;
    SLB_TEST_CASE("Invalid symbol index l=14 for type 1A (max 13)");
    SLB_TEST_FALSE(slbValidateSuperframeIndex(&invalidIndex, SLB_CP_OFDM_SYMBOL_TYPE_1A));

    SLB_TEST_CASE("Link symbol kinds: G/T/A for comm domain, D for passthrough");
    SLB_TEST_EQ_U8((unsigned int)SLB_LINK_SYMBOL_G, 0U);
    SLB_TEST_EQ_U8((unsigned int)SLB_LINK_SYMBOL_T, 1U);
    SLB_TEST_EQ_U8((unsigned int)SLB_LINK_SYMBOL_A, 2U);
    SLB_TEST_EQ_U8((unsigned int)SLB_LINK_SYMBOL_D, 3U);

    SLB_TEST_END();
    return slbTestFailCount(&slbTestStats);
}
