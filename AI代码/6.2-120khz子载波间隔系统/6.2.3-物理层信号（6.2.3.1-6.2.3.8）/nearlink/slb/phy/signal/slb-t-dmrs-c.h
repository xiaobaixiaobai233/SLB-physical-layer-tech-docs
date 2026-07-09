/**
 * @file slb-t-dmrs-c.h
 * @brief T 链路控制信息解调参考信号 T-DMRS-C（6.2.3.7）
 * airMode = slb
 */
#ifndef SLB_T_DMRS_C_H
#define SLB_T_DMRS_C_H

#include "slb-ref-signal-common.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct SlbTDmrsCConfig {
    SlbRnLContext            rnLContext;
    SlbSubcarrierGroupConfig subcarrierGroup;
    SlbCombSubcarrierConfig  combConfig;   /**< 与 TCI 梳齿集合完全相同 */
    SlbAntennaPortConfig     antennaPortConfig;
    bool                     hasSubcarrierGroup;
} SlbTDmrsCConfig;

void slbGenerateTDmrsCSequence(const SlbTDmrsCConfig *config,
                               SlbSubcarrierBuffer tDmrsCSequence);

/** 映射于 TCI 发送符号的第一个符号，频域梳齿与 TCI 相同 */
void slbMapTDmrsCToResourceGrid(const SlbTDmrsCConfig *config,
                                const SlbSubcarrierBuffer tDmrsCSequence,
                                SlbComplex antennaPortBuffers[][SLB_SUBCARRIER_COUNT]);

#ifdef __cplusplus
}
#endif

#endif /* SLB_T_DMRS_C_H */
