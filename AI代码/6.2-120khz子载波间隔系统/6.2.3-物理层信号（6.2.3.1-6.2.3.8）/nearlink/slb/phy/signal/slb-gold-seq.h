/**
 * @file slb-gold-seq.h
 * @brief 31 阶 Gold 序列 c(i) 与伪随机 QPSK 序列 r_{n,l}(k)（6.5.2 / 6.5.3）
 * airMode = slb
 */
#ifndef SLB_GOLD_SEQ_H
#define SLB_GOLD_SEQ_H

#include "slb-phy-types.h"

#ifdef __cplusplus
extern "C" {
#endif

/** r_{n,l}(k) 生成上下文参数 */
typedef struct SlbRnLContext {
    uint8_t  radioFrameIndex;   /**< n: 0..7 */
    uint8_t  symbolIndex;       /**< l: 依符号类型 */
    uint16_t nId;               /**< N_ID: G 节点标识低 8 位 */
    uint8_t  nCp;               /**< N_CP: 0..3 */
} SlbRnLContext;

/**
 * @brief 初始化 x1 预计算查找表（模块启动时调用一次）
 * @return 0 成功，非 0 失败
 */
int slbInitGoldSequenceTables(void);

/** 是否已完成 x1 预计算 */
bool slbIsGoldSequenceTablesReady(void);

/**
 * @brief 计算 c_init（6.5.3）
 */
uint32_t slbCalculateCInit(uint8_t radioFrameIndex,
                           uint8_t symbolIndex,
                           uint16_t nId,
                           uint8_t nCp);

/**
 * @brief 由上下文计算 c_init
 */
uint32_t slbCalculateCInitFromContext(const SlbRnLContext *context);

/**
 * @brief 生成 Gold 比特序列 c(i)，i = 0..bitCount-1
 * @param cInit     x2 初态（31 bit）
 * @param bitCount  输出比特数（r_{n,l} 需 322）
 * @param goldBits  输出缓冲，至少 bitCount 字节（每字节 0/1）
 */
void slbGenerateGoldSequenceBits(uint32_t cInit,
                                 uint16_t bitCount,
                                 uint8_t *goldBits);

/**
 * @brief 生成伪随机 QPSK 序列 r_{n,l}(k)，k = 0..160（6.5.3）
 * @param context       (n, l, N_ID, N_CP)
 * @param rnLSequence   输出 161 个复数；DC 位置由调用方或本函数置零
 */
void slbGenerateRnLSequence(const SlbRnLContext *context,
                            SlbSubcarrierBuffer rnLSequence);

#ifdef __cplusplus
}
#endif

#endif /* SLB_GOLD_SEQ_H */
