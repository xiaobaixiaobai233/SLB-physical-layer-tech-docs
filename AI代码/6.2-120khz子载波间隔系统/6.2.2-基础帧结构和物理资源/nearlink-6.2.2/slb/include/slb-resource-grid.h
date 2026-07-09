/**
 * @file slb-resource-grid.h
 * @brief 6.2.2.6 资源网格（161 × N_sym^frame RE 矩阵）
 */
#ifndef SLB_RESOURCE_GRID_H
#define SLB_RESOURCE_GRID_H

#include <stddef.h>
#include <stdint.h>

#include "slb-cp-ofdm-symbol.h"
#include "slb-resource-element.h"

#ifdef __cplusplus
extern "C" {
#endif

/** 资源网格：每天线端口 p、每基础载波、每无线帧一个网格 */
typedef struct SlbResourceGrid {
    SlbComplex *values;              /**< 长度 = subcarrierCount * symbolCount */
    uint16_t    subcarrierCount;     /**< 固定 161 */
    uint8_t     symbolCount;         /**< N_sym^frame */
    uint8_t     antennaPortIndex;
    uint8_t     baseCarrierIndex;
} SlbResourceGrid;

/**
 * @brief 初始化资源网格（values 由调用方分配）
 */
void slbInitResourceGrid(SlbResourceGrid *resourceGrid,
                         SlbComplex *valueBuffer,
                         SlbCpOfdmSymbolType symbolType,
                         uint8_t antennaPortIndex,
                         uint8_t baseCarrierIndex);

/**
 * @brief 将网格全部 RE 置零（含 DC 子载波）
 */
void slbClearResourceGrid(SlbResourceGrid *resourceGrid);

/**
 * @brief 读取 RE 值；索引非法返回 (0,0)
 */
SlbComplex slbReadResourceElement(const SlbResourceGrid *resourceGrid,
                                  const SlbResourceElementIndex *reIndex);

#ifdef __cplusplus
}
#endif

#endif /* SLB_RESOURCE_GRID_H */
