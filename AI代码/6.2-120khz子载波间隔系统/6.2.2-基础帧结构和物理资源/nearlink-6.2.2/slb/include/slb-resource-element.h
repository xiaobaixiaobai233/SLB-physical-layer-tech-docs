/**
 * @file slb-resource-element.h
 * @brief 6.2.2.7 资源元素 RE 与复数映射 a_{k,l}^{(p)}
 */
#ifndef SLB_RESOURCE_ELEMENT_H
#define SLB_RESOURCE_ELEMENT_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** 资源元素复数值 */
typedef struct SlbComplex {
    float real;
    float imag;
} SlbComplex;

/** RE 索引 (k, l)_p（6.2.2.7） */
typedef struct SlbResourceElementIndex {
    uint16_t subcarrierIndex; /**< k: 0 ~ 160 */
    uint8_t  symbolIndex;     /**< l: 0 ~ N_sym^frame - 1 */
    uint8_t  antennaPortIndex;
} SlbResourceElementIndex;

/**
 * @brief 校验 RE 索引合法性（含 k != 80 约束）
 */
bool slbValidateResourceElementIndex(const SlbResourceElementIndex *reIndex,
                                     uint8_t symbolCountPerRadioFrame);

/**
 * @brief 强制 DC 约束：a_{80,l} ≡ 0
 */
void slbEnforceDcSubcarrierZero(SlbComplex *gridValues,
                                uint8_t symbolCountPerRadioFrame);

/**
 * @brief 写入 RE 值；若 k=80 或索引非法则拒绝
 */
bool slbMapResourceElementValue(SlbComplex *gridValues,
                                uint16_t subcarrierCount,
                                uint8_t symbolCountPerRadioFrame,
                                const SlbResourceElementIndex *reIndex,
                                SlbComplex value);

#ifdef __cplusplus
}
#endif

#endif /* SLB_RESOURCE_ELEMENT_H */
