/**
 * @file slb_phy_pwr_meas.h
 * @brief 6.5.4 功率控制 / 6.5.5 测量量
 */
#ifndef SLB_PHY_PWR_MEAS_H
#define SLB_PHY_PWR_MEAS_H

#include "slb_phy_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ---------- 6.5.4 功率控制 ---------- */

int slb_pwr_calc_pathloss(float gLinkRefPwr_dBm, float rsrp_dBm, float *PL_dB);
int slb_pwr_calc_initial(uint16_t M, float P0_dBm, float PL_dB, float *P_dBm);
int slb_pwr_dBm_to_linear(float P_dBm, float *P_lin_mW);
int slb_pwr_linear_to_dBm(float P_lin_mW, float *P_dBm);
int slb_pwr_aggregate(const float *P_lin, uint8_t N, uint16_t K, float Pmax_lin,
                      float *Pk_lin);

typedef enum {
    SLB_PO_SRS = 0,
    SLB_PO_RACH,
    SLB_PO_ACK,
    SLB_PO_DATA_CLASS1,
    SLB_PO_DATA_CLASS2
} slb_po_type_e;

/* ---------- 6.5.5 通信测量量 ---------- */

typedef struct {
    const float *re_power; /* 各 RE 线性功率 (W) */
    uint16_t     n_re;
    const float *ant_power; /* 多天线时各天线 RSRP，可为 NULL */
    uint8_t      n_ant;
} slb_meas_rsrp_cfg_t;

typedef struct {
    const float *sym_power; /* 整符号各 RE 总功率或预聚合 */
    uint16_t     n_re;
    const float *ant_power;
    uint8_t      n_ant;
} slb_meas_rssi_cfg_t;

typedef struct {
    const float *sym_pwr_dBm; /* 测量窗口内各符号/段功率 (dBm) */
    uint32_t     n_sym;
    float        T_measure_s;
    uint8_t      dt_preamble_detected;
    float        dt_frame_duration_s; /* 检测到 DT 时使用 */
} slb_meas_cbra_cfg_t;

int slb_meas_rsrp(const slb_meas_rsrp_cfg_t *cfg, float *rsrp_linear);
int slb_meas_rssi(const slb_meas_rssi_cfg_t *cfg, float *rssi_linear);
int slb_meas_rsrq(float rsrp_linear, float rssi_linear, float *rsrq);
int slb_meas_sinr(float rsrp_linear, float rssi_linear, float *sinr);
int slb_meas_cbra(const slb_meas_cbra_cfg_t *cfg, float *cbra);

#define SLB_CBRA_THRESH_DT_DBM   (-87.0f)
#define SLB_CBRA_THRESH_SYM_DBM  (-62.0f)

/* ---------- 6.5.5 位置测量量 ---------- */

typedef struct {
    const slb_cplx_t *freq_iq;
    uint8_t           mode; /* 1=全帧平均，2=第 n 个测距符号 */
    uint8_t           n_rx;
    uint8_t           n_tx;
    uint16_t          n_sc;
} slb_meas_pms_csi_cfg_t;

typedef struct {
    int16_t *iq_linear;
    int16_t *rpl_dBm;
} slb_pms_csi_t;

typedef struct {
    double t[6]; /* t1..t6: TOA/TOD 时间戳 */
    uint8_t n_signals; /* 2 或 3 */
    uint8_t node_role; /* 1=第一位置节点，2=第二位置节点 */
} slb_pms_time_t;

typedef struct {
    double T;
    double T1;
    double T2;
} slb_pms_timediff_t;

typedef struct {
    const slb_cplx_t *array_snap;
    uint8_t           n_rx;
    uint16_t          n_snap;
} slb_meas_pms_aoa_cfg_t;

typedef struct {
    float azimuth_deg;
    float elevation_deg;
} slb_pms_aoa_t;

int slb_meas_pms_csi(const slb_meas_pms_csi_cfg_t *cfg, slb_pms_csi_t *out);
int slb_meas_pms_timediff(const slb_pms_time_t *t, slb_pms_timediff_t *out);
int slb_meas_pms_aoa(const slb_meas_pms_aoa_cfg_t *cfg, slb_pms_aoa_t *out);
float slb_meas_pms_iq_dBm(int16_t iq_linear, int16_t rpl_dBm);

#ifdef __cplusplus
}
#endif

#endif /* SLB_PHY_PWR_MEAS_H */
