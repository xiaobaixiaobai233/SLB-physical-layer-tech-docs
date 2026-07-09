/**
 * @file slb-phy-example.c
 * @brief SLB 6.2.2 基础帧结构与物理资源 API 演示
 *
 * airMode = slb
 */
#include <stdio.h>

#include "slb-antenna-port.h"
#include "slb-carrier.h"
#include "slb-comm-domain.h"
#include "slb-cot-tti.h"
#include "slb-cp-ofdm-symbol.h"
#include "slb-link.h"
#include "slb-passthrough-link.h"
#include "slb-phy-constants.h"
#include "slb-resource-grid.h"
#include "slb-resource-element.h"
#include "slb-rx-resource-locate.h"
#include "slb-superframe.h"

int main(void)
{
    SlbSymbolTimingParams timingParams;
    SlbNodeWorkingChannelConfig channelConfig = {8U, 8U};
    SlbCommDomainAntennaPortConfig portConfig = {0U, 0U, 0U, 0U};
    SlbComplex gridBuffer[SLB_SUBCARRIER_COUNT_PER_BASE_CARRIER * 14U];
    SlbResourceGrid resourceGrid;
    SlbResourceElementIndex reIndex = {10U, 3U, 0U};
    SlbComplex payload = {1.0f, -1.0f};
    uint8_t mergeCount;
    SlbRxResourceLocateContext rxContext = {0};

    printf("=== SLB 6.2.2 Frame Structure Demo ===\n");
    printf("sampleRateHz=%u subcarrierSpacingHz=%u\n",
           SLB_SAMPLE_RATE_HZ, SLB_SUBCARRIER_SPACING_HZ);

    if (slbSelectSymbolTimingParams(SLB_CP_OFDM_SYMBOL_TYPE_1A, &timingParams)) {
        printf("type1A: cpLengthTs=%u symbolLengthTs=%u boundaryLengthTs=%u\n",
               timingParams.cpLengthTs,
               timingParams.symbolLengthTs,
               timingParams.boundarySymbolLengthTs);
    }

    printf("symbolCountPerRadioFrame(type1A)=%u\n",
           slbCalculateSymbolCountPerRadioFrame(SLB_CP_OFDM_SYMBOL_TYPE_1A));
    printf("isBoundarySymbol(type1A,l=7)=%d\n",
           slbValidateIsBoundarySymbol(SLB_CP_OFDM_SYMBOL_TYPE_1A, 7U) ? 1 : 0);
    printf("ttiRadioFrameCount(N_TTI=3)=%u\n", slbCalculateTtiRadioFrameCount(3U));

    if (slbSelectSubcarrierGroupMergeCount(8U, &mergeCount)) {
        printf("N_CH=8 -> L_CH=%u groupIndexMax=%u\n",
               mergeCount,
               slbCalculateWorkingSubcarrierGroupIndexMax(&channelConfig));
    }

    printf("commDomainUnifiedPort=%d\n",
           slbValidateCommDomainUnifiedPort(&portConfig) ? 1 : 0);
    printf("gLinkSymbolKind=%u passthroughDSymbolValid=%d\n",
           (unsigned)slbSelectLinkSymbolKind(SLB_LOGICAL_LINK_G_LINK),
           slbValidatePassthroughSymbolKind(SLB_LINK_SYMBOL_D) ? 1 : 0);

    slbInitResourceGrid(&resourceGrid, gridBuffer,
                        SLB_CP_OFDM_SYMBOL_TYPE_1A, 0U, 0U);
    if (slbMapResourceElementValue(resourceGrid.values,
                                  resourceGrid.subcarrierCount,
                                  resourceGrid.symbolCount,
                                  &reIndex, payload)) {
        SlbComplex readBack = slbReadResourceElement(&resourceGrid, &reIndex);
        printf("mapped RE(k=%u,l=%u) = (%f, %f)\n",
               reIndex.subcarrierIndex, reIndex.symbolIndex,
               readBack.real, readBack.imag);
    }

    if (slbRxLocateStep1SelectSymbolCount(SLB_CP_OFDM_SYMBOL_TYPE_1A, &rxContext) ==
        SLB_RX_LOCATE_STEP_OK) {
        printf("rxStep1 symbolCount=%u\n", rxContext.symbolCountPerRadioFrame);
    }

    printf("superframeDurationTs=%u\n", slbCalculateSuperframeDurationTs());
    return 0;
}
