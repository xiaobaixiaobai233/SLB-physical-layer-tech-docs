# SLB 6.2.2 基础帧结构和物理资源 — C 参考实现

**airMode = slb** · 命名遵循 [nearlink-naming](../../../../../.claude/skills/nearlink-naming/SKILL.md)

依据工作区《SLB物理层6.2.2基础帧结构和物理资源技术文档》及 T/XS 10001-2025 第 6.2.2 节，将时频资源坐标系的查表、校验与接收侧定位流程实现为可编译 C 模块，供 SLB 120 kHz 物理层实现与联调参考。

> 本仓库**不包含** OFDM 调制（6.2.9）、PIB 比特编解码（6.2.4/6.2.5）、信号序列生成（6.2.3）及 MAC 调度策略。

---

## 目录结构

```text
nearlink-6.2.2/slb/
├── include/          公共头文件（Slb* 类型 + slb* API）
├── src/              实现文件（一文件对应一个 6.2.2 子节或流程）
├── test/             6.2.2 标准符合性 demo（验证用）
├── example/          演示程序
├── Makefile
└── README.md
```

---

## 编译与运行

### 方式 A：CMake + Visual Studio（推荐）

**1. 先编译**（Cursor 底部 CMake 面板点 Build，或终端）：

```powershell
cd nearlink-6.2.2\slb
& "D:\Microsoft Visual Studio\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build build --config Debug
```

**2. 直接运行 demo 测试**（无需 ctest）：

```powershell
.\build\Debug\slb_test_runner.exe
```

看到 `ALL DEMOS PASSED` 即表示 6.2.2 验证通过。

**3. 可选：用 ctest**（需写全路径，且当前目录在 `slb/`）：

```powershell
& "D:\Microsoft Visual Studio\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\ctest.exe" --test-dir build -C Debug --output-on-failure
```

> 注意：你之前在 `slb/` 目录下用了 `ctest --test-dir slb/build`，路径多了一层 `slb/`；应改为 `--test-dir build`。

### 方式 B：Makefile

```bash
cd nearlink-6.2.2/slb
make test         # 编译并运行全部 demo
make example      # 仅 API 演示
```

---

## 标准符合性 Demo（`test/`）

依据 T/XS 10001-2025 **§6.2.2** 各表与强制约束编写，用于验证实现是否与标准一致。

| Demo 文件 | 验证内容（标准条款） |
|-----------|---------------------|
| `demo-6221-cp-ofdm-symbol.c` | **6.2.2.1** 表 1 CP/GAP/边界符号长；表 2 每帧符号数；表 3 边界 CP 位置；6.5.3 N_CP 索引 |
| `demo-6222-superframe.c` | **6.2.2.2** Tf=30720Ts；8 无线帧；三级索引合法性；G/T/A/D 符号枚举 |
| `demo-6223-cot-tti.c` | **6.2.2.3** TTI=2^N_TTI；gLink 在 TTI 开头、末符号 GAP；COT 内 TTI 范围计算 |
| `demo-6224-carrier.c` | **6.2.2.4** 表 4 全 18 档 N_CH/L_CH；DC #80 不可映射；子载波组编号 |
| `demo-6225-resource-grid.c` | **6.2.2.5~7** 同天线端口；161×14 网格；a_{80,l}≡0；RE 映射/拒绝 |
| `demo-6228-comm-domain-link.c` | **6.2.2.8~10** 通信域非空；g/t/auxLink 符号；passthroughLink D 符号 |
| `demo-rx-resource-locate.c` | 接收侧 Step 1~6 完整流程（符号类型→边界→TTI→链路→载波→RE） |
| `slb-test-runner.c` | 总入口，汇总 PASS/FAIL，退出码 0=全通过 |

运行成功示例：

```text
ALL DEMOS PASSED
```

任一断言失败会打印 `FAIL:` 行并以非零退出码结束。

---

## 文件说明

### 头文件（`include/`）

| 文件 | 对应标准 | 作用 |
|------|----------|------|
| `slb-phy-constants.h` | 6.1 / 6.2.2 基本量 | 全局常量：`SLB_SAMPLE_RATE_HZ`、`SLB_SUBCARRIER_SPACING_HZ`、`SLB_SUPERFRAME_DURATION_TS`、161 子载波、DC #80 等 |
| `slb-cp-ofdm-symbol.h` | **6.2.2.1** | `SlbCpOfdmSymbolType` 五种符号类型；表 1 时域参数；表 2 每帧符号数；表 3 边界 CP；6.5.3 `N_CP` 索引 |
| `slb-superframe.h` | **6.2.2.2** | 超帧/无线帧时间坐标 `SlbSuperframeIndex`；G/T/A/D/GAP 符号类型 `SlbLinkSymbolKind` |
| `slb-cot-tti.h` | **6.2.2.3** | `SlbChannelOccupancyTime`（COT）、`SlbTransmissionTimeInterval`（TTI）；`2^N_TTI` 计算；通信域 TTI 结构校验 |
| `slb-carrier.h` | **6.2.2.4** | 表 4 `N_CH`/`L_CH` 配对；DC 子载波可映射性；基础子载波组编号 |
| `slb-antenna-port.h` | **6.2.2.5** | 逻辑天线端口；SAB/G-PCIB/广播/G-DMRS-C 同端口校验 |
| `slb-resource-grid.h` | **6.2.2.6** | `SlbResourceGrid`（161 × N_sym^frame）；初始化、清零、读 RE |
| `slb-resource-element.h` | **6.2.2.7** | RE 索引 `(k,l)_p`；`a_{k,l}` 映射；`a_{80,l}≡0` 强制 |
| `slb-comm-domain.h` | **6.2.2.8** | gNode 通信域资源范围（SAB、G-PCIB、广播、可调度资源） |
| `slb-link.h` | **6.2.2.9** | gLink / tLink / auxLink 逻辑链路与 G/T/A 符号对应 |
| `slb-passthrough-link.h` | **6.2.2.10** | passthroughLink；先发/后发节点；D 符号校验 |
| `slb-rx-resource-locate.h` | 技术文档 §处理步骤 | 接收侧资源定位 Step 1~6 流水线 API |

