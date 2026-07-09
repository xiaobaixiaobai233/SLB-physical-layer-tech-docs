/**
 * @file slb-test.h
 * @brief 轻量测试框架（无外部依赖），用于 6.2.2 标准符合性 demo
 *
 * airMode = slb
 */
#ifndef SLB_TEST_H
#define SLB_TEST_H

#include <stdio.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct SlbTestStats {
    const char *suiteName;
    unsigned int passCount;
    unsigned int failCount;
} SlbTestStats;

#define SLB_TEST_SUITE(name) \
    static SlbTestStats slbTestStats = { (name), 0U, 0U }

#define SLB_TEST_BEGIN() slbTestPrintSuiteHeader(slbTestStats.suiteName)

#define SLB_TEST_CASE(desc) slbTestPrintCase(desc)

#define SLB_TEST_TRUE(cond) slbTestAssertTrue(&(slbTestStats), __FILE__, __LINE__, (cond), #cond)

#define SLB_TEST_FALSE(cond) slbTestAssertTrue(&(slbTestStats), __FILE__, __LINE__, !(cond), "!(" #cond ")")

#define SLB_TEST_EQ_U32(actual, expected) \
    slbTestAssertEqualU32(&(slbTestStats), __FILE__, __LINE__, (actual), (expected), #actual, #expected)

#define SLB_TEST_EQ_U16(actual, expected) \
    slbTestAssertEqualU16(&(slbTestStats), __FILE__, __LINE__, (actual), (expected), #actual, #expected)

#define SLB_TEST_EQ_U8(actual, expected) \
    slbTestAssertEqualU8(&(slbTestStats), __FILE__, __LINE__, (actual), (expected), #actual, #expected)

#define SLB_TEST_END() slbTestPrintSuiteSummary(&slbTestStats)

static inline void slbTestPrintSuiteHeader(const char *suiteName)
{
    printf("\n========== %s ==========\n", suiteName);
}

static inline void slbTestPrintCase(const char *description)
{
    printf("  [CASE] %s\n", description);
}

static inline void slbTestAssertTrue(SlbTestStats *stats,
                                     const char *file,
                                     int line,
                                     int condition,
                                     const char *expr)
{
    if (condition) {
        stats->passCount++;
        printf("    PASS: %s\n", expr);
    } else {
        stats->failCount++;
        printf("    FAIL: %s (%s:%d)\n", expr, file, line);
    }
}

static inline void slbTestAssertEqualU32(SlbTestStats *stats,
                                         const char *file,
                                         int line,
                                         unsigned long actual,
                                         unsigned long expected,
                                         const char *actualExpr,
                                         const char *expectedExpr)
{
    if (actual == expected) {
        stats->passCount++;
        printf("    PASS: %s == %s (%lu)\n", actualExpr, expectedExpr, actual);
    } else {
        stats->failCount++;
        printf("    FAIL: %s == %s, got %lu expect %lu (%s:%d)\n",
               actualExpr, expectedExpr, actual, expected, file, line);
    }
}

static inline void slbTestAssertEqualU16(SlbTestStats *stats,
                                         const char *file,
                                         int line,
                                         unsigned int actual,
                                         unsigned int expected,
                                         const char *actualExpr,
                                         const char *expectedExpr)
{
    slbTestAssertEqualU32(stats, file, line, actual, expected, actualExpr, expectedExpr);
}

static inline void slbTestAssertEqualU8(SlbTestStats *stats,
                                        const char *file,
                                        int line,
                                        unsigned int actual,
                                        unsigned int expected,
                                        const char *actualExpr,
                                        const char *expectedExpr)
{
    slbTestAssertEqualU32(stats, file, line, actual, expected, actualExpr, expectedExpr);
}

static inline void slbTestPrintSuiteSummary(const SlbTestStats *stats)
{
    printf("  --- %s: %u passed, %u failed ---\n",
           stats->suiteName, stats->passCount, stats->failCount);
}

static inline unsigned int slbTestFailCount(const SlbTestStats *stats)
{
    return stats->failCount;
}

/** 各 demo 入口（按 6.2.2 子节划分） */
unsigned int slbRunDemo6221CpOfdmSymbol(void);
unsigned int slbRunDemo6222Superframe(void);
unsigned int slbRunDemo6223CotTti(void);
unsigned int slbRunDemo6224Carrier(void);
unsigned int slbRunDemo6225ResourceGrid(void);
unsigned int slbRunDemo6228CommDomainLink(void);
unsigned int slbRunDemoRxResourceLocate(void);

#ifdef __cplusplus
}
#endif

#endif /* SLB_TEST_H */
