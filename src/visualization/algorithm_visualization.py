"""
algorithm_visualization_paper.py
论文级算法性能对比可视化（精简版）
"""

import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

# =========================
# 论文级统一风格
# =========================
plt.rcParams.update({
    "font.family": "sans-serif",
    "font.sans-serif": ["SimHei", "Arial Unicode MS", "DejaVu Sans"],
    "axes.unicode_minus": False,

    "font.size": 10,
    "axes.labelsize": 11,
    "axes.titlesize": 14,
    "legend.fontsize": 10,
    "xtick.labelsize": 10,
    "ytick.labelsize": 10,

    "lines.linewidth": 1.8,
    "lines.markersize": 5,
    "axes.linewidth": 1.0,
    "grid.alpha": 0.25,
})

# 低饱和、可黑白打印
COLORS = {
    '暴力搜索': '#444444',
    '状态压缩DP': '#1f77b4',
    '区间DP': '#2ca02c',
    'Blossom': '#9467bd',
}

MARKERS = {
    '暴力搜索': 'o',
    '状态压缩DP': 's',
    '区间DP': '^',
    'Blossom': 'D'
}


# =========================
# 数据加载
# =========================
def load_data(filename="algorithm_comparison.csv"):
    df = pd.read_csv(filename)
    df.replace(-1, np.nan, inplace=True)
    return df


# =========================
# 图1：时间复杂度对比（核心图）
# =========================
def fig_time_log(df):
    fig, ax = plt.subplots(figsize=(7.5, 5.2))

    algs = [
        ('暴力搜索', 'time_brute'),
        ('状态压缩DP', 'time_state_dp'),
        ('区间DP', 'time_interval_dp'),
        ('Blossom', 'time_blossom')
    ]

    for name, col in algs:
        d = df[df[col].notna()]
        ax.semilogy(
            d['n'], d[col],
            marker=MARKERS[name],
            label=name,
            color=COLORS[name],
            alpha=0.9
        )

    # 算法适用规模分界（极少、克制）
    ax.axvline(12, linestyle='--', linewidth=1, color='gray')
    ax.axvline(22, linestyle='--', linewidth=1, color='gray')

    ax.set_xlim(0, 520)
    ax.set_ylim(0.5, 1e8)

    ax.set_xlabel('奇度顶点数 n')
    ax.set_ylabel('运行时间（μs，对数尺度）')
    ax.set_title('不同算法时间性能对比')

    ax.grid(True, which='major')
    ax.grid(False, which='minor')

    ax.legend(frameon=True)

    plt.tight_layout()
    plt.savefig('Fig1_Time_Comparison_Log.png', dpi=300)
    plt.show()


# =========================
# 图2：小规模精细对比
# =========================
def fig_small_scale(df):
    fig, ax = plt.subplots(figsize=(7.5, 5.2))

    d = df[df['n'] <= 22]

    for name, col in [
        ('暴力搜索', 'time_brute'),
        ('状态压缩DP', 'time_state_dp'),
        ('区间DP', 'time_interval_dp'),
    ]:
        dd = d[d[col].notna()]
        ax.plot(
            dd['n'], dd[col],
            marker=MARKERS[name],
            label=name,
            color=COLORS[name]
        )

    ax.set_xlabel('奇度顶点数 n')
    ax.set_ylabel('运行时间（μs）')
    ax.set_title('小规模下算法性能对比（n ≤ 22）')

    ax.grid(True)
    ax.legend()

    plt.tight_layout()
    plt.savefig('Fig2_Small_Scale_Comparison.png', dpi=300)
    plt.show()


# =========================
# 图3：多项式算法对比
# =========================
def fig_polynomial(df):
    fig, ax = plt.subplots(figsize=(7.5, 5.2))

    d = df[df['n'] >= 20]

    ax.plot(
        d['n'], d['time_interval_dp'],
        marker='^', label='区间DP',
        color=COLORS['区间DP']
    )
    ax.plot(
        d['n'], d['time_blossom'],
        marker='D', label='Blossom',
        color=COLORS['Blossom']
    )

    ax.set_xlabel('奇度顶点数 n')
    ax.set_ylabel('运行时间（μs）')
    ax.set_title('多项式时间算法性能对比')

    ax.grid(True)
    ax.legend()

    plt.tight_layout()
    plt.savefig('Fig3_Polynomial_Comparison.png', dpi=300)
    plt.show()


# =========================
# 图4：内存使用对比
# =========================
def fig_memory(df):
    fig, ax = plt.subplots(figsize=(7.5, 5.2))

    for name, col in [
        ('状态压缩DP', 'memory_state_dp'),
        ('区间DP', 'memory_interval_dp'),
        ('Blossom', 'memory_blossom')
    ]:
        d = df[df[col].notna()]
        ax.loglog(
            d['n'], d[col],
            marker=MARKERS[name],
            label=name,
            color=COLORS[name]
        )

    ax.set_xlabel('奇度顶点数 n')
    ax.set_ylabel('内存使用（KB，对数尺度）')
    ax.set_title('算法内存开销对比')

    ax.grid(True, which='major')
    ax.grid(False, which='minor')
    ax.legend()

    plt.tight_layout()
    plt.savefig('Fig4_Memory_Comparison.png', dpi=300)
    plt.show()


# =========================
# 主流程
# =========================
def main():
    df = load_data()

    fig_time_log(df)
    fig_small_scale(df)
    fig_polynomial(df)
    fig_memory(df)

    print("图表生成完成!")


if __name__ == "__main__":
    main()
