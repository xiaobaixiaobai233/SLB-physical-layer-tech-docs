/**
 * @file demo-6228-comm-domain-link.c
 * @brief Demo：6.2.2.8~6.2.2.10 通信域、gLink/tLink/auxLink、passthroughLink
 *
 * 标准依据：T/XS 10001-2025 §6.2.2.8 / §6.2.2.9 / §6.2.2.10
 */
#include "slb-test.h"

#include "slb-comm-domain.h"
#include "slb-cot-tti.h"
#include "slb-link.h"
#include "slb-passthrough-link.h"
#include "slb-superframe.h"

unsigned int slbRunDemo6228CommDomainLink(void)
{
    SlbCommDomain commDomain;
    SlbPassthroughLinkContext passthroughContext;
    SlbChannelOccupancyTime emptyCot = {0U, 0U};

    SLB_TEST_SUITE("Demo 6.2.2.8~6.2.2.10 Comm Domain / Links / Passthrough");
    SLB_TEST_BEGIN();

    SLB_TEST_CASE("gLink -> G symbol, tLink -> T, auxLink -> A");
    SLB_TEST_EQ_U8((unsigned int)slbSelectLinkSymbolKind(SLB_LOGICAL_LINK_G_LINK),
                   (unsigned int)SLB_LINK_SYMBOL_G);
    SLB_TEST_EQ_U8((unsigned int)slbSelectLinkSymbolKind(SLB_LOGICAL_LINK_T_LINK),
                   (unsigned int)SLB_LINK_SYMBOL_T);
    SLB_TEST_EQ_U8((unsigned int)slbSelectLinkSymbolKind(SLB_LOGICAL_LINK_AUX_LINK),
                   (unsigned int)SLB_LINK_SYMBOL_A);

    SLB_TEST_CASE("Comm domain accepts G/T/A/GAP, rejects D symbol");
    SLB_TEST_TRUE(slbValidateCommDomainSymbolKind(SLB_LINK_SYMBOL_G));
    SLB_TEST_TRUE(slbValidateCommDomainSymbolKind(SLB_LINK_SYMBOL_T));
    SLB_TEST_TRUE(slbValidateCommDomainSymbolKind(SLB_LINK_SYMBOL_A));
    SLB_TEST_TRUE(slbValidateCommDomainSymbolKind(SLB_LINK_SYMBOL_GAP));
    SLB_TEST_FALSE(slbValidateCommDomainSymbolKind(SLB_LINK_SYMBOL_D));

    SLB_TEST_CASE("Passthrough link accepts D and GAP only");
    SLB_TEST_TRUE(slbValidatePassthroughSymbolKind(SLB_LINK_SYMBOL_D));
    SLB_TEST_TRUE(slbValidatePassthroughSymbolKind(SLB_LINK_SYMBOL_GAP));
    SLB_TEST_FALSE(slbValidatePassthroughSymbolKind(SLB_LINK_SYMBOL_G));

    SLB_TEST_CASE("Comm domain must contain at least one resource type");
    commDomain.gNodeId = 1U;
    commDomain.baseCarrierIndex = 0U;
    commDomain.hasSabResource = false;
    commDomain.hasGPcibResource = false;
    commDomain.hasBroadcastResource = false;
    commDomain.hasSchedulableResource = false;
    SLB_TEST_FALSE(slbValidateCommDomain(&commDomain));
    commDomain.hasSabResource = true;
    SLB_TEST_TRUE(slbValidateCommDomain(&commDomain));

    SLB_TEST_CASE("Passthrough context requires non-zero COT frame count");
    passthroughContext.cot = emptyCot;
    passthroughContext.nodeRole = SLB_PASSTHROUGH_NODE_FIRST;
    passthroughContext.firstNodeId = 100U;
    passthroughContext.secondNodeId = 200U;
    SLB_TEST_FALSE(slbValidatePassthroughLinkContext(&passthroughContext));
    passthroughContext.cot.radioFrameCount = 4U;
    SLB_TEST_TRUE(slbValidatePassthroughLinkContext(&passthroughContext));

    SLB_TEST_END();
    return slbTestFailCount(&slbTestStats);
}
