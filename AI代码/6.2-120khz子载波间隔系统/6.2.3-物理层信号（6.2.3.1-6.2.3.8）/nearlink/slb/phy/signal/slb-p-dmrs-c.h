/**
 * @file slb-p-dmrs-c.h
 * @brief 直通链路公共解调参考信号 P-DMRS-C（6.2.3.5）
 * airMode = slb
 */
#ifndef SLB_P_DMRS_C_H
#define SLB_P_DMRS_C_H

#include "slb-ref-signal-common.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SLB_P_DMRS_C_SYMBOL_INDEX  6u

typedef struct SlbPDmrsCConfig {
    SlbRnLContext rnLContext;
    bool          hasSabOnCarrier;  /**< 仅含 SAB 载波映射 */
} SlbPDmrsCConfig;

void slbGeneratePDmrsCSequence(const SlbPDmrsCConfig *config,
                              SlbSubcarrierBuffer pDmrsCSequence);

void slbMapPDmrsCToResourceGrid(const SlbPDmrsCConfig *config,
                                const SlbSubcarrierBuffer pDmrsCSequence,
                                SlbSubcarrierBuffer symbolBuffer);

#ifdef __cplusplus
}
#endif

#endif /* SLB_P_DMRS_C_H */
