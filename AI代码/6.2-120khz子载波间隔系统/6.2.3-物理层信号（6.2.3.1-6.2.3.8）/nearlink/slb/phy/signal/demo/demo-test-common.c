/**
 * @file demo-test-common.c
 */
#include "demo-test-common.h"

#ifdef _WIN32
#include <windows.h>
#endif

int gDemoPassCount = 0;
int gDemoFailCount = 0;

void demoInitConsole(void)
{
#ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif
}

void demoPrintSection(const char *sectionTitle)
{
    printf("\n--- %s ---\n", sectionTitle);
}

void demoAssertTrue(bool condition, const char *testName)
{
    if (condition) {
        gDemoPassCount++;
        printf("[PASS] %s\n", testName);
    } else {
        gDemoFailCount++;
        printf("[FAIL] %s\n", testName);
    }
}

void demoAssertNear(float actual, float expected, float tolerance, const char *testName)
{
    demoAssertTrue(fabsf(actual - expected) <= tolerance, testName);
}

void demoAssertComplexNear(const SlbComplex *actual,
                           const SlbComplex *expected,
                           float tolerance,
                           const char *testName)
{
    bool isNear = (fabsf(actual->re - expected->re) <= tolerance) &&
                  (fabsf(actual->im - expected->im) <= tolerance);
    demoAssertTrue(isNear, testName);
}

float demoComplexMagnitude(const SlbComplex *value)
{
    return sqrtf(value->re * value->re + value->im * value->im);
}

float demoMaxComplexError(const SlbSubcarrierBuffer a,
                          const SlbSubcarrierBuffer b)
{
    float maxErr = 0.0f;
    for (uint16_t n = 0; n < SLB_SUBCARRIER_COUNT; n++) {
        float errRe = fabsf(a[n].re - b[n].re);
        float errIm = fabsf(a[n].im - b[n].im);
        if (errRe > maxErr) {
            maxErr = errRe;
        }
        if (errIm > maxErr) {
            maxErr = errIm;
        }
    }
    return maxErr;
}

int demoCountNonZeroRe(const SlbSubcarrierBuffer buffer)
{
    int count = 0;
    for (uint16_t subcarrierIndex = 0;
         subcarrierIndex < SLB_SUBCARRIER_COUNT;
         subcarrierIndex++) {
        if (buffer[subcarrierIndex].re != 0.0f ||
            buffer[subcarrierIndex].im != 0.0f) {
            count++;
        }
    }
    return count;
}

int demoPrintSummary(const char *demoName)
{
    printf("\n========== %s: %d PASS / %d FAIL ==========\n",
           demoName, gDemoPassCount, gDemoFailCount);
    return gDemoFailCount;
}
