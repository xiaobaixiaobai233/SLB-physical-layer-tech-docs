/**
 * @file slb_phy_test.c
 * @brief SLB 6.5 通用功能库基础自测
 */
#include "slb_phy.h"

#include <stdio.h>
#include <string.h>

static int test_crc_gold_qpsk(void)
{
    slb_qpsk_cinit_param_t p = {0, 0, 5, 3};
    uint32_t               c_init;
    slb_qpsk_seq_t         qs;
    slb_cplx_t             r[161];
    uint8_t                gold5[5];
    slb_pn_seq_t           pn;
    int                    ok = 1;

    c_init = slb_qpsk_calc_cinit(&p);
    if (c_init != 344087u) {
        printf("FAIL c_init: got %u expect 344087\n", c_init);
        ok = 0;
    }

    pn.c_init = c_init;
    pn.M_PN   = 5u;
    pn.c      = gold5;
    pn.c_cap  = 5u;
    slb_pn_gold_generate(&pn);
    if (gold5[0] != 0u || gold5[1] != 1u || gold5[2] != 1u || gold5[3] != 1u
        || gold5[4] != 0u) {
        printf("FAIL gold prefix\n");
        ok = 0;
    }

    qs.param = p;
    qs.r     = r;
    qs.r_cap = 161u;
    if (slb_qpsk_ref_generate(&qs) != SLB_PHY_OK) {
        printf("FAIL qpsk ref\n");
        ok = 0;
    }
    return ok;
}

static int test_mod_scram(void)
{
    uint8_t    bits[4] = {1, 0, 1, 1};
    slb_cplx_t sym;
    uint8_t    out[8];
    uint32_t   cinit;

    sym = slb_mod_qpsk_one(bits[0], bits[1]);
    if (sym.re > 0.0f) {
        printf("FAIL qpsk sign\n");
        return 0;
    }

    cinit = slb_scram_cinit_data(3, 100u);
    if (slb_scram_apply(bits, out, 4u, cinit) != SLB_PHY_OK) {
        printf("FAIL scram\n");
        return 0;
    }
    return 1;
}

static int test_pwr_meas(void)
{
    float PL;
    float P;
    float rsrq;

    slb_pwr_calc_pathloss(-10.0f, -70.0f, &PL);
    if (PL < 59.9f || PL > 60.1f) {
        printf("FAIL pathloss\n");
        return 0;
    }
    slb_pwr_calc_initial(12, -100.0f, PL, &P);
    if (P < -30.0f || P > -28.0f) {
        printf("FAIL initial power\n");
        return 0;
    }
    slb_meas_rsrq(1.0f, 4.0f, &rsrq);
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
    printf(pass ? "slb_phy_test: ALL PASS\n" : "slb_phy_test: FAILED\n");
    return pass ? 0 : 1;
}
