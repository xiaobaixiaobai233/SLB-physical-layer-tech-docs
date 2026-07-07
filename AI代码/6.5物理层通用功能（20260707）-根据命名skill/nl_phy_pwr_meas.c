/**
 * @file nl_phy_pwr_meas.c
 * @brief 6.5.4 功率控制 / 6.5.5 测量量实现
 */
#include "nl_phy_pwr_meas.h"

#include <math.h>

static float nl_ant_max(const float *v, uint8_t n)
{
    float   max_val = 0.0f;
    uint8_t i;

    if (v == NULL || n == 0u) {
        return 0.0f;
    }
    max_val = v[0];
    for (i = 1u; i < n; i++) {
        if (v[i] > max_val) {
            max_val = v[i];
        }
    }
    return max_val;
}

static float nl_linear_avg(const float *v, uint16_t n)
{
    float    sum = 0.0f;
    uint16_t i;

    if (v == NULL || n == 0u) {
        return 0.0f;
    }
    for (i = 0u; i < n; i++) {
        sum += v[i];
    }
    return sum / (float)n;
}

int nl_pwr_calc_pathloss(float gLinkRefPwr_dbm, float rsrp_dbm, float *pl_db)
{
    if (pl_db == NULL) {
        return NL_PHY_ERR_PARAM;
    }
    *pl_db = gLinkRefPwr_dbm - rsrp_dbm;
    return NL_PHY_OK;
}

int nl_pwr_calc_initial(uint16_t M, float P0_dbm, float pl_db, float *p_dbm)
{
    if (p_dbm == NULL || M == 0u) {
        return NL_PHY_ERR_PARAM;
    }
    *p_dbm = 10.0f * log10f((float)M) + P0_dbm + pl_db;
    return NL_PHY_OK;
}

int nl_pwr_dbm_to_linear(float p_dbm, float *p_lin_mW)
{
    if (p_lin_mW == NULL) {
        return NL_PHY_ERR_PARAM;
    }
    *p_lin_mW = powf(10.0f, p_dbm / 10.0f);
    return NL_PHY_OK;
}

int nl_pwr_linear_to_dbm(float p_lin_mW, float *p_dbm)
{
    if (p_dbm == NULL || p_lin_mW <= 0.0f) {
        return NL_PHY_ERR_PARAM;
    }
    *p_dbm = 10.0f * log10f(p_lin_mW);
    return NL_PHY_OK;
}

int nl_pwr_aggregate(const float *p_lin, uint8_t N, uint16_t K, float pMax_lin,
                     float *pK_lin)
{
    float   sum = 0.0f;
    float   total;
    uint8_t i;

    if (p_lin == NULL || pK_lin == NULL || N == 0u || K == 0u) {
        return NL_PHY_ERR_PARAM;
    }

    for (i = 0u; i < N; i++) {
        sum += p_lin[i];
    }

    total = (sum > pMax_lin) ? pMax_lin : sum;
    if (sum > pMax_lin) {
        for (i = 0u; i < K; i++) {
            pK_lin[i] = pMax_lin / (float)K;
        }
    } else {
        for (i = 0u; i < K; i++) {
            pK_lin[i] = total / (float)K;
        }
    }
    return NL_PHY_OK;
}

int nl_meas_rsrp(const nl_meas_rsrp_cfg_t *cfg, float *rsrp_linear)
{
    float val;

    if (cfg == NULL || rsrp_linear == NULL || cfg->re_power == NULL || cfg->n_re == 0u) {
        return NL_PHY_ERR_PARAM;
    }

    val = nl_linear_avg(cfg->re_power, cfg->n_re);
    if (cfg->ant_power != NULL && cfg->n_ant > 0u) {
        val = nl_ant_max(cfg->ant_power, cfg->n_ant);
    }
    *rsrp_linear = val;
    return NL_PHY_OK;
}

int nl_meas_rssi(const nl_meas_rssi_cfg_t *cfg, float *rssi_linear)
{
    float val;

    if (cfg == NULL || rssi_linear == NULL || cfg->sym_power == NULL || cfg->n_re == 0u) {
        return NL_PHY_ERR_PARAM;
    }

    val = nl_linear_avg(cfg->sym_power, cfg->n_re);
    if (cfg->ant_power != NULL && cfg->n_ant > 0u) {
        val = nl_ant_max(cfg->ant_power, cfg->n_ant);
    }
    *rssi_linear = val;
    return NL_PHY_OK;
}

int nl_meas_rsrq(float rsrp_linear, float rssi_linear, float *rsrq)
{
    if (rsrq == NULL || rssi_linear <= 0.0f) {
        return NL_PHY_ERR_PARAM;
    }
    *rsrq = rsrp_linear / rssi_linear;
    return NL_PHY_OK;
}

int nl_meas_sinr(float rsrp_linear, float rssi_linear, float *sinr)
{
    float denom;

    if (sinr == NULL) {
        return NL_PHY_ERR_PARAM;
    }
    denom = rssi_linear - rsrp_linear;
    if (denom <= 0.0f) {
        return NL_PHY_ERR_MEAS;
    }
    *sinr = rsrp_linear / denom;
    return NL_PHY_OK;
}

