/**
 * @file slb-passthrough-link.c
 * @brief 6.2.2.10 直通链路（D 符号）
 */
#include "slb-passthrough-link.h"

bool slbValidatePassthroughSymbolKind(SlbLinkSymbolKind symbolKind)
{
    return (symbolKind == SLB_LINK_SYMBOL_D || symbolKind == SLB_LINK_SYMBOL_GAP);
}

bool slbValidatePassthroughLinkContext(const SlbPassthroughLinkContext *context)
{
    if (context == NULL) {
        return false;
    }
    if (context->cot.radioFrameCount == 0U) {
        return false;
    }
    /* 直通链路不允许通信域 G/T/A 符号混用，由上层保证 D 符号序列 */
    return true;
}
