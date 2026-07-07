# -*- coding: utf-8 -*-
"""合并 6.5.1-6.5.9 四份分段技术文档为整体文档"""
from pathlib import Path

OUT_DIR = Path(r"C:\Users\CMDN\Desktop\暂存\20260701 update")
SOURCES = [
    (Path(r"C:\Users\CMDN\Desktop\暂存\20260701 update\6.5.1-6.5.3（20260701）.tex"), "6.5.1～6.5.3 共性基础算法"),
    (Path(r"C:\Users\CMDN\Desktop\暂存\20260701 update\6.5.4-6.5.5（20260701）.tex"), "6.5.4～6.5.5 功率控制与测量量"),
    (Path(r"E:\实习\workspace\整合文档\6.5物理层通用功能\6.5.6.tex"), "6.5.6 调制方式"),
    (Path(r"C:\Users\CMDN\Desktop\暂存\20260701 update\6.5.7-6.5.9（20260701）.tex"), "6.5.7～6.5.9 比特加扰、波形与序列"),
]

PREAMBLE = r"""% !TEX program = xelatex
\documentclass[11pt,a4paper]{ctexart}

\usepackage{amsmath,amssymb,amsfonts}
\usepackage{booktabs}
\usepackage{geometry}
\usepackage{hyperref}
\usepackage{enumitem}
\usepackage{xcolor}
\usepackage{listings}
\usepackage{longtable}

\geometry{left=2.5cm,right=2.5cm,top=2.5cm,bottom=2.5cm}
\hypersetup{colorlinks=true,linkcolor=blue,urlcolor=blue}

\lstset{
  basicstyle=\ttfamily\small,
  breaklines=true,
  frame=single,
  backgroundcolor=\color{gray!8},
  columns=fullflexible,
  keepspaces=true,
  showstringspaces=false
}

\newcommand{\dBm}{\,\mathrm{dBm}}
\newcommand{\dB}{\,\mathrm{dB}}
\newcommand{\NGPID}{N_{\mathrm{G-PID}}}

\title{%
  \textbf{星闪 SLB 物理层}\\[0.4em]
  \Large 6.5 物理层通用功能 技术文档\\[0.3em]
  \large 6.5.1\textasciitilde 6.5.9 完整实现参考\\[0.2em]
  \normalsize CRC \textbullet\ Gold/QPSK \textbullet\ 功控/测量 \textbullet\ 调制 \textbullet\ 加扰/SC-FDMA/ZC
}
\author{依据 T/XS 10001-2025《星闪无线通信系统 接入层\\
同步低时延宽带空口 SLB 技术要求和测试方法》整理\\
（团体标准 20250326 版）\\[0.5em]
代码目录：\texttt{E:\textbackslash 实习\textbackslash AI代码\textbackslash 6.5物理层通用功能}}
\date{2026年7月1日}

\begin{document}
\maketitle
\tableofcontents
\newpage

%======================================================================
\part{总览}
%======================================================================

\section{文档范围与模块划分}

本文档整合 SLB 120\,kHz 物理层协议第 \textbf{6.5 节``物理层通用功能''}全部子节（6.5.1\textasciitilde 6.5.9），为工程实现与联调提供统一的算法定义、接口说明与 C 语言参考实现映射。

\begin{table}[h]
\centering
\small
\begin{tabular}{llll}
\toprule
\textbf{节号} & \textbf{名称} & \textbf{C 模块} & \textbf{核心输出} \\
\midrule
6.5.1 & 循环冗余校验 CRC & \texttt{slb\_phy\_common} & 编码/校验比特 \\
6.5.2 & 伪随机 Gold 序列 & \texttt{slb\_phy\_common} & $c(i)$ \\
6.5.3 & 伪随机 QPSK 序列 & \texttt{slb\_phy\_common} & 161 点参考符号 \\
6.5.4 & 功率控制 & \texttt{slb\_phy\_pwr\_meas} & 发射功率 $P$ \\
6.5.5 & 测量量 & \texttt{slb\_phy\_pwr\_meas} & RSRP/RSSI/RSRQ/SINR/PMS \\
6.5.6 & 调制方式 & \texttt{slb\_phy\_mod} & $d(i)$ 符号 \\
6.5.7 & 比特加扰 & \texttt{slb\_phy\_scram} & $\tilde{t}_i$ \\
6.5.8 & SC-FDMA 波形 & \texttt{slb\_phy\_scfdma} & $y(n)$ \\
6.5.9 & ZC 序列/组跳 & \texttt{slb\_phy\_zc} & $r_{u,v}^{(\alpha)}(n)$ \\
\bottomrule
\end{tabular}
\end{table}

\section{PHY 发送/接收流水线}

\begin{itemize}[nosep]
  \item \textbf{控制信息（6.2.4）}：6.5.1 CRC $\rightarrow$ 6.2.6 极化编码 $\rightarrow$ 6.5.6 QPSK $\rightarrow$ 资源映射；
  \item \textbf{数据 TB（6.2.5）}：6.5.1 CRC $\rightarrow$ 6.2.6 编码 $\rightarrow$ 6.5.7 加扰 $\rightarrow$ 6.5.6 调制 $\rightarrow$ 6.5.4 功控 $\rightarrow$ 映射；
  \item \textbf{参考信号（6.2.3）}：6.5.2 Gold $\rightarrow$ 6.5.3 QPSK 或 6.5.9 ZC $\rightarrow$ 映射；
  \item \textbf{深覆盖（6.3.3）}：6.5.7 加扰 $\rightarrow$ 6.5.6 调制 $\rightarrow$ 6.5.8 SC-FDMA $\rightarrow$ 映射；DMRS 走 6.5.9。
\end{itemize}

\section{统一错误码}

\begin{table}[h]
\centering
\begin{tabular}{cll}
\toprule
\textbf{宏} & \textbf{值} & \textbf{含义} \\
\midrule
\texttt{SLB\_PHY\_OK} & 0 & 成功 \\
\texttt{SLB\_PHY\_ERR\_PARAM} & $-1$ & 空指针或非法参数 \\
\texttt{SLB\_PHY\_ERR\_BUF} & $-2$ & 输出缓冲区不足 \\
\texttt{SLB\_PHY\_ERR\_MEAS} / \texttt{SLB\_PHY\_ERR\_MODE} & $-3$ & 测量非法或模式错误 \\
\bottomrule
\end{tabular}
\end{table}

\section{工程文件结构}

\begin{lstlisting}
E:/实习/AI代码/6.5物理层通用功能/
  slb_phy.h                 # 统一入口
  slb_phy_types.h           # 公共类型
  slb_phy_common.c/.h       # 6.5.1-6.5.3
  slb_phy_pwr_meas.c/.h     # 6.5.4-6.5.5
  slb_phy_mod.c/.h          # 6.5.6
  slb_phy_scram.c/.h        # 6.5.7
  slb_phy_scfdma.c/.h       # 6.5.8
  slb_phy_zc.c/.h           # 6.5.9
  slb_phy_test.c            # 基础自测
  Makefile
\end{lstlisting}

\newpage
"""


