/**
 * @file slb-passthrough-link.h
 * @brief 6.2.2.10 直通链路（passthroughLink，D 符号，竞争获取）
 */
#ifndef SLB_PASSTHROUGH_LINK_H
#define SLB_PASSTHROUGH_LINK_H

#include <stdbool.h>
#include <stdint.h>

#include "slb-cot-tti.h"
#include "slb-superframe.h"

#ifdef __cplusplus
extern "C" {
#endif

/** 直通链路节点角色（6.2.2.10） */
typedef enum SlbPassthroughNodeRole {
    SLB_PASSTHROUGH_NODE_FIRST  = 0, /**< 先发节点：竞争信道成功者 */
    SLB_PASSTHROUGH_NODE_SECOND = 1  /**< 后发节点：与先发节点通信 */
} SlbPassthroughNodeRole;

/** 直通链路上下文 */
typedef struct SlbPassthroughLinkContext {
    SlbChannelOccupancyTime cot;       /**< 竞争成功后获得的 COT */
    SlbPassthroughNodeRole  nodeRole;
    uint16_t                firstNodeId;
    uint16_t                secondNodeId;
} SlbPassthroughLinkContext;

/**
 * @brief 校验符号为 D 符号（直通链路专用，不可用于通信域 G/T/A）
 */
bool slbValidatePassthroughSymbolKind(SlbLinkSymbolKind symbolKind);

/**
 * @brief 校验直通链路不使用 gNode 预连接调度（仅竞争 COT + D 符号序列）
 */
bool slbValidatePassthroughLinkContext(const SlbPassthroughLinkContext *context);

#ifdef __cplusplus
}
#endif

#endif /* SLB_PASSTHROUGH_LINK_H */
