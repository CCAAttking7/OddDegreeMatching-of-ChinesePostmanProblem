/*
暴力搜索（Brute-Force Search）求解最小权完美匹配:

算法思想：
1. 枚举所有可能的完美匹配方案：对于m个顶点，所有完美匹配的数量为双阶乘 (m-1)!!。
2. 顶点排列生成：生成所有顶点的排列，将每个排列顺序解读为两两配对。
   例如排列 (a,b,c,d) 对应匹配对 (a,b) 和 (c,d)。
3. 成本计算：对于每个排列，按顺序将顶点两两配对，累加配对顶点间的距离作为该匹配方案的总成本。
4. 最优解更新：记录所有匹配方案中的最小成本。

优化策略：
为避免因顶点顺序不同而重复计算相同的匹配集合，算法固定第一个顶点（固定为0），
仅对其余 m-1 个顶点进行排列，将枚举量从 m! 减少到 (m-1)!。

时间复杂度：O((m-1)!! × m)，空间复杂度：O(m)，其中m为顶点数。
适用于：极小规模（m ≤ 12）问题的精确求解，主要作为验证其他算法正确性的基准。
*/

#ifndef BRUTE_FORCE_H
#define BRUTE_FORCE_H

#include <vector>
#include <algorithm>
#include <climits>
#include <iostream>

/**
 * 暴力搜索求解最小权完美匹配
 * @param n 奇数度顶点个数（必须为偶数）
 * @param dist 距离矩阵，dist[i][j]表示顶点i到顶点j的距离
 * @return 最小匹配权和，如果顶点数为奇数返回-1
 */
inline long long bruteForceMatching(int n, const std::vector<std::vector<long long>> &dist)
{
    // 输入验证：完美匹配要求顶点数为偶数
    if (n % 2 == 1)
    {
        std::cerr << "错误：奇数度顶点个数必须为偶数！" << std::endl;
        return -1;
    }

    // 初始化顶点序列：0, 1, 2, ..., n-1
    std::vector<int> vertices(n);
    for (int i = 0; i < n; ++i)
        vertices[i] = i;

    long long minCost = LLONG_MAX; // 初始化为最大值，便于后续更新

    // 核心枚举循环：生成所有可能的匹配方案
    // 固定第一个顶点为0，仅排列剩余的 n-1 个顶点，避免重复计算相同匹配
    do
    {
        long long currentCost = 0;
        // 将当前排列顺序解读为两两配对：vertices[0]&[1], vertices[2]&[3], ...
        for (int i = 0; i < n; i += 2)
        {
            int u = vertices[i];
            int v = vertices[i + 1];
            currentCost += dist[u][v]; // 累加当前配对的距离
        }
        // 更新全局最小成本
        if (currentCost < minCost)
            minCost = currentCost;
    } while (std::next_permutation(vertices.begin() + 1, vertices.end()));
    // ^ 关键优化：vertices.begin() + 1 表示固定第一个顶点

    return minCost;
}

/**
 * 计算双阶乘 (n-1)!! = (n-1) × (n-3) × ... × 1
 * @param n 偶数
 * @return (n-1)!! 的值
 */
inline long long doubleFactorial(int n)
{
    long long result = 1;
    for (int i = n - 1; i > 0; i -= 2)
    {
        result *= i;
    }
    return result;
}

#endif // BRUTE_FORCE_H