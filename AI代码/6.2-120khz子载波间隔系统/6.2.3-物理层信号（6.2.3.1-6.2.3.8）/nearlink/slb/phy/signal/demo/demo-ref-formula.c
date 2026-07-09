/**
 * @file demo-ref-formula.c
 * @brief 标准公式独立参考实现（6.2.3.1 / 6.5.2 / 6.5.3）
 */
#include "demo-ref-formula.h"

#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static void refGenerateX1Bits(uint16_t length, uint8_t *x1Out)
{
    uint8_t state[31];
    state[0] = 1;
    for (int i = 1; i < 31; i++) {
        state[i] = 0;
    }
    for (uint16_t i = 0; i < length; i++) {
        x1Out[i] = state[0];
        uint8_t fb = (uint8_t)((state[3] + state[0]) & 1u);
        for (int j = 30; j > 0; j--) {
            state[j] = state[j - 1];
        }
        state[0] = fb;
    }
}

static void refGenerateX2Bits(uint32_t cInit, uint16_t length, uint8_t *x2Out)
{
    uint8_t state[31];
    for (int i = 0; i < 31; i++) {
        state[i] = (uint8_t)((cInit >> i) & 1u);
    }
    for (uint16_t i = 0; i < length; i++) {
        x2Out[i] = state[0];
        uint8_t fb = (uint8_t)((state[3] + state[2] + state[1] + state[0]) & 1u);
        for (int j = 30; j > 0; j--) {
            state[j] = state[j - 1];
        }
        state[0] = fb;
    }
}

void refGenerateFtsSequence(uint16_t rootIndexU, SlbSubcarrierBuffer ftsSequence)
{
    const float phaseScale = -(float)M_PI * (float)rootIndexU / 161.0f;
    for (uint16_t n = 0; n < SLB_SUBCARRIER_COUNT; n++) {
        if (n == SLB_DC_SUBCARRIER_INDEX) {
            ftsSequence[n].re = 0.0f;
            ftsSequence[n].im = 0.0f;
            continue;
        }
        float phaseRad = phaseScale * (float)n * (float)(n + 1u);
        ftsSequence[n].re = cosf(phaseRad);
        ftsSequence[n].im = sinf(phaseRad);
    }
}

void refGenerateStsZcSequence(uint8_t rootIndexU, SlbSubcarrierBuffer stsZcSequence)
{
    slbClearSubcarrierBuffer(stsZcSequence);
    const float phaseScale = -(float)M_PI * (float)rootIndexU / 21.0f;
    for (uint16_t n = 0; n < SLB_SUBCARRIER_COUNT; n++) {
        if (n == SLB_DC_SUBCARRIER_INDEX || (n % 8u) != 0u) {
            continue;
        }
        uint16_t g = n / 8u;
        float phaseRad = phaseScale * (float)g * (float)(g + 1u);
        stsZcSequence[n].re = cosf(phaseRad);
        stsZcSequence[n].im = sinf(phaseRad);
    }
}

void refGenerateStsSequence(uint8_t rootIndexU, SlbSubcarrierBuffer stsSequence)
{
    SlbSubcarrierBuffer stsZcSequence;
    refGenerateStsZcSequence(rootIndexU, stsZcSequence);
    for (uint16_t n = 0; n < SLB_SUBCARRIER_COUNT; n++) {
        stsSequence[n].re = SLB_STS_POWER_SCALE * stsZcSequence[n].re;
        stsSequence[n].im = SLB_STS_POWER_SCALE * stsZcSequence[n].im;
    }
}

uint32_t refCalculateCInit(uint8_t radioFrameIndex,
                           uint8_t symbolIndex,
                           uint16_t nId,
                           uint8_t nCp)
{
    uint32_t termA = (uint32_t)(15u * (radioFrameIndex + 1u) + symbolIndex + 1u);
    uint32_t termB = (uint32_t)(4u * nId + 1u);
    return (uint32_t)(1024u * termA * termB + 4u * nId + nCp);
}

void refGenerateGoldSequenceBits(uint32_t cInit,
                                 uint16_t bitCount,
                                 uint8_t *goldBits)
{
    const uint16_t xLength = (uint16_t)(bitCount + 1600u);
    uint8_t x1Bits[SLB_GOLD_X1_LUT_SIZE];
    uint8_t x2Bits[SLB_GOLD_X1_LUT_SIZE];

    refGenerateX1Bits(xLength, x1Bits);
    refGenerateX2Bits(cInit, xLength, x2Bits);

    for (uint16_t i = 0; i < bitCount; i++) {
        uint16_t idx = (uint16_t)(i + 1600u);
        goldBits[i] = (uint8_t)((x1Bits[idx] + x2Bits[idx]) & 1u);
    }
}

void refGenerateRnLSequence(const SlbRnLContext *context,
                            SlbSubcarrierBuffer rnLSequence)
{
    uint8_t goldBits[322];
    uint32_t cInit = refCalculateCInit(context->radioFrameIndex,
                                       context->symbolIndex,
                                       context->nId,
                                       context->nCp);
    refGenerateGoldSequenceBits(cInit, 322u, goldBits);

    for (uint16_t k = 0; k < SLB_SUBCARRIER_COUNT; k++) {
        if (k == SLB_DC_SUBCARRIER_INDEX) {
            rnLSequence[k].re = 0.0f;
            rnLSequence[k].im = 0.0f;
            continue;
        }
        float iBit = (float)(2 * goldBits[2u * k] - 1);
        float qBit = (float)(2 * goldBits[2u * k + 1u] - 1);
        rnLSequence[k].re = SLB_QPSK_NORM_SCALE * iBit;
        rnLSequence[k].im = SLB_QPSK_NORM_SCALE * qBit;
    }
}
