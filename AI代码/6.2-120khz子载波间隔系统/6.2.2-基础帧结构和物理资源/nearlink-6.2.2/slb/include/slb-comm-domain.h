/**
 * @file slb-comm-domain.h
 * @brief 6.2.2.8 通信域（gNode 工作载波上的可调度资源集合）
 */
#ifndef SLB_COMM_DOMAIN_H
#define SLB_COMM_DOMAIN_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** 通信域资源范围（6.2.2.8） */
typedef struct SlbCommDomain {
    uint16_t gNodeId;              /**< 拥有该通信域的 gNode */
    uint8_t  baseCarrierIndex;     /**< gNode 工作载波上的基础载波序号 */
    bool     hasSabResource;       /**< 含 SAB 同步信息块资源 */
    bool     hasGPcibResource;     /**< 含 gLink 物理层控制信息块 G-PCIB */
    bool     hasBroadcastResource; /**< 含广播信息资源 */
    bool     hasSchedulableResource; /**< 含 gNode 可调度/配置资源 */
} SlbCommDomain;

/**
 * @brief 校验结构体是否描述一个非空通信域
 */
bool slbValidateCommDomain(const SlbCommDomain *commDomain);

#ifdef __cplusplus
}
#endif

#endif /* SLB_COMM_DOMAIN_H */
