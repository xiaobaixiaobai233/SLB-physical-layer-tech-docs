/**
 * @file slb-g-dmrs-c.h
 * @brief G 链路公共解调参考信号 G-DMRS-C（6.2.3.4）
 * airMode = slb
 */
#ifndef SLB_G_DMRS_C_H
#define SLB_G_DMRS_C_H

#include "slb-ref-signal-common.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum SlbGDmrsCSymbolIndex {
    SLB_G_DMRS_C_SYMBOL_6 = 6,  /**< 含 SAB 的 TTI 首帧 #6 */
    SLB_G_DMRS_C_SYMBOL_0 = 0   /**< 不含 SAB 的 TTI 首帧 #0 */
} SlbGDmrsCSymbolIndex;

typedef struct SlbGDmrsCConfig {
    SlbRnLContext           rnLContext;
    bool                    hasSabOnCarrier;
    SlbGDmrsCSymbolIndex    symbolIndex;   /**< 由 hasSabOnCarrier 推导或显式指定 */
} SlbGDmrsCConfig;

void slbGenerateGDmrsCSequence(const SlbGDmrsCConfig *config,
                              SlbSubcarrierBuffer gDmrsCSequence);

/** 选择 G-DMRS-C 时域符号索引 */
uint8_t slbSelectGDmrsCSymbolIndex(bool hasSabOnCarrier);

void slbMapGDmrsCToResourceGrid(const SlbGDmrsCConfig *config,
                                const SlbSubcarrierBuffer gDmrsCSequence,
                                SlbSubcarrierBuffer symbolBuffer);

#ifdef __cplusplus
}
#endif

#endif /* SLB_G_DMRS_C_H */
