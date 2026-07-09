/**
 * @file slb-link.h
 * @brief 6.2.2.9 G 链路、tLink、auxLink 三条逻辑链路
 */
#ifndef SLB_LINK_H
#define SLB_LINK_H

#include <stdbool.h>
#include <stdint.h>

#include "slb-superframe.h"

#ifdef __cplusplus
extern "C" {
#endif

/** 逻辑链路类型（6.2.2.9） */
typedef enum SlbLogicalLinkType {
    SLB_LOGICAL_LINK_G_LINK = 0,  /**< gLink：gNode → 至少一个 tNode，G 符号 */
    SLB_LOGICAL_LINK_T_LINK = 1,  /**< tLink：tNode → gNode，T 符号 */
    SLB_LOGICAL_LINK_AUX_LINK = 2 /**< auxLink：tNode → tNode/G，A 符号 */
} SlbLogicalLinkType;

/** 链路方向描述 */
typedef struct SlbLogicalLinkDescriptor {
    SlbLogicalLinkType linkType;
    SlbLinkSymbolKind  symbolKind; /**< 对应 G / T / A 符号 */
} SlbLogicalLinkDescriptor;

/**
 * @brief 逻辑链路类型 → 无线帧符号类型
 */
SlbLinkSymbolKind slbSelectLinkSymbolKind(SlbLogicalLinkType linkType);

/**
 * @brief 校验符号类型是否属于通信域（G/T/A），非 D 符号
 */
bool slbValidateCommDomainSymbolKind(SlbLinkSymbolKind symbolKind);

#ifdef __cplusplus
}
#endif

#endif /* SLB_LINK_H */
