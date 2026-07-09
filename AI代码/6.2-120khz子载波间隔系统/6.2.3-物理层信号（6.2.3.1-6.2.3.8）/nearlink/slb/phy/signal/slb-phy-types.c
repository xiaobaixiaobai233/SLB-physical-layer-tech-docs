/**
 * @file slb-phy-types.c
 * airMode = slb
 */
#include "slb-phy-types.h"

void slbClearSubcarrierBuffer(SlbSubcarrierBuffer buffer)
{
    for (uint16_t subcarrierIndex = 0;
         subcarrierIndex < SLB_SUBCARRIER_COUNT;
         subcarrierIndex++) {
        buffer[subcarrierIndex].re = 0.0f;
        buffer[subcarrierIndex].im = 0.0f;
    }
}

void slbZeroDcSubcarrier(SlbSubcarrierBuffer buffer)
{
    buffer[SLB_DC_SUBCARRIER_INDEX].re = 0.0f;
    buffer[SLB_DC_SUBCARRIER_INDEX].im = 0.0f;
}
