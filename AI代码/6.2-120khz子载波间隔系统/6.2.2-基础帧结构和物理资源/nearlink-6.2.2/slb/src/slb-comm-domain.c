/**
 * @file slb-comm-domain.c
 * @brief 6.2.2.8 通信域校验
 */
#include "slb-comm-domain.h"

bool slbValidateCommDomain(const SlbCommDomain *commDomain)
{
    if (commDomain == NULL) {
        return false;
    }
    return (commDomain->hasSabResource ||
            commDomain->hasGPcibResource ||
            commDomain->hasBroadcastResource ||
            commDomain->hasSchedulableResource);
}
