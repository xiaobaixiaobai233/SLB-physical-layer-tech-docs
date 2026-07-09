/**
 * @file slb-antenna-port.h
 * @brief 6.2.2.5 天线端口（逻辑端口）
 */
#ifndef SLB_ANTENNA_PORT_H
#define SLB_ANTENNA_PORT_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** 天线端口索引（逻辑概念，非物理天线编号） */
typedef uint8_t SlbAntennaPortIndex;

/** 通信域同端口内容类别（SAB / G-PCIB / 广播 / G-DMRS-C 须同端口） */
typedef enum SlbCommDomainPortContent {
    SLB_COMM_DOMAIN_PORT_SAB       = 0,
    SLB_COMM_DOMAIN_PORT_G_PCIB    = 1,
    SLB_COMM_DOMAIN_PORT_BROADCAST = 2,
    SLB_COMM_DOMAIN_PORT_G_DMRS_C  = 3
} SlbCommDomainPortContent;

/** 通信域端口绑定配置 */
typedef struct SlbCommDomainAntennaPortConfig {
    SlbAntennaPortIndex sabPortIndex;
    SlbAntennaPortIndex gPcibPortIndex;
    SlbAntennaPortIndex broadcastPortIndex;
    SlbAntennaPortIndex gDmrsCPortIndex;
} SlbCommDomainAntennaPortConfig;

/**
 * @brief 校验通信域 SAB/G-PCIB/广播/G-DMRS-C 是否使用同一天线端口
 */
bool slbValidateCommDomainUnifiedPort(
    const SlbCommDomainAntennaPortConfig *portConfig);

#ifdef __cplusplus
}
#endif

#endif /* SLB_ANTENNA_PORT_H */
