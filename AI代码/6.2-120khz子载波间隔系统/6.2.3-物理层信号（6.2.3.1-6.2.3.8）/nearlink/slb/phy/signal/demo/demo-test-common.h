/**
 * @file demo-test-common.h
 * @brief 6.2.3 验证 demo 公共测试工具
 */
#ifndef DEMO_TEST_COMMON_H
#define DEMO_TEST_COMMON_H

#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include "slb-phy-types.h"

#define DEMO_PHASE_TOL           1.0e-4f
#define DEMO_MAG_TOL             1.0e-3f
#define DEMO_QPSK_TOL            1.0e-5f

extern int gDemoPassCount;
extern int gDemoFailCount;

/** Call once at program start (UTF-8 console on Windows). */
void demoInitConsole(void);

void demoPrintSection(const char *sectionTitle);
void demoAssertTrue(bool condition, const char *testName);
void demoAssertNear(float actual, float expected, float tolerance, const char *testName);
void demoAssertComplexNear(const SlbComplex *actual,
                           const SlbComplex *expected,
                           float tolerance,
                           const char *testName);
float demoComplexMagnitude(const SlbComplex *value);
float demoMaxComplexError(const SlbSubcarrierBuffer a,
                          const SlbSubcarrierBuffer b);
int demoCountNonZeroRe(const SlbSubcarrierBuffer buffer);
int demoPrintSummary(const char *demoName);

#endif /* DEMO_TEST_COMMON_H */
