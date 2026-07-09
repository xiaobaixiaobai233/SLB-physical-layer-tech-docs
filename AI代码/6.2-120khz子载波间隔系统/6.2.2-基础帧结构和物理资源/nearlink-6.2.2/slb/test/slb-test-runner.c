/**
 * @file slb-test-runner.c
 * @brief 6.2.2 标准符合性 demo 总入口
 *
 * 运行全部 demo 套件，汇总 PASS/FAIL。
 * 依据 T/XS 10001-2025 §6.2.2（团体标准 20250326 版）。
 */
#include <stdio.h>

#include "slb-test.h"

int main(void)
{
    unsigned int totalFailCount = 0U;

    printf("SLB 6.2.2 Standard Compliance Demos\n");
    printf("Reference: T/XS 10001-2025 section 6.2.2\n");
    printf("========================================\n");

    totalFailCount += slbRunDemo6221CpOfdmSymbol();
    totalFailCount += slbRunDemo6222Superframe();
    totalFailCount += slbRunDemo6223CotTti();
    totalFailCount += slbRunDemo6224Carrier();
    totalFailCount += slbRunDemo6225ResourceGrid();
    totalFailCount += slbRunDemo6228CommDomainLink();
    totalFailCount += slbRunDemoRxResourceLocate();

    printf("\n========================================\n");
    if (totalFailCount == 0U) {
        printf("ALL DEMOS PASSED\n");
        return 0;
    }
    printf("TOTAL FAILURES: %u\n", totalFailCount);
    return 1;
}
