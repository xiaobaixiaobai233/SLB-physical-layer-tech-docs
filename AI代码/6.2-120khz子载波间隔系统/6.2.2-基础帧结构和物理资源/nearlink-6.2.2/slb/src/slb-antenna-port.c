/**
 * @file slb-antenna-port.c
 * @brief 6.2.2.5 通信域天线端口统一校验
 */
#include "slb-antenna-port.h"

bool slbValidateCommDomainUnifiedPort(
    const SlbCommDomainAntennaPortConfig *portConfig)
{
    if (portConfig == NULL) {
        return false;
    }
    return (portConfig->sabPortIndex == portConfig->gPcibPortIndex &&
            portConfig->gPcibPortIndex == portConfig->broadcastPortIndex &&
            portConfig->broadcastPortIndex == portConfig->gDmrsCPortIndex);
}
