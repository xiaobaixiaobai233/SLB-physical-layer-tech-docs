/**
 * @file demo-6223-cot-tti.c
 * @brief Demo：6.2.2.3 COT 与 TTI — 2^N_TTI、通信域 TTI 规则、COT 内 TTI 范围
 *
 * 标准依据：T/XS 10001-2025 §6.2.2.3
 */
#include "slb-test.h"

#include "slb-cot-tti.h"
#include "slb-superframe.h"

unsigned int slbRunDemo6223CotTti(void)
{
    SlbChannelOccupancyTime cot;
    uint32_t startOffset;
    uint32_t frameCount;
    /* 合法：G 在开头，末符号 GAP — 对应标准通信域 TTI 规则 */
    const uint8_t validTtiSymbols[] = {
        (uint8_t)SLB_LINK_SYMBOL_G,
        (uint8_t)SLB_LINK_SYMBOL_T,
        (uint8_t)SLB_LINK_SYMBOL_GAP
    };
    const uint8_t invalidTtiNoG[] = {
        (uint8_t)SLB_LINK_SYMBOL_T,
        (uint8_t)SLB_LINK_SYMBOL_GAP
    };
    const uint8_t invalidTtiNoGapEnd[] = {
        (uint8_t)SLB_LINK_SYMBOL_G,
        (uint8_t)SLB_LINK_SYMBOL_T
    };

    SLB_TEST_SUITE("Demo 6.2.2.3 COT / TTI");
    SLB_TEST_BEGIN();

    SLB_TEST_CASE("TTI frame count = 2^N_TTI (N_TTI=0..6)");
    SLB_TEST_EQ_U32(slbCalculateTtiRadioFrameCount(0U), 1U);
    SLB_TEST_EQ_U32(slbCalculateTtiRadioFrameCount(1U), 2U);
    SLB_TEST_EQ_U32(slbCalculateTtiRadioFrameCount(2U), 4U);
    SLB_TEST_EQ_U32(slbCalculateTtiRadioFrameCount(3U), 8U);
    SLB_TEST_EQ_U32(slbCalculateTtiRadioFrameCount(4U), 16U);
    SLB_TEST_EQ_U32(slbCalculateTtiRadioFrameCount(5U), 32U);
    SLB_TEST_EQ_U32(slbCalculateTtiRadioFrameCount(6U), 64U);

    SLB_TEST_CASE("N_TTI=7 out of range");
    SLB_TEST_FALSE(slbValidateTtiExponent(7U));
    SLB_TEST_EQ_U32(slbCalculateTtiRadioFrameCount(7U), 0U);

    SLB_TEST_CASE("Comm domain TTI: gLink at start, last symbol is GAP");
    SLB_TEST_TRUE(slbValidateCommDomainTtiStructure(validTtiSymbols, 3U));

    SLB_TEST_CASE("Reject TTI without gLink at start");
    SLB_TEST_FALSE(slbValidateCommDomainTtiStructure(invalidTtiNoG, 2U));

    SLB_TEST_CASE("Reject TTI whose last symbol is not GAP");
    SLB_TEST_FALSE(slbValidateCommDomainTtiStructure(invalidTtiNoGapEnd, 2U));

    cot.startRadioFrameOffset = 100U;
    cot.radioFrameCount = 16U;
    SLB_TEST_CASE("COT TTI #0 range: N_TTI=2 -> 4 frames at offset 100");
    SLB_TEST_TRUE(slbCalculateTtiRadioFrameRange(&cot, 2U, 0U, &startOffset, &frameCount));
    SLB_TEST_EQ_U32(startOffset, 100U);
    SLB_TEST_EQ_U32(frameCount, 4U);

    SLB_TEST_CASE("COT TTI #1 range: frames 104..107");
    SLB_TEST_TRUE(slbCalculateTtiRadioFrameRange(&cot, 2U, 1U, &startOffset, &frameCount));
    SLB_TEST_EQ_U32(startOffset, 104U);
    SLB_TEST_EQ_U32(frameCount, 4U);

    SLB_TEST_CASE("TTI #4 exceeds COT (only 4 TTIs fit in 16 frames)");
    SLB_TEST_FALSE(slbCalculateTtiRadioFrameRange(&cot, 2U, 4U, &startOffset, &frameCount));

    SLB_TEST_END();
    return slbTestFailCount(&slbTestStats);
}
