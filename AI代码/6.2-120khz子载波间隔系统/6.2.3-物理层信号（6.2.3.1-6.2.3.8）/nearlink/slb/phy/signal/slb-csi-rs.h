/**
 * @file slb-csi-rs.h
 * @brief 信道状态信息参考信号 CSI-RS（6.2.3.3）
 * airMode = slb
 */
#ifndef SLB_CSI_RS_H
#define SLB_CSI_RS_H

#include "slb-ref-signal-common.h"

#ifdef __cplusplus
extern "C" {
#endif

/** CSI-RS 时域起始符号规则 */
typedef enum SlbCsiRsStartSymbolRule {
    SLB_CSI_RS_START_AFTER_G_PCIB = 0,   /**< 含 SAB：G-PCIB 后首符号 */
    SLB_CSI_RS_START_AFTER_STS    = 1,   /**< 无 SAB 且 #0 为 STS */
    SLB_CSI_RS_START_TTI_FIRST    = 2    /**< 无 SAB 且 #0 非 STS */
} SlbCsiRsStartSymbolRule;

typedef struct SlbCsiRsConfig {
    SlbRnLContext              rnLContext;
    SlbSubcarrierGroupConfig   subcarrierGroup;
    SlbCombSubcarrierConfig    combConfig;       /**< csi-RS-Set-Conf 梳齿 */
    SlbAntennaPortConfig       antennaPortConfig;
    SlbCsiRsStartSymbolRule    startSymbolRule;
    bool                       hasSubcarrierGroup;
} SlbCsiRsConfig;

void slbGenerateCsiRsSequence(const SlbCsiRsConfig *config,
                              SlbSubcarrierBuffer csiRsSequence);

void slbMapCsiRsToResourceGrid(const SlbCsiRsConfig *config,
                               const SlbSubcarrierBuffer csiRsSequence,
                               SlbComplex antennaPortBuffers[][SLB_SUBCARRIER_COUNT]);

/** 根据载波类型与 STS 存在性选择起始符号规则 */
SlbCsiRsStartSymbolRule slbSelectCsiRsStartSymbolRule(bool hasSabOnCarrier,
                                                      bool isStsOnTtiFirstSymbol);

#ifdef __cplusplus
}
#endif

#endif /* SLB_CSI_RS_H */
