/*
状态压缩动态规划（State Compression DP）求解最小权完美匹配:

算法思想：
1. 状态编码：使用一个m位的二进制掩码（mask）表示当前匹配状态。
   第i位为1表示第i个顶点已被匹配，为0表示未匹配。
2. 状态定义：dp[mask] 表示达到状态mask（即已匹配mask所指示的顶点集合）所需的最小匹配成本。
3. 状态转移：对于每个状态mask，找到第一个未被匹配的顶点i。为避免重复计数，固定i必须参与下一次匹配。
   然后枚举另一个未被匹配的顶点j，将i与j配对，得到新状态newMask = mask | (1<<i) | (1<<j)。
   转移方程：dp[newMask] = min(dp[newMask], dp[mask] + dist[i][j])。
4. 计算顺序：从空集（mask=0）开始，逐步增加已匹配顶点数，直到所有顶点都被匹配（mask=(1<<m)-1）。

时间复杂度：O(m² * 2^m)，空间复杂度：O(2^m)，其中m为顶点数。
适用于：小到中等规模（m ≤ 20）的精确求解，可作为暴力算法的优化版本。
*/

#ifndef STATE_DP_H
#define STATE_DP_H

#include <vector>
#include <climits>
#include <iostream>

/**
 * 状态压缩DP求解最小权完美匹配
 * @param m 奇数度顶点个数
 * @param dist 距离矩阵，dist[i][j]表示顶点i到顶点j的最短路径长度
 * @return 最小匹配权和，如果顶点数为奇数返回-1
 */
inline long long stateDPMatching(int m, const std::vector<std::vector<long long>> &dist)
{
    if (m % 2 == 1)
        return -1;
    int fullMask = (1 << m) - 1; // 所有顶点都匹配的状态：二进制有m个1
    // DP数组，dp[mask]表示达到状态mask的最小成本，初始时除空状态外都设为无穷大
    std::vector<long long> dp(1 << m, LLONG_MAX);
    dp[0] = 0; // 初始状态：没有顶点被匹配，成本为0

    // 遍历所有可能的状态（从空集到全集）
    for (int mask = 0; mask <= fullMask; ++mask)
    {
        // 如果当前状态不可达（尚未计算出合法成本），则跳过
        if (dp[mask] == LLONG_MAX)
            continue;
        // 找到第一个未被匹配的顶点i（mask中最低位的0）
        int i = 0;
        while (i < m && (mask & (1 << i)))
            i++;
        // 理论上此时i应该小于m，因为mask不是全1状态
        if (i >= m)
            continue;
        // 尝试将顶点i与所有未被匹配的顶点j进行配对
        for (int j = i + 1; j < m; ++j)
        {
            // 如果顶点j已被匹配，跳过
            if (mask & (1 << j))
                continue;
            // 新状态：在原状态基础上加入顶点i和j的匹配
            int newMask = mask | (1 << i) | (1 << j);
            long long newCost = dp[mask] + dist[i][j];
            // 如果找到更优解，更新DP表
            if (newCost < dp[newMask])
            {
                dp[newMask] = newCost;
            }
        }
    }
    // 返回所有顶点都匹配时的最小成本
    return dp[fullMask];
}

#endif // STATE_DP_H