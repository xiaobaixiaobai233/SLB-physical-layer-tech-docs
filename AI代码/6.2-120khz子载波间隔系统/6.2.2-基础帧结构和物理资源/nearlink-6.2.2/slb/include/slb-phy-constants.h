/**
 * @file slb-phy-constants.h
 * @brief SLB 120 kHz 物理层 6.2.2 节全局常量（对应标准 6.1 / 6.2.2 基本量）
 *
 * airMode = slb
 * 依据 T/XS 10001-2025 第 6.2.2 节整理。
 */
#ifndef SLB_PHY_CONSTANTS_H
#define SLB_PHY_CONSTANTS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** 基准频率 fs = 30.72 MHz（6.1） */
#define SLB_SAMPLE_RATE_HZ 30720000U

/** OFDM FFT 点数（有效数据段 = 256 * Ts） */
#define SLB_FFT_SIZE 256U

/** 子载波间隔 Δf = fs / 256 = 120 kHz（6.2.2.1） */
#define SLB_SUBCARRIER_SPACING_HZ 120000U

/** 超帧时长 Tf = 30720 * Ts = 1 ms（6.2.2.2） */
#define SLB_SUPERFRAME_DURATION_TS 30720U

/** 每超帧无线帧数（6.2.2.2） */
#define SLB_RADIO_FRAME_COUNT_PER_SUPERFRAME 8U

/** 超帧编号循环上界（#0 ~ #65535） */
#define SLB_SUPERFRAME_INDEX_MAX 65535U

/** 基础信道带宽 20 MHz（6.2.2.4） */
#define SLB_BASE_CHANNEL_BANDWIDTH_MHZ 20U

/** 每基础载波子载波数（6.2.2.4） */
#define SLB_SUBCARRIER_COUNT_PER_BASE_CARRIER 161U

/** DC 子载波索引 k = 80（6.2.2.4 / 6.2.2.7） */
#define SLB_DC_SUBCARRIER_INDEX 80U

/** 每基础载波基础子载波组数（6.2.2.4） */
#define SLB_BASE_SUBCARRIER_GROUP_COUNT 16U

/** 每组有效子载波数（6.2.2.4） */
#define SLB_SUBCARRIER_COUNT_PER_GROUP 10U

/** N_TTI 最大值（6.2.2.3） */
#define SLB_TTI_EXPONENT_MAX 6U

/** 表 4 支持的 N_CH 取值个数 */
#define SLB_NODE_CHANNEL_COUNT_TABLE_SIZE 18U

#ifdef __cplusplus
}
#endif

#endif /* SLB_PHY_CONSTANTS_H */