def extract_body(tex: str) -> str:
    start = tex.find("\\begin{document}")
    end = tex.find("\\end{document}")
    if start < 0 or end < 0:
        raise ValueError("invalid tex")
    body = tex[start + len("\\begin{document}") : end]
    # 去掉子文档标题页与目录
    for marker in ("\\maketitle", "\\tableofcontents", "\\newpage"):
        while marker in body:
            idx = body.find(marker)
            line_end = body.find("\n", idx)
            body = body[:idx] + body[line_end + 1 :]
    return body.strip()


def main():
    parts = [PREAMBLE]
    for path, part_title in SOURCES:
        text = path.read_text(encoding="utf-8")
        body = extract_body(text)
        parts.append(f"\n%{'='*70}\n\\part{{{part_title}}}\n%{'='*70}\n\n")
        parts.append(body)
        parts.append("\n\\newpage\n")

    parts.append(r"""
\section{全模块 API 速查}

\begin{lstlisting}[language=C]
#include "slb_phy.h"

/* 6.5.1-6.5.3 */
slb_crc_encode(&enc); slb_crc_check(&chk);
slb_pn_gold_generate(&pn);
slb_qpsk_ref_generate(&qpsk);

/* 6.5.4-6.5.5 */
slb_pwr_calc_pathloss(...); slb_pwr_calc_initial(...);
slb_meas_rsrp(...); slb_meas_rsrq(...); slb_meas_sinr(...);

/* 6.5.6 */
slb_mod_map(bits, N_bit, SLB_MOD_64QAM, symbols, &N_sym);

/* 6.5.7-6.5.9 */
slb_scram_apply(in, out, N, c_init);
slb_scfdma_precode(x, y, M_symb, M_sc_total);
slb_zc_group_hop(n_s, n_id_x, &u);
slb_zc_gen(u, v, alpha, m, seq, M_rs);
\end{lstlisting}

\subsection{编译与测试}

\begin{lstlisting}[language=bash]
cd E:/实习/AI代码/6.5物理层通用功能
make test
\end{lstlisting}

\vfill
\begin{center}
\small\textcolor{gray}{精确参数与字段定义以 T/XS 10001-2025 正式标准为准；本文档仅供 SLB 物理层实现与联调参考。}
\end{center}

\end{document}
""")

    out_tex = OUT_DIR / "SLB物理层6.5物理层通用功能技术文档（20260701）.tex"
    out_tex.write_text("".join(parts), encoding="utf-8")
    print(f"Wrote {out_tex}")


if __name__ == "__main__":
    main()
