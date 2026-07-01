# SLB 物理层 6.5 通用功能 C 库

依据 **T/XS 10001-2025** 第 6.5 节（6.5.1～6.5.9）实现的 SLB 120 kHz 物理层共性算法库，供 CRC、Gold 序列、QPSK 参考信号、功率控制、测量量、调制、加扰、SC-FDMA 与 ZC 序列等模块重复调用。

## 目录结构

| 文件 | 对应协议节 |
|------|-----------|
| `slb_phy_types.h` | 公共类型与错误码 |
| `slb_phy_common.c/.h` | 6.5.1 CRC、6.5.2 Gold、6.5.3 QPSK RS |
| `slb_phy_pwr_meas.c/.h` | 6.5.4 功控、6.5.5 测量量 |
| `slb_phy_mod.c/.h` | 6.5.6 QPSK～4096QAM |
| `slb_phy_scram.c/.h` | 6.5.7 比特加扰 |
| `slb_phy_scfdma.c/.h` | 6.5.8 SC-FDMA DFT 预编码 |
| `slb_phy_zc.c/.h` | 6.5.9 ZC 序列与组跳 |
| `slb_phy.h` | 统一 `#include` 入口 |
| `slb_phy_test.c` | 基础自测（含标准算例 c_init=344087） |

## 编译

需要 C99 编译器与数学库（`-lm`）：

```bash
cd E:/实习/AI代码/6.5物理层通用功能
make
make test
```

Windows 下可使用 MinGW-w64 或 MSYS2 中的 `gcc`。

## 使用示例

```c
#include "slb_phy.h"

/* Gold + QPSK 参考信号 */
slb_qpsk_cinit_param_t p = { .n = 0, .l = 0, .N_ID = 5, .N_CP = 3 };
slb_cplx_t r[161];
slb_qpsk_ref_generate(&(slb_qpsk_seq_t){ .param = p, .r = r, .r_cap = 161 });

/* 数据加扰 */
uint32_t c_init = slb_scram_cinit_data(n_f, n_g_pid);
slb_scram_apply(bits_in, bits_out, N_bit, c_init);

/* 64QAM 调制 */
slb_mod_map(bits, N_bit, SLB_MOD_64QAM, symbols, &n_sym);
```

## 关联文档

整合技术文档（tex + pdf）位于：

`C:\Users\CMDN\Desktop\暂存\20260701 update\SLB物理层6.5物理层通用功能技术文档（20260701）.tex/.pdf`
