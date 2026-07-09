/**
 * @file demo-6225-resource-grid.c
 * @brief Demo：6.2.2.5~6.2.2.7 天线端口、资源网格、RE 与 a_{80,l}≡0
 *
 * 标准依据：T/XS 10001-2025 §6.2.2.5 / §6.2.2.6 / §6.2.2.7
 */
#include "slb-test.h"

#include "slb-antenna-port.h"
#include "slb-cp-ofdm-symbol.h"
#include "slb-phy-constants.h"
#include "slb-resource-element.h"
#include "slb-resource-grid.h"

unsigned int slbRunDemo6225ResourceGrid(void)
{
    SlbComplex gridBuffer[SLB_SUBCARRIER_COUNT_PER_BASE_CARRIER * 14U];
    SlbResourceGrid resourceGrid;
    SlbResourceElementIndex reIndex;
    SlbComplex payload = {1.0f, -1.0f};
    SlbComplex readBack;
    SlbCommDomainAntennaPortConfig unifiedPort = {2U, 2U, 2U, 2U};
    SlbCommDomainAntennaPortConfig splitPort = {0U, 1U, 2U, 3U};

    SLB_TEST_SUITE("Demo 6.2.2.5~6.2.2.7 Antenna Port / Resource Grid / RE");
    SLB_TEST_BEGIN();

    SLB_TEST_CASE("Comm domain SAB/G-PCIB/broadcast/G-DMRS-C same antenna port");
    SLB_TEST_TRUE(slbValidateCommDomainUnifiedPort(&unifiedPort));
    SLB_TEST_FALSE(slbValidateCommDomainUnifiedPort(&splitPort));

    SLB_TEST_CASE("Resource grid size 161 x 14 for type 1A");
    slbInitResourceGrid(&resourceGrid, gridBuffer,
                        SLB_CP_OFDM_SYMBOL_TYPE_1A, 0U, 0U);
    SLB_TEST_EQ_U16(resourceGrid.subcarrierCount, 161U);
    SLB_TEST_EQ_U8(resourceGrid.symbolCount, 14U);

    SLB_TEST_CASE("Map RE at (k=10, l=3) and read back");
    reIndex.subcarrierIndex = 10U;
    reIndex.symbolIndex = 3U;
    reIndex.antennaPortIndex = 0U;
    SLB_TEST_TRUE(slbMapResourceElementValue(resourceGrid.values,
                                             resourceGrid.subcarrierCount,
                                             resourceGrid.symbolCount,
                                             &reIndex, payload));
    readBack = slbReadResourceElement(&resourceGrid, &reIndex);
    SLB_TEST_TRUE(readBack.real == 1.0f && readBack.imag == -1.0f);

    SLB_TEST_CASE("Reject mapping to DC subcarrier k=80 (a_{80,l} must be 0)");
    reIndex.subcarrierIndex = SLB_DC_SUBCARRIER_INDEX;
    SLB_TEST_FALSE(slbMapResourceElementValue(resourceGrid.values,
                                              resourceGrid.subcarrierCount,
                                              resourceGrid.symbolCount,
                                              &reIndex, payload));

    SLB_TEST_CASE("DC column forced to zero after slbEnforceDcSubcarrierZero");
    {
        size_t flatIndex;
        flatIndex = (size_t)5U * (size_t)SLB_SUBCARRIER_COUNT_PER_BASE_CARRIER +
                    (size_t)SLB_DC_SUBCARRIER_INDEX;
        gridBuffer[flatIndex].real = 99.0f;
        gridBuffer[flatIndex].imag = 99.0f;
        slbEnforceDcSubcarrierZero(gridBuffer, 14U);
        readBack = gridBuffer[flatIndex];
        SLB_TEST_TRUE(readBack.real == 0.0f && readBack.imag == 0.0f);
    }

    SLB_TEST_CASE("Validate RE index rejects out-of-range l");
    reIndex.subcarrierIndex = 10U;
    reIndex.symbolIndex = 14U;
    SLB_TEST_FALSE(slbValidateResourceElementIndex(&reIndex, 14U));

    SLB_TEST_END();
    return slbTestFailCount(&slbTestStats);
}
