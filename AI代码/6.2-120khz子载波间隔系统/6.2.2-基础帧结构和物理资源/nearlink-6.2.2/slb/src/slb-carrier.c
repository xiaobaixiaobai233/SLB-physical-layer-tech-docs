/**
 * @file slb-carrier.c
 * @brief 6.2.2.4 表 4 与频域子载波组计算
 */
#include "slb-carrier.h"

#include "slb-phy-constants.h"

static const uint8_t SLB_NODE_CHANNEL_COUNT_TABLE[SLB_NODE_CHANNEL_COUNT_TABLE_SIZE] = {
    1U, 2U, 3U, 4U, 5U, 6U, 7U, 8U, 9U, 10U, 11U, 12U, 13U, 14U, 15U, 16U, 25U, 32U
};

static const uint8_t SLB_SUBCARRIER_GROUP_MERGE_TABLE[SLB_NODE_CHANNEL_COUNT_TABLE_SIZE] = {
    1U, 2U, 4U, 4U, 8U, 8U, 8U, 8U, 16U, 16U, 16U, 16U, 16U, 16U, 16U, 16U, 32U, 32U
};

static int slbFindNodeChannelTableIndex(uint8_t nodeChannelCount)
{
    uint8_t i;

    for (i = 0U; i < SLB_NODE_CHANNEL_COUNT_TABLE_SIZE; ++i) {
        if (SLB_NODE_CHANNEL_COUNT_TABLE[i] == nodeChannelCount) {
            return (int)i;
        }
    }
    return -1;
}

bool slbSelectSubcarrierGroupMergeCount(uint8_t nodeChannelCount,
                                        uint8_t *subcarrierGroupMergeCount)
{
    int tableIndex;

    if (subcarrierGroupMergeCount == NULL) {
        return false;
    }
    tableIndex = slbFindNodeChannelTableIndex(nodeChannelCount);
    if (tableIndex < 0) {
        return false;
    }
    *subcarrierGroupMergeCount = SLB_SUBCARRIER_GROUP_MERGE_TABLE[tableIndex];
    return true;
}

bool slbValidateNodeChannelPairing(uint8_t nodeChannelCount,
                                   uint8_t subcarrierGroupMergeCount)
{
    uint8_t expectedMergeCount;

    if (!slbSelectSubcarrierGroupMergeCount(nodeChannelCount, &expectedMergeCount)) {
        return false;
    }
    return (expectedMergeCount == subcarrierGroupMergeCount);
}

uint16_t slbCalculateWorkingSubcarrierGroupIndexMax(
    const SlbNodeWorkingChannelConfig *config)
{
    uint32_t groupCount;

    if (config == NULL || config->subcarrierGroupMergeCount == 0U) {
        return 0U;
    }
    if (!slbValidateNodeChannelPairing(config->nodeChannelCount,
                                       config->subcarrierGroupMergeCount)) {
        return 0U;
    }
    groupCount = (16U * (uint32_t)config->nodeChannelCount) /
                 (uint32_t)config->subcarrierGroupMergeCount;
    if (groupCount == 0U) {
        return 0U;
    }
    return (uint16_t)(groupCount - 1U);
}

bool slbValidateMappableSubcarrierIndex(uint16_t subcarrierIndex)
{
    if (subcarrierIndex >= SLB_SUBCARRIER_COUNT_PER_BASE_CARRIER) {
        return false;
    }
    return (subcarrierIndex != SLB_DC_SUBCARRIER_INDEX);
}

bool slbCalculateBaseSubcarrierGroupIndex(uint16_t subcarrierIndex,
                                          uint8_t *groupIndex)
{
    uint16_t effectiveIndex;

    if (groupIndex == NULL) {
        return false;
    }
    if (subcarrierIndex >= SLB_SUBCARRIER_COUNT_PER_BASE_CARRIER) {
        return false;
    }
    if (subcarrierIndex == SLB_DC_SUBCARRIER_INDEX) {
        return false;
    }
    /* #0~#79 映射 0~7；#81~#160 映射 8~15（每 10 有效子载波一组） */
    if (subcarrierIndex < SLB_DC_SUBCARRIER_INDEX) {
        effectiveIndex = subcarrierIndex;
    } else {
        effectiveIndex = subcarrierIndex - 1U;
    }
    *groupIndex = (uint8_t)(effectiveIndex / SLB_SUBCARRIER_COUNT_PER_GROUP);
    return true;
}
