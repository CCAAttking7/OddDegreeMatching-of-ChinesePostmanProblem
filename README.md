中国邮递员问题奇点匹配算法实现
本项目系统实现了中国邮递员问题中核心子问题——奇度顶点最小权完美匹配的四种精确算法，涵盖从暴力搜索到多项式时间算法的完整渐进式优化路径。

算法实现

1. 暴力搜索算法
   时间复杂度：O((n-1)!!)
   适用规模：n ≤ 12
   特点：逻辑简单，作为正确性验证的黄金标准
   文件：src/algorithms/brute_force.h

2. 状态压缩动态规划
   时间复杂度：O(n²·2ⁿ)
   适用规模：12 < n ≤ 22
   特点：将状态编码为二进制位，显著提升中小规模问题的求解效率
   文件：src/algorithms/state_dp.h

3. 区间动态规划
   时间复杂度：O(n³)
   适用规模：n ≤ 1000+ (需满足欧氏距离条件)
   特点：利用平面点集"无交叉边"性质，针对欧氏距离问题高度优化
   文件：src/algorithms/interval_dp.h

4. Blossom 算法 (基于 LEMON 库)
   时间复杂度：O(n³)
   适用规模：n ≤ 2000+
   特点：通用精确算法，适用于任意图结构，基于 Edmonds 开花算法
   文件：src/algorithms/blossom_lemon.h

项目结构：
├── src/
│ ├── algorithms/ # 核心算法实现
│ │ ├── brute_force.h
│ │ ├── state_dp.h
│ │ ├── interval_dp.h
│ │ └── blossom_lemon.h
│ └── utils/ # 工具函数
│ ├── generate_data.h
│ ├── generate_matrix.h
│ └── generate_point.h
| └──tests/ # 测试程序
│ ├── test_brute.cpp
│ ├── test_state_dp.cpp
│ ├── test_interval_dp.cpp
│ ├── test_blossom.cpp
│ └── test_comprehensive.cpp
| └── visualization/ #数据可视化
| ├── algorithm_comparison.csv
| ├── algorithm_visualization.py
| ├── Fig1_Time_Comparison_Log.png
| |── Fig2_Small_Scale_Comparison.png
| ├── Fig3_Polynomial_Comparison.png
| └── Fig4_Memory_Comparison.png

快速开始

环境要求
C++17 兼容编译器 (GCC/Clang/MSVC)
LEMON 图论库 (版本 1.4+)
Python 3.8+ (仅用于可视化)

编译示例
bash

# 编译测试程序

g++ -std=c++17 -O2 tests/test_comprehensive.cpp -I src -I [LEMON_INCLUDE_PATH] -L [LEMON_LIB_PATH] -lemon -o test_comprehensive

# 运行测试

./test_comprehensive

算法性能对比
根据实际测试数据（见 docs/figures/），各算法性能边界如下：
算法 时间复杂度 实用规模 n=10 耗时 n=100 耗时
暴力搜索 O((n-1)!!) n ≤ 12 1,484 μs 不可行
状态压缩 DP O(n²·2ⁿ) n ≤ 22 18 μs 不可行
区间 DP O(n³) n ≤ 1000+ 5 μs 122 μs
Blossom O(n³) n ≤ 2000+ 71 μs 7,593 μs

算法选择建议
基于问题特征选择合适算法：
验证基准：n ≤ 12 时使用暴力搜索验证正确性
中小规模通用图：12 < n ≤ 22 时使用状态压缩 DP
大规模欧氏距离问题：n > 22 且满足几何性质时使用区间 DP（首选）
大规模一般图问题：n > 22 且不满足特殊结构时使用 Blossom 算法

实验环境
处理器：Intel Core Ultra 5 125H
内存：16 GB DDR4 5600 MT/s
操作系统：Windows 11 Professional 22H2
编译器：GCC 15.2.0 (MinGW-w64)
编译选项：-O2 优化级别

参考文献
1.J. Edmonds & E. L. Johnson, "Matching, Euler tours and the Chinese postman", Mathematical Programming, 1973
2.M. Held & R. M. Karp, "A dynamic programming approach to sequencing problems", Journal of SIAM, 1962
3.J. Edmonds, "Paths, trees, and flowers", Canadian Journal of Mathematics, 1965

详细引用见论文，暂未上传。

许可证
本项目采用 MIT 许可证。详见 LICENSE 文件。
