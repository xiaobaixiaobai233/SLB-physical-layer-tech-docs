/**
 * @file slb_phy_common.h
 * @brief 6.5.1 CRC / 6.5.2 Gold 序列 / 6.5.3 伪随机 QPSK 参考信号
 */
#ifndef SLB_PHY_COMMON_H
#define SLB_PHY_COMMON_H

#include "slb_phy_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ---------- 6.5.1 CRC ---------- */

typedef enum {
    SLB_CRC24A = 0,
    SLB_CRC24B,
    SLB_CRC16,
    SLB_CRC8,
    SLB_CRC6
} slb_crc_poly_t;

typedef struct {
    const uint8_t *a;
    uint32_t       A;
    slb_crc_poly_t poly;
    const uint8_t *mask; /* 24 bit，NULL 表示无掩码 */
    uint8_t       *b;
    uint32_t       b_cap;
} slb_crc_encode_t;

typedef struct {
    const uint8_t *b;
    uint32_t       B;
    slb_crc_poly_t poly;
    const uint8_t *mask;
    uint8_t       *pass;
} slb_crc_check_t;

uint32_t slb_crc_get_len(slb_crc_poly_t poly);
int      slb_crc_encode(const slb_crc_encode_t *cfg);
int      slb_crc_check(const slb_crc_check_t *cfg);

/* ---------- 6.5.2 Gold 序列 ---------- */

typedef struct {
    uint32_t  c_init;
    uint32_t  M_PN;
    uint8_t  *c;
    uint32_t  c_cap;
} slb_pn_seq_t;

int slb_pn_gold_generate(const slb_pn_seq_t *cfg);

/* ---------- 6.5.3 伪随机 QPSK 参考信号 ---------- */

typedef struct {
    uint8_t n;
    uint8_t l;
    uint8_t N_ID;
    uint8_t N_CP;
} slb_qpsk_cinit_param_t;

typedef struct {
    slb_qpsk_cinit_param_t param;
    slb_cplx_t            *r;
    uint32_t               r_cap;
} slb_qpsk_seq_t;

#define SLB_QPSK_N_SC 161u
#define SLB_QPSK_DC_K 80u

uint32_t slb_qpsk_calc_cinit(const slb_qpsk_cinit_param_t *param);
int      slb_qpsk_ref_generate(const slb_qpsk_seq_t *cfg);
int      slb_qpsk_map_to_re(const slb_cplx_t *r, slb_cplx_t *a_kl, uint32_t k);

#ifdef __cplusplus
}
#endif

#endif /* SLB_PHY_COMMON_H */