int nl_meas_cbra(const nl_meas_cbra_cfg_t *cfg, float *cbra)
{
    float    T_busy = 0.0f;
    float    sym_duration;
    uint32_t i;

    if (cfg == NULL || cbra == NULL || cfg->sym_pwr_dbm == NULL || cfg->n_sym == 0u
        || cfg->T_measure_s <= 0.0f) {
        return NL_PHY_ERR_PARAM;
    }

    sym_duration = cfg->T_measure_s / (float)cfg->n_sym;

    if (cfg->dt_preamble_detected) {
        for (i = 0u; i < cfg->n_sym; i++) {
            if (cfg->sym_pwr_dbm[i] > NL_CBRA_THRESH_DT_DBM) {
                T_busy = cfg->dt_frame_duration_s;
                break;
            }
        }
        if (T_busy <= 0.0f) {
            T_busy = cfg->dt_frame_duration_s;
        }
    } else {
        for (i = 0u; i < cfg->n_sym; i++) {
            if (cfg->sym_pwr_dbm[i] > NL_CBRA_THRESH_SYM_DBM) {
                T_busy += sym_duration;
            }
        }
    }

    *cbra = T_busy / cfg->T_measure_s;
    if (*cbra > 1.0f) {
        *cbra = 1.0f;
    }
    return NL_PHY_OK;
}

int nl_meas_pms_csi(const nl_meas_pms_csi_cfg_t *cfg, nl_pms_csi_t *out)
{
    uint16_t k;
    float    max_amp = 0.0f;

    if (cfg == NULL || out == NULL || cfg->freq_iq == NULL || cfg->n_sc == 0u) {
        return NL_PHY_ERR_PARAM;
    }
    if (out->iq_linear == NULL || out->rpl_dbm == NULL) {
        return NL_PHY_ERR_BUF;
    }

    for (k = 0u; k < cfg->n_sc; k++) {
        float amp = sqrtf(cfg->freq_iq[k].re * cfg->freq_iq[k].re
                          + cfg->freq_iq[k].im * cfg->freq_iq[k].im);
        if (amp > max_amp) {
            max_amp = amp;
        }
    }
    if (max_amp <= 0.0f) {
        max_amp = 1.0f;
    }

    for (k = 0u; k < cfg->n_sc; k++) {
        float amp = sqrtf(cfg->freq_iq[k].re * cfg->freq_iq[k].re
                          + cfg->freq_iq[k].im * cfg->freq_iq[k].im);
        float rel = amp / max_amp;
        int   iq  = (int)(rel * 1024.0f);
        if (iq > 2047) {
            iq = 2047;
        }
        if (iq < -2048) {
            iq = -2048;
        }
        out->iq_linear[k] = (int16_t)iq;
        out->rpl_dbm[k]   = 0;
        (void)cfg->mode;
        (void)cfg->n_rx;
        (void)cfg->n_tx;
    }
    return NL_PHY_OK;
}

float nl_meas_pms_iq_dbm(int16_t iq_linear, int16_t rpl_dbm)
{
    float mag = fabsf((float)iq_linear) / 1024.0f;
    if (mag <= 0.0f) {
        mag = 1e-12f;
    }
    return 20.0f * log10f(mag) + (float)rpl_dbm;
}

int nl_meas_pms_timediff(const nl_pms_time_t *t, nl_pms_timediff_t *out)
{
    if (t == NULL || out == NULL) {
        return NL_PHY_ERR_PARAM;
    }

    out->T  = 0.0;
    out->T1 = 0.0;
    out->T2 = 0.0;

    if (t->n_signals == 2u) {
        if (t->node_role == 1u) {
            out->T = t->t[3] - t->t[0]; /* t4 - t1 */
        } else if (t->node_role == 2u) {
            out->T = t->t[2] - t->t[1]; /* t3 - t2 */
        } else {
            return NL_PHY_ERR_PARAM;
        }
    } else if (t->n_signals == 3u) {
        if (t->node_role == 1u) {
            out->T1 = t->t[3] - t->t[0];
            out->T2 = t->t[4] - t->t[3];
        } else if (t->node_role == 2u) {
            out->T1 = t->t[2] - t->t[1];
            out->T2 = t->t[5] - t->t[2];
        } else {
            return NL_PHY_ERR_PARAM;
        }
    } else {
        return NL_PHY_ERR_PARAM;
    }
    return NL_PHY_OK;
}

int nl_meas_pms_aoa(const nl_meas_pms_aoa_cfg_t *cfg, nl_pms_aoa_t *out)
{
    float    sum_re = 0.0f;
    float    sum_im = 0.0f;
    uint16_t i;

    if (cfg == NULL || out == NULL || cfg->array_snap == NULL || cfg->n_rx == 0u) {
        return NL_PHY_ERR_PARAM;
    }

    /* 简化相位差估计：双天线或阵列首单元参考 */
    for (i = 0u; i < cfg->n_rx && i < cfg->n_snap; i++) {
        sum_re += cfg->array_snap[i].re;
        sum_im += cfg->array_snap[i].im;
    }

    out->azimuth_deg   = atan2f(sum_im, sum_re) * 180.0f / 3.14159265f;
    if (out->azimuth_deg < 0.0f) {
        out->azimuth_deg += 360.0f;
    }
    out->elevation_deg = 90.0f;
    (void)cfg->n_snap;
    return NL_PHY_OK;
}
