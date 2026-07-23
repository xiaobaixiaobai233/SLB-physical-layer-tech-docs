/**
 * @file nl_phy_pwr_meas.h
 * @brief 6.5.4 功率控制 / 6.5.5 测量量
 */
#ifndef NL_PHY_PWR_MEAS_H
#define NL_PHY_PWR_MEAS_H

#include "nl_phy_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ---------- 6.5.4 功率控制 ---------- */

int nl_pwr_calc_pathloss(float gLinkRefPwr_dbm, float rsrp_dbm, float *pl_db);
int nl_pwr_calc_initial(uint16_t M, float P0_dbm, float pl_db, float *p_dbm);
int nl_pwr_dbm_to_linear(float p_dbm, float *p_lin_mW);
int nl_pwr_linear_to_dbm(float p_lin_mW, float *p_dbm);
int nl_pwr_aggregate(const float *p_lin, uint8_t N, uint16_t K, float pMax_lin,
                     float *pK_lin);

typedef enum {
    NL_PO_SRS = 0,
    NL_PO_RACH,
    NL_PO_ACK,
    NL_PO_DATA_CLASS1,
    NL_PO_DATA_CLASS2
} nl_po_type_e;

/* ---------- 6.5.5 通信测量量 ---------- */

typedef struct {
    const float *re_power; /* 各 RE 线性功率 (W) */
    uint16_t     n_re;
    const float *ant_power; /* 多天线时各天线 RSRP，可为 NULL */
    uint8_t      n_ant;
} nl_meas_rsrp_cfg_t;

typedef struct {
    const float *sym_power; /* 整符号各 RE 总功率或预聚合 */
    uint16_t     n_re;
    const float *ant_power;
    uint8_t      n_ant;
} nl_meas_rssi_cfg_t;

typedef struct {
    const float *sym_pwr_dbm; /* 测量窗口内各符号/段功率 (dBm) */
    uint32_t     n_sym;
    float        T_measure_s;
    uint8_t      dt_preamble_detected;
    float        dt_frame_duration_s; /* 检测到 DT 时使用 */
} nl_meas_cbra_cfg_t;

int nl_meas_rsrp(const nl_meas_rsrp_cfg_t *cfg, float *rsrp_linear);
int nl_meas_rssi(const nl_meas_rssi_cfg_t *cfg, float *rssi_linear);
int nl_meas_rsrq(float rsrp_linear, float rssi_linear, float *rsrq);
int nl_meas_sinr(float rsrp_linear, float rssi_linear, float *sinr);
int nl_meas_cbra(const nl_meas_cbra_cfg_t *cfg, float *cbra);

#define NL_CBRA_THRESH_DT_DBM   (-87.0f)
#define NL_CBRA_THRESH_SYM_DBM  (-62.0f)

/* ---------- 6.5.5 位置测量量 ---------- */

typedef struct {
    const nl_cplx_t *freq_iq;
    uint8_t           mode; /* 1=全帧平均，2=第 n 个测距符号 */
    uint8_t           n_rx;
    uint8_t           n_tx;
    uint16_t          n_sc;
} nl_meas_pms_csi_cfg_t;

typedef struct {
    int16_t *iq_linear;
    int16_t *rpl_dbm;
} nl_pms_csi_t;

typedef struct {
    double t[6]; /* t1..t6: TOA/TOD 时间戳 */
    uint8_t n_signals; /* 2 或 3 */
    uint8_t node_role; /* 1=第一位置节点，2=第二位置节点 */
} nl_pms_time_t;

typedef struct {
    double T;
    double T1;
    double T2;
} nl_pms_timediff_t;

typedef struct {
    const nl_cplx_t *array_snap;
    uint8_t           n_rx;
    uint16_t          n_snap;
} nl_meas_pms_aoa_cfg_t;

typedef struct {
    float azimuth_deg;
    float elevation_deg;
} nl_pms_aoa_t;

int   nl_meas_pms_csi(const nl_meas_pms_csi_cfg_t *cfg, nl_pms_csi_t *out);
int   nl_meas_pms_timediff(const nl_pms_time_t *t, nl_pms_timediff_t *out);
int   nl_meas_pms_aoa(const nl_meas_pms_aoa_cfg_t *cfg, nl_pms_aoa_t *out);
float nl_meas_pms_iq_dbm(int16_t iq_linear, int16_t rpl_dbm);

#ifdef __cplusplus
}
#endif

#endif /* NL_PHY_PWR_MEAS_H */
