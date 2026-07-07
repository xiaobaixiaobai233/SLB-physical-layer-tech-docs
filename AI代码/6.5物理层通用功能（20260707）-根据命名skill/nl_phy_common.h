/**
 * @file nl_phy_common.h
 * @brief 6.5.1 CRC / 6.5.2 Gold 序列 / 6.5.3 伪随机 QPSK 参考信号
 */
#ifndef NL_PHY_COMMON_H
#define NL_PHY_COMMON_H

#include "nl_phy_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ---------- 6.5.1 CRC ---------- */

typedef enum {
    NL_CRC24A = 0,
    NL_CRC24B,
    NL_CRC16,
    NL_CRC8,
    NL_CRC6
} nl_crc_poly_t;

typedef struct {
    const uint8_t *a;
    uint32_t       A;
    nl_crc_poly_t  poly;
    const uint8_t *mask; /* 24 bit，NULL 表示无掩码 */
    uint8_t       *b;
    uint32_t       b_cap;
} nl_crc_encode_cfg_t;

typedef struct {
    const uint8_t *b;
    uint32_t       B;
    nl_crc_poly_t  poly;
    const uint8_t *mask;
    uint8_t       *pass;
} nl_crc_check_cfg_t;

uint32_t nl_crc_get_len(nl_crc_poly_t poly);
int      nl_crc_encode(const nl_crc_encode_cfg_t *cfg);
int      nl_crc_check(const nl_crc_check_cfg_t *cfg);

/* ---------- 6.5.2 Gold 序列 ---------- */

/**
 * Gold 序列 x1 预计算：
 *   递推公式 x1(n+31) = (x1(n+3) + x1(n)) mod 2
 *   初始条件 x1(0)=1, x1(1..30)=0
 *   前 10001 个 x1 比特已预计算并打包在 nl_phy_common.c 的 nl_pn_x1_table[] 中。
 *   nl_pn_gold_generate() 仅迭代 x2 LFSR，x1 通过查表获取，消除全部 x1 计算。
 */

typedef struct {
    uint32_t  c_init;
    uint32_t  M_PN;
    uint8_t  *c;
    uint32_t  c_cap;
} nl_pn_seq_cfg_t;

int nl_pn_gold_generate(const nl_pn_seq_cfg_t *cfg);

/* ---------- 6.5.3 伪随机 QPSK 参考信号 ---------- */

typedef struct {
    uint8_t n;
    uint8_t l;
    uint8_t N_ID;
    uint8_t N_CP;
} nl_qpsk_cinit_param_t;

typedef struct {
    nl_qpsk_cinit_param_t param;
    nl_cplx_t            *r;
    uint32_t               r_cap;
} nl_qpsk_seq_cfg_t;

#define NL_QPSK_N_SC 161u
#define NL_QPSK_DC_K 80u

uint32_t nl_qpsk_calc_cinit(const nl_qpsk_cinit_param_t *param);
int      nl_qpsk_ref_generate(const nl_qpsk_seq_cfg_t *cfg);
int      nl_qpsk_map_to_re(const nl_cplx_t *r, nl_cplx_t *a_kl, uint32_t k);

#ifdef __cplusplus
}
#endif

#endif /* NL_PHY_COMMON_H */
