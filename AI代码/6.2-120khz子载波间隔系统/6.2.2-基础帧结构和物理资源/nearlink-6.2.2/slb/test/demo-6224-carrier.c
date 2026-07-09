/**
 * @file demo-6224-carrier.c
 * @brief Demo：6.2.2.4 信道与载波 — 表 4、DC #80、基础子载波组
 *
 * 标准依据：T/XS 10001-2025 §6.2.2.4
 */
#include "slb-test.h"

#include "slb-carrier.h"
#include "slb-phy-constants.h"

unsigned int slbRunDemo6224Carrier(void)
{
    SlbNodeWorkingChannelConfig channelConfig;
    uint8_t mergeCount;

    SLB_TEST_SUITE("Demo 6.2.2.4 Carrier / Table 4");
    SLB_TEST_BEGIN();

    SLB_TEST_CASE("161 subcarriers per base carrier, DC at k=80");
    SLB_TEST_EQ_U32(SLB_SUBCARRIER_COUNT_PER_BASE_CARRIER, 161U);
    SLB_TEST_EQ_U32(SLB_DC_SUBCARRIER_INDEX, 80U);

    SLB_TEST_CASE("DC subcarrier k=80 is not mappable");
    SLB_TEST_FALSE(slbValidateMappableSubcarrierIndex(80U));
    SLB_TEST_TRUE(slbValidateMappableSubcarrierIndex(79U));
    SLB_TEST_TRUE(slbValidateMappableSubcarrierIndex(81U));
    SLB_TEST_FALSE(slbValidateMappableSubcarrierIndex(161U));

    /* 表 4 全 18 档（标准正文表格） */
    SLB_TEST_CASE("Table 4: N_CH -> L_CH pairing (all 18 entries)");
    SLB_TEST_TRUE(slbSelectSubcarrierGroupMergeCount(1U, &mergeCount));
    SLB_TEST_EQ_U8(mergeCount, 1U);
    SLB_TEST_TRUE(slbSelectSubcarrierGroupMergeCount(2U, &mergeCount));
    SLB_TEST_EQ_U8(mergeCount, 2U);
    SLB_TEST_TRUE(slbSelectSubcarrierGroupMergeCount(3U, &mergeCount));
    SLB_TEST_EQ_U8(mergeCount, 4U);
    SLB_TEST_TRUE(slbSelectSubcarrierGroupMergeCount(4U, &mergeCount));
    SLB_TEST_EQ_U8(mergeCount, 4U);
    SLB_TEST_TRUE(slbSelectSubcarrierGroupMergeCount(8U, &mergeCount));
    SLB_TEST_EQ_U8(mergeCount, 8U);
    SLB_TEST_TRUE(slbSelectSubcarrierGroupMergeCount(16U, &mergeCount));
    SLB_TEST_EQ_U8(mergeCount, 16U);
    SLB_TEST_TRUE(slbSelectSubcarrierGroupMergeCount(25U, &mergeCount));
    SLB_TEST_EQ_U8(mergeCount, 32U);
    SLB_TEST_TRUE(slbSelectSubcarrierGroupMergeCount(32U, &mergeCount));
    SLB_TEST_EQ_U8(mergeCount, 32U);

    SLB_TEST_CASE("Table 4: invalid N_CH=17 not in table");
    SLB_TEST_FALSE(slbSelectSubcarrierGroupMergeCount(17U, &mergeCount));
    SLB_TEST_FALSE(slbValidateNodeChannelPairing(8U, 4U));

    SLB_TEST_CASE("Working subcarrier group index max: N_CH=8, L_CH=8 -> 15");
    channelConfig.nodeChannelCount = 8U;
    channelConfig.subcarrierGroupMergeCount = 8U;
    SLB_TEST_EQ_U16(slbCalculateWorkingSubcarrierGroupIndexMax(&channelConfig), 15U);

    SLB_TEST_CASE("Working subcarrier group index max: N_CH=25, L_CH=32 -> 11");
    channelConfig.nodeChannelCount = 25U;
    channelConfig.subcarrierGroupMergeCount = 32U;
    SLB_TEST_EQ_U16(slbCalculateWorkingSubcarrierGroupIndexMax(&channelConfig), 11U);

    SLB_TEST_CASE("Base subcarrier group mapping (skip DC #80)");
    {
        uint8_t groupIndex;
        SLB_TEST_TRUE(slbCalculateBaseSubcarrierGroupIndex(0U, &groupIndex));
        SLB_TEST_EQ_U8(groupIndex, 0U);
        SLB_TEST_TRUE(slbCalculateBaseSubcarrierGroupIndex(9U, &groupIndex));
        SLB_TEST_EQ_U8(groupIndex, 0U);
        SLB_TEST_TRUE(slbCalculateBaseSubcarrierGroupIndex(10U, &groupIndex));
        SLB_TEST_EQ_U8(groupIndex, 1U);
        SLB_TEST_TRUE(slbCalculateBaseSubcarrierGroupIndex(79U, &groupIndex));
        SLB_TEST_EQ_U8(groupIndex, 7U);
        SLB_TEST_TRUE(slbCalculateBaseSubcarrierGroupIndex(81U, &groupIndex));
        SLB_TEST_EQ_U8(groupIndex, 8U);
        SLB_TEST_FALSE(slbCalculateBaseSubcarrierGroupIndex(80U, &groupIndex));
    }

    SLB_TEST_END();
    return slbTestFailCount(&slbTestStats);
}
