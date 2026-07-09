/**
 * @file demo-6221-cp-ofdm-symbol.c
 * @brief Demo：6.2.2.1 波形和符号 — 表 1 / 表 2 / 表 3 / 6.5.3 N_CP
 *
 * 标准依据：T/XS 10001-2025 §6.2.2.1
 */
#include "slb-test.h"

#include "slb-cp-ofdm-symbol.h"
#include "slb-phy-constants.h"

unsigned int slbRunDemo6221CpOfdmSymbol(void)
{
    SlbSymbolTimingParams timingParams;

    SLB_TEST_SUITE("Demo 6.2.2.1 CP-OFDM Symbol (Table 1/2/3)");
    SLB_TEST_BEGIN();

    /* 基本量：Δf = fs / 256 = 120 kHz */
    SLB_TEST_CASE("Basic quantity: subcarrier spacing");
    SLB_TEST_EQ_U32(SLB_SAMPLE_RATE_HZ / SLB_FFT_SIZE, SLB_SUBCARRIER_SPACING_HZ);

    /* 表 1：五种符号类型 CP / 符号长度 / GAP1 / 边界符号长 */
    SLB_TEST_CASE("Table 1: Type 1A timing (18/274/20/276 Ts)");
    SLB_TEST_TRUE(slbSelectSymbolTimingParams(SLB_CP_OFDM_SYMBOL_TYPE_1A, &timingParams));
    SLB_TEST_EQ_U16(timingParams.cpLengthTs, 18U);
    SLB_TEST_EQ_U16(timingParams.symbolLengthTs, 274U);
    SLB_TEST_EQ_U16(timingParams.gap1LengthTs, 20U);
    SLB_TEST_EQ_U16(timingParams.boundarySymbolLengthTs, 276U);
    SLB_TEST_TRUE(timingParams.hasBoundarySymbol);

    SLB_TEST_CASE("Table 1: Type 1B GAP1=22, boundary=278");
    SLB_TEST_TRUE(slbSelectSymbolTimingParams(SLB_CP_OFDM_SYMBOL_TYPE_1B, &timingParams));
    SLB_TEST_EQ_U16(timingParams.gap1LengthTs, 22U);
    SLB_TEST_EQ_U16(timingParams.boundarySymbolLengthTs, 278U);

    SLB_TEST_CASE("Table 1: Type 2 (39/295/44/300 Ts)");
    SLB_TEST_TRUE(slbSelectSymbolTimingParams(SLB_CP_OFDM_SYMBOL_TYPE_2, &timingParams));
    SLB_TEST_EQ_U16(timingParams.cpLengthTs, 39U);
    SLB_TEST_EQ_U16(timingParams.symbolLengthTs, 295U);
    SLB_TEST_EQ_U16(timingParams.gap1LengthTs, 44U);
    SLB_TEST_EQ_U16(timingParams.boundarySymbolLengthTs, 300U);

    SLB_TEST_CASE("Table 1: Type 3/4 no separate boundary symbol");
    SLB_TEST_TRUE(slbSelectSymbolTimingParams(SLB_CP_OFDM_SYMBOL_TYPE_3, &timingParams));
    SLB_TEST_FALSE(timingParams.hasBoundarySymbol);
    SLB_TEST_EQ_U16(timingParams.cpLengthTs, 64U);
    SLB_TEST_EQ_U16(timingParams.symbolLengthTs, 320U);
    SLB_TEST_TRUE(slbSelectSymbolTimingParams(SLB_CP_OFDM_SYMBOL_TYPE_4, &timingParams));
    SLB_TEST_EQ_U16(timingParams.cpLengthTs, 128U);
    SLB_TEST_EQ_U16(timingParams.symbolLengthTs, 384U);

    /* 表 2：每无线帧符号数 */
    SLB_TEST_CASE("Table 2: N_sym^frame per symbol type");
    SLB_TEST_EQ_U8(slbCalculateSymbolCountPerRadioFrame(SLB_CP_OFDM_SYMBOL_TYPE_1A), 14U);
    SLB_TEST_EQ_U8(slbCalculateSymbolCountPerRadioFrame(SLB_CP_OFDM_SYMBOL_TYPE_1B), 14U);
    SLB_TEST_EQ_U8(slbCalculateSymbolCountPerRadioFrame(SLB_CP_OFDM_SYMBOL_TYPE_2), 13U);
    SLB_TEST_EQ_U8(slbCalculateSymbolCountPerRadioFrame(SLB_CP_OFDM_SYMBOL_TYPE_3), 12U);
    SLB_TEST_EQ_U8(slbCalculateSymbolCountPerRadioFrame(SLB_CP_OFDM_SYMBOL_TYPE_4), 10U);

    /* 表 3：边界 CP 符号位置 */
    SLB_TEST_CASE("Table 3: Type 1A boundary at #0 and #7");
    SLB_TEST_TRUE(slbValidateIsBoundarySymbol(SLB_CP_OFDM_SYMBOL_TYPE_1A, 0U));
    SLB_TEST_TRUE(slbValidateIsBoundarySymbol(SLB_CP_OFDM_SYMBOL_TYPE_1A, 7U));
    SLB_TEST_FALSE(slbValidateIsBoundarySymbol(SLB_CP_OFDM_SYMBOL_TYPE_1A, 1U));
    SLB_TEST_FALSE(slbValidateIsBoundarySymbol(SLB_CP_OFDM_SYMBOL_TYPE_1A, 6U));

    SLB_TEST_CASE("Table 3: Type 1B/2 boundary only at #0");
    SLB_TEST_TRUE(slbValidateIsBoundarySymbol(SLB_CP_OFDM_SYMBOL_TYPE_1B, 0U));
    SLB_TEST_FALSE(slbValidateIsBoundarySymbol(SLB_CP_OFDM_SYMBOL_TYPE_1B, 7U));
    SLB_TEST_TRUE(slbValidateIsBoundarySymbol(SLB_CP_OFDM_SYMBOL_TYPE_2, 0U));

    SLB_TEST_CASE("Table 3: Type 3/4 no boundary symbol");
    SLB_TEST_FALSE(slbValidateIsBoundarySymbol(SLB_CP_OFDM_SYMBOL_TYPE_3, 0U));
    SLB_TEST_FALSE(slbValidateIsBoundarySymbol(SLB_CP_OFDM_SYMBOL_TYPE_4, 0U));

    /* 6.5.3 衔接：N_CP 索引 */
    SLB_TEST_CASE("6.5.3 mapping: symbol type -> N_CP index");
    SLB_TEST_EQ_U8(slbSelectNcPIndex(SLB_CP_OFDM_SYMBOL_TYPE_1A), 3U);
    SLB_TEST_EQ_U8(slbSelectNcPIndex(SLB_CP_OFDM_SYMBOL_TYPE_1B), 3U);
    SLB_TEST_EQ_U8(slbSelectNcPIndex(SLB_CP_OFDM_SYMBOL_TYPE_2), 2U);
    SLB_TEST_EQ_U8(slbSelectNcPIndex(SLB_CP_OFDM_SYMBOL_TYPE_3), 1U);
    SLB_TEST_EQ_U8(slbSelectNcPIndex(SLB_CP_OFDM_SYMBOL_TYPE_4), 0U);

    SLB_TEST_END();
    return slbTestFailCount(&slbTestStats);
}
