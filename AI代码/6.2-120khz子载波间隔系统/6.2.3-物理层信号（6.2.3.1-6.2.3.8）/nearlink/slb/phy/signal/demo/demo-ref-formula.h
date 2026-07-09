/**
 * @file demo-ref-formula.h
 * @brief 按 T/XS 10001-2025 标准公式独立实现的参考序列（仅用于验证）
 */
#ifndef DEMO_REF_FORMULA_H
#define DEMO_REF_FORMULA_H

#include "slb-phy-types.h"
#include "slb-gold-seq.h"

void refGenerateFtsSequence(uint16_t rootIndexU, SlbSubcarrierBuffer ftsSequence);
void refGenerateStsZcSequence(uint8_t rootIndexU, SlbSubcarrierBuffer stsZcSequence);
void refGenerateStsSequence(uint8_t rootIndexU, SlbSubcarrierBuffer stsSequence);

uint32_t refCalculateCInit(uint8_t radioFrameIndex,
                             uint8_t symbolIndex,
                             uint16_t nId,
                             uint8_t nCp);

void refGenerateGoldSequenceBits(uint32_t cInit,
                                 uint16_t bitCount,
                                 uint8_t *goldBits);

void refGenerateRnLSequence(const SlbRnLContext *context,
                            SlbSubcarrierBuffer rnLSequence);

#endif /* DEMO_REF_FORMULA_H */
