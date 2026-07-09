/**
 * @file slb-phy-signal.h
 * @brief SLB 6.2.3 物理层信号模块统一入口
 * airMode = slb
 */
#ifndef SLB_PHY_SIGNAL_H
#define SLB_PHY_SIGNAL_H

#include "slb-phy-types.h"
#include "slb-gold-seq.h"
#include "slb-zc-seq.h"
#include "slb-ref-signal-common.h"
#include "slb-fts.h"
#include "slb-sts.h"
#include "slb-srs.h"
#include "slb-csi-rs.h"
#include "slb-g-dmrs-c.h"
#include "slb-p-dmrs-c.h"
#include "slb-dmrs-d.h"
#include "slb-t-dmrs-c.h"
#include "slb-pas.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 初始化全部可预计算查表（FTS/STS LUT + Gold x1 LUT）
 * 对应编程要求：预先生成 FTS、STS 及 Gold 路径 x1
 */
int slbInitPhySignalModule(void);

#ifdef __cplusplus
}
#endif

#endif /* SLB_PHY_SIGNAL_H */
