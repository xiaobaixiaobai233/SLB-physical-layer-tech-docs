/**
 * @file nl_phy_test.c
 * @brief NearLink 物理层通用功能库基础自测
 */
#include "nl_phy.h"

#include <stdio.h>
#include <string.h>

static int test_crc_gold_qpsk(void)
{
    nl_qpsk_cinit_param_t p = {0, 0, 5, 3};
    uint32_t              c_init;
    nl_qpsk_seq_cfg_t     qs;
    nl_cplx_t             r[161];
    uint8_t               gold5[5];
    nl_pn_seq_cfg_t       pn;
    int                   ok = 1;

    c_init = nl_qpsk_calc_cinit(&p);
    if (c_init != 344087u) {
        printf("FAIL c_init: got %u expect 344087\n", c_init);
        ok = 0;
    }

    pn.c_init = c_init;
    pn.M_PN   = 5u;
    pn.c      = gold5;
    pn.c_cap  = 5u;
    nl_pn_gold_generate(&pn);
    if (gold5[0] != 0u || gold5[1] != 1u || gold5[2] != 1u || gold5[3] != 1u
        || gold5[4] != 0u) {
        printf("FAIL gold prefix\n");
        ok = 0;
    }

    qs.param = p;
    qs.r     = r;
    qs.r_cap = 161u;
    if (nl_qpsk_ref_generate(&qs) != NL_PHY_OK) {
        printf("FAIL qpsk ref\n");
        ok = 0;
    }
    return ok;
}

static int test_mod_scram(void)
{
    uint8_t    bits[4] = {1, 0, 1, 1};
    nl_cplx_t  sym;
    uint8_t    out[8];
    uint32_t   cinit;

    sym = nl_mod_qpsk_one(bits[0], bits[1]);
    if (sym.re > 0.0f) {
        printf("FAIL qpsk sign\n");
        return 0;
    }

    cinit = nl_scram_calc_cinit_data(3, 100u);
    if (nl_scram_apply(bits, out, 4u, cinit) != NL_PHY_OK) {
        printf("FAIL scram\n");
        return 0;
    }
    return 1;
}

static int test_pwr_meas(void)
{
    float pl_db;
    float p;
    float rsrq;

    nl_pwr_calc_pathloss(-10.0f, -70.0f, &pl_db);
    if (pl_db < 59.9f || pl_db > 60.1f) {
        printf("FAIL pathloss\n");
        return 0;
    }
    nl_pwr_calc_initial(12, -100.0f, pl_db, &p);
    if (p < -30.0f || p > -28.0f) {
        printf("FAIL initial power\n");
        return 0;
    }
    nl_meas_rsrq(1.0f, 4.0f, &rsrq);
    if (rsrq < 0.24f || rsrq > 0.26f) {
        printf("FAIL rsrq\n");
        return 0;
    }
    return 1;
}

int main(void)
{
    int pass = 1;
    pass &= test_crc_gold_qpsk();
    pass &= test_mod_scram();
    pass &= test_pwr_meas();
    printf(pass ? "nl_phy_test: ALL PASS\n" : "nl_phy_test: FAILED\n");
    return pass ? 0 : 1;
}
