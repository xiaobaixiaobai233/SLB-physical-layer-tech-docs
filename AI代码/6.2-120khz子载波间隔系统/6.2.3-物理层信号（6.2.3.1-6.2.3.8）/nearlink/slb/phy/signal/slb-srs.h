/**
 * @file slb-srs.h
 * @brief 信道探测信号 SRS（6.2.3.2）
 * airMode = slb
 */
#ifndef SLB_SRS_H
#define SLB_SRS_H

#include "slb-ref-signal-common.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct SlbSrsConfig {
    SlbRnLContext            rnLContext;
    SlbSubcarrierGroupConfig subcarrierGroup;
    SlbCombSubcarrierConfig  combConfig;      /**< srs-Set-Conf 梳齿 */
    SlbAntennaPortConfig     antennaPortConfig;
    bool                     hasSubcarrierGroup;
} SlbSrsConfig;

/** 生成 SRS 伪随机 QPSK 序列 r_{n,l}(k) */
void slbGenerateSrsSequence(const SlbSrsConfig *config,
                            SlbSubcarrierBuffer srsSequence);

/**
 * @brief 映射 SRS 到 T 链路 RE：a_{k,l}^{(p)} = r_{n,l}(k)
 * @param antennaPortBuffers [portCount][161]
 */
void slbMapSrsToResourceGrid(const SlbSrsConfig *config,
                             const SlbSubcarrierBuffer srsSequence,
                             SlbComplex antennaPortBuffers[][SLB_SUBCARRIER_COUNT]);

#ifdef __cplusplus
}
#endif

#endif /* SLB_SRS_H */
