/**
 * @file slb-gold-seq.c
 * @brief Gold 序列 c(i) 与 r_{n,l}(k) 实现（6.5.2 / 6.5.3）
 * airMode = slb
 */
#include "slb-gold-seq.h"

#include <math.h>
#include <string.h>

static uint8_t slbX1Lut[SLB_GOLD_X1_LUT_SIZE];
static bool    slbGoldTablesReady = false;

static void slbGenerateX1Lut(void)
{
    uint8_t x1State[31];

    x1State[0] = 1;
    for (int i = 1; i < 31; i++) {
        x1State[i] = 0;
    }

    for (uint16_t i = 0; i < SLB_GOLD_X1_LUT_SIZE; i++) {
        slbX1Lut[i] = x1State[0];
        uint8_t feedback = (uint8_t)((x1State[3] + x1State[0]) & 1u);
        for (int j = 30; j > 0; j--) {
            x1State[j] = x1State[j - 1];
        }
        x1State[0] = feedback;
    }
}

static void slbGenerateX2Sequence(uint32_t cInit,
                                  uint16_t length,
                                  uint8_t *x2Out)
{
    uint8_t x2State[31];

    for (int i = 0; i < 31; i++) {
        x2State[i] = (uint8_t)((cInit >> i) & 1u);
    }

    for (uint16_t i = 0; i < length; i++) {
        x2Out[i] = x2State[0];
        uint8_t feedback = (uint8_t)((x2State[3] + x2State[2] + x2State[1] + x2State[0]) & 1u);
        for (int j = 30; j > 0; j--) {
            x2State[j] = x2State[j - 1];
        }
        x2State[0] = feedback;
    }
}

int slbInitGoldSequenceTables(void)
{
    slbGenerateX1Lut();
    slbGoldTablesReady = true;
    return 0;
}

bool slbIsGoldSequenceTablesReady(void)
{
    return slbGoldTablesReady;
}

uint32_t slbCalculateCInit(uint8_t radioFrameIndex,
                           uint8_t symbolIndex,
                           uint16_t nId,
                           uint8_t nCp)
{
    uint32_t termA = (uint32_t)(15u * (radioFrameIndex + 1u) + symbolIndex + 1u);
    uint32_t termB = (uint32_t)(4u * nId + 1u);
    return (uint32_t)(1024u * termA * termB + 4u * nId + nCp);
}

uint32_t slbCalculateCInitFromContext(const SlbRnLContext *context)
{
    return slbCalculateCInit(context->radioFrameIndex,
                           context->symbolIndex,
                           context->nId,
                           context->nCp);
}

void slbGenerateGoldSequenceBits(uint32_t cInit,
                                 uint16_t bitCount,
                                 uint8_t *goldBits)
{
    if (!slbGoldTablesReady) {
        slbInitGoldSequenceTables();
    }

    const uint16_t x2Length = (uint16_t)(bitCount + 1600u);
    uint8_t x2Sequence[SLB_GOLD_X1_LUT_SIZE];

    slbGenerateX2Sequence(cInit, x2Length, x2Sequence);

    for (uint16_t i = 0; i < bitCount; i++) {
        uint16_t offsetIndex = (uint16_t)(i + 1600u);
        goldBits[i] = (uint8_t)((slbX1Lut[offsetIndex] + x2Sequence[offsetIndex]) & 1u);
    }
}

void slbGenerateRnLSequence(const SlbRnLContext *context,
                            SlbSubcarrierBuffer rnLSequence)
{
    const uint16_t goldBitCount = (uint16_t)(2u * SLB_QPSK_SEQUENCE_LENGTH);
    uint8_t goldBits[322];

    uint32_t cInit = slbCalculateCInitFromContext(context);
    slbGenerateGoldSequenceBits(cInit, goldBitCount, goldBits);

    for (uint16_t subcarrierIndex = 0;
         subcarrierIndex < SLB_SUBCARRIER_COUNT;
         subcarrierIndex++) {
        if (subcarrierIndex == SLB_DC_SUBCARRIER_INDEX) {
            rnLSequence[subcarrierIndex].re = 0.0f;
            rnLSequence[subcarrierIndex].im = 0.0f;
            continue;
        }

        uint16_t m = subcarrierIndex;
        float iBit = (float)(2 * goldBits[2u * m] - 1);
        float qBit = (float)(2 * goldBits[2u * m + 1u] - 1);
        rnLSequence[subcarrierIndex].re = SLB_QPSK_NORM_SCALE * iBit;
        rnLSequence[subcarrierIndex].im = SLB_QPSK_NORM_SCALE * qBit;
    }
}
