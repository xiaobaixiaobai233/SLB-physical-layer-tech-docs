/**
 * @file slb-cp-ofdm-symbol.h
 * @brief 6.2.2.1 CP-OFDM 波形与五种符号类型（表 1）
 */
#ifndef SLB_CP_OFDM_SYMBOL_H
#define SLB_CP_OFDM_SYMBOL_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** 120 kHz CP-OFDM 符号类型（表 1） */
typedef enum SlbCpOfdmSymbolType {
    SLB_CP_OFDM_SYMBOL_TYPE_1A = 0,
    SLB_CP_OFDM_SYMBOL_TYPE_1B = 1,
    SLB_CP_OFDM_SYMBOL_TYPE_2  = 2,
    SLB_CP_OFDM_SYMBOL_TYPE_3  = 3,
    SLB_CP_OFDM_SYMBOL_TYPE_4  = 4
} SlbCpOfdmSymbolType;

/** 符号时域参数（单位：Ts） */
typedef struct SlbSymbolTimingParams {
    uint16_t cpLengthTs;           /**< 一般 CP 长度 */
    uint16_t symbolLengthTs;       /**< 一般 CP-OFDM 符号总长度 */
    uint16_t gap1LengthTs;         /**< GAP1；类型三/四为 0 */
    uint16_t boundarySymbolLengthTs; /**< 边界 CP-OFDM 符号长度；类型三/四等于 symbolLengthTs */
    bool     hasBoundarySymbol;    /**< 是否存在单独边界符号配置（表 3） */
} SlbSymbolTimingParams;

/**
 * @brief 按符号类型查表 1，获取 CP/GAP/符号时长参数
 * @param symbolType 符号类型
 * @param timingParams 输出参数
 * @return true 成功；false 无效 symbolType
 */
bool slbSelectSymbolTimingParams(SlbCpOfdmSymbolType symbolType,
                                 SlbSymbolTimingParams *timingParams);

/**
 * @brief 查表 2：每无线帧符号数 N_sym^frame
 */
uint8_t slbCalculateSymbolCountPerRadioFrame(SlbCpOfdmSymbolType symbolType);

/**
 * @brief 判断符号索引 l 是否为边界 CP-OFDM 符号（表 3）
 */
bool slbValidateIsBoundarySymbol(SlbCpOfdmSymbolType symbolType, uint8_t symbolIndex);

/**
 * @brief 6.5.3 衔接：符号类型 → N_CP 索引
 * 类型一 A/B→3；类型二→2；类型三→1；类型四→0
 */
uint8_t slbSelectNcPIndex(SlbCpOfdmSymbolType symbolType);

#ifdef __cplusplus
}
#endif

#endif /* SLB_CP_OFDM_SYMBOL_H */