### 源文件（`src/`）

| 文件 | 实现要点 |
|------|----------|
| `slb-cp-ofdm-symbol.c` | 表 1/2/3 静态查表；`slbSelectSymbolTimingParams`、`slbValidateIsBoundarySymbol`、`slbSelectNcPIndex` |
| `slb-superframe.c` | 超帧 1 ms 时长；超帧/无线帧/符号三级索引合法性校验 |
| `slb-cot-tti.c` | TTI 无线帧数；COT 内 TTI 范围计算；gLink 在 TTI 开头、末符号为 GAP 校验 |
| `slb-carrier.c` | 表 4 全 18 档 N_CH→L_CH；工作子载波组上界；k→子载波组映射（跳过 DC） |
| `slb-antenna-port.c` | 通信域四路内容同端口一致性检查 |
| `slb-resource-element.c` | RE  flat 索引；映射/校验；DC 列强制清零 |
| `slb-resource-grid.c` | 网格初始化（按符号类型定列数）；`memset` 清零 + DC 约束 |
| `slb-comm-domain.c` | 通信域非空校验 |
| `slb-link.c` | 逻辑链路 → G/T/A 符号；通信域符号合法性 |
| `slb-passthrough-link.c` | D/GAP 符号校验；COT 非空校验 |
| `slb-rx-resource-locate.c` | 六步接收定位：符号数→边界 CP→TTI 范围→链路序列→载波选择→RE 校验 |

### 示例（`example/`）

| 文件 | 作用 |
|------|------|
| `slb-phy-example.c` | 演示类型一 A 参数查表、TTI 计算、表 4 查 L_CH、资源网格 RE 映射、接收 Step 1 |

---

## 命名约定（nearlink-naming, airMode=slb）

| 类别 | 规则 | 示例 |
|------|------|------|
| 类型/结构体 | `Slb` + PascalCase | `SlbResourceGrid`, `SlbCpOfdmSymbolType` |
| 函数 | `slb` + 固定动词 + 语义名 | `slbCalculateTtiRadioFrameCount`, `slbValidateResourceElementIndex` |
| 常量 | `SLB_` + UPPER_SNAKE | `SLB_DC_SUBCARRIER_INDEX` |
| 协议术语 | SLB 官方词，不用泛化同义词 | gLink / tLink / auxLink / passthroughLink / resourceGrid / resourceElement |
| 带单位变量 | 后缀单位 | `sampleRateHz`, `cpLengthTs`, `nodeChannelCount`（N_CH 无量纲计数例外） |
| 公式局部变量 | 仅短函数内可用 | `k`, `l` 在 RE 映射循环中 |

---

## 主要 API 速查

```c
/* 6.2.2.1 符号参数 */
slbSelectSymbolTimingParams(SLB_CP_OFDM_SYMBOL_TYPE_1A, &timingParams);
slbCalculateSymbolCountPerRadioFrame(symbolType);
slbValidateIsBoundarySymbol(symbolType, symbolIndex);

/* 6.2.2.3 TTI */
slbCalculateTtiRadioFrameCount(ttiExponent);  /* 2^N_TTI */

/* 6.2.2.4 载波 */
slbSelectSubcarrierGroupMergeCount(nodeChannelCount, &mergeCount);
slbValidateMappableSubcarrierIndex(subcarrierIndex);

/* 6.2.2.6/7 资源网格 */
slbInitResourceGrid(&grid, buffer, symbolType, portIndex, carrierIndex);
slbMapResourceElementValue(grid.values, 161, symbolCount, &reIndex, value);

/* 接收定位 */
slbRxLocateStep1SelectSymbolCount(symbolType, &rxContext);
/* ... Step 2 ~ 6 ... */
```

---

## 上下游关系

| 方向 | 章节 | 与本模块关系 |
|------|------|--------------|
| 上游 | 6.1 | `Ts`、`fs` 基准 → `slb-phy-constants.h` |
| 上游 | 6.2.1 / MAC | 符号类型、N_TTI、N_CH/L_CH 配置输入 |
| 下游 | 6.2.3 | FTS/STS/DMRS 映射到 `SlbResourceElementIndex` |
| 下游 | 6.2.4/6.2.5 | PIB 块内符号 # 索引依赖帧结构 |
| 下游 | 6.5.3 | `slbSelectNcPIndex` 提供 N_CP 查表入口 |

---

## 约束提醒（实现时必须遵守）

1. **DC 子载波**：`k=80` 恒零，任何内容不得映射  
2. **表 4 配对**：`N_CH` 与 `L_CH` 必须联合校验  
3. **边界 CP**：严格按表 3，否则 6.5.3 符号定时不一致  
4. **通信域 TTI**：gLink 在开头，末符号为切换间隔  
5. **域不可混用**：通信域 G/T/A 配置不用于 passthroughLink D 符号场景  

---

## 参考文档

- 工作区：`SLB物理层6.2.2基础帧结构和物理资源技术文档.tex`
- 工作区：`星闪120kHz物理层6.2.2基础帧结构和物理资源-学习资料.tex`
- 标准：T/XS 10001-2025 第 6.2.2 节（20250326 版）

精确字段与图表以正式标准为准；本文档与代码仅供工程理解与联调参考。
