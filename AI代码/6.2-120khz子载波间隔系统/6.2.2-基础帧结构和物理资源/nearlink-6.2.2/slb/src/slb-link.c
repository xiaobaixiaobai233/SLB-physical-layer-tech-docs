/**
 * @file slb-link.c
 * @brief 6.2.2.9 gLink / tLink / auxLink
 */
#include "slb-link.h"

SlbLinkSymbolKind slbSelectLinkSymbolKind(SlbLogicalLinkType linkType)
{
    switch (linkType) {
    case SLB_LOGICAL_LINK_G_LINK:
        return SLB_LINK_SYMBOL_G;
    case SLB_LOGICAL_LINK_T_LINK:
        return SLB_LINK_SYMBOL_T;
    case SLB_LOGICAL_LINK_AUX_LINK:
        return SLB_LINK_SYMBOL_A;
    default:
        return SLB_LINK_SYMBOL_GAP;
    }
}

bool slbValidateCommDomainSymbolKind(SlbLinkSymbolKind symbolKind)
{
    return (symbolKind == SLB_LINK_SYMBOL_G ||
            symbolKind == SLB_LINK_SYMBOL_T ||
            symbolKind == SLB_LINK_SYMBOL_A ||
            symbolKind == SLB_LINK_SYMBOL_GAP);
}
