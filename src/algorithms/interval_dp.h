/*
区间动态规划求解欧氏平面点集的最小权完美匹配:

核心前提是保证最优匹配的边不会交叉：满足以下任一条件即可保证“无交叉”
(1)点在欧式平面，边权为欧氏距离
(2)点在任意度量空间，且距离函数满足三角不等式
(3)点在一棵树上，边权为树上路径长度
(4)所有点位于一条直线上

算法思想：
1.理论基础：基于"欧氏平面最优匹配无交叉边"定理。将已排序的点序列中，
  任何最优匹配都可以被最左侧点所在的匹配边划分为三个独立部分。
2.状态定义：dp[l][r] 表示将排序后点序列中索引 [l, r] 区间内的所有点
  进行完美匹配的最小总欧氏距离。
3.核心策略：区间DP，自底向上。固定区间最左侧点 l，枚举其匹配点 k，
  则区间被划分为：
       - 匹配边 (l, k)
       - 左子区间 [l+1, k-1]（需独立完美匹配）
       - 右子区间 [k+1, r]（需独立完美匹配）
  总成本为这三部分成本之和，取所有 k 中的最小值。
4.关键优化：k 的枚举步长为 2 (k += 2)，保证子区间点数为偶数（可匹配）。

时间复杂度：O(m^3)，空间复杂度：O(m^2)，其中 m 为点数。
适用于：点集位于欧氏平面或满足三角不等式的度量空间。
*/

#ifndef INTERVAL_DP_H
#define INTERVAL_DP_H

#include <vector>
#include <cmath>
#include <algorithm>
#include <limits>

// 维护一个点的坐标
struct Point
{
    double x, y;
    Point(double x = 0, double y = 0) : x(x), y(y) {}
};

/**
 * 计算两点间欧式距离
 */
inline double euclideanDist(const Point &a, const Point &b)
{
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return std::sqrt(dx * dx + dy * dy);
}

/**
 * 核心区间DP函数，返回匹配成本
 * @param points 已排序的点集（按x坐标，然后y坐标）
 * @return 最小匹配成本，如果点数为奇数返回-1.0
 */
inline double intervalDPMatching(const std::vector<Point> &points)
{
    int m = points.size();
    if (m % 2 != 0)
        return -1.0;

    // 预计算并存储所有点对之间的欧氏距离，用空间换时间，便于后续查找dist[l][k]
    std::vector<std::vector<double>> dist(m, std::vector<double>(m, 0.0));
    for (int i = 0; i < m; ++i)
        for (int j = i + 1; j < m; ++j)
            dist[i][j] = dist[j][i] = euclideanDist(points[i], points[j]);

    // 建立DP表，dp[l][r]表示points[l]到points[r]这个连续区间内所有点的最小成本
    std::vector<std::vector<double>> dp(m, std::vector<double>(m,
                                                               std::numeric_limits<double>::max()));

    // 初始化：只有两个点时，直接匹配
    for (int i = 0; i < m - 1; i += 2)
        dp[i][i + 1] = dist[i][i + 1];

    // 状态转移核心
    // 外层逻辑，按照区间长度len从小到大遍历，+2因为数量必须是偶数；
    // 必须先算好所有小区间的答案才能计算包含他们的大区间
    for (int len = 4; len <= m; len += 2)
    {
        // 中层逻辑，遍历每一个长度为len的的区间的起点l，确定终点r，获得dp[l][r]
        for (int l = 0; l + len - 1 < m; ++l)
        {
            int r = l + len - 1;
            double min_cost = std::numeric_limits<double>::max();

            // 内层逻辑，枚举点l的匹配对象k，k+=2保证k-l+1是奇数，保证 [l+1, k-1]是偶数个
            for (int k = l + 1; k <= r; k += 2)
            {
                double cost = dist[l][k];
                if (l + 1 <= k - 1)
                    cost += dp[l + 1][k - 1];
                if (k + 1 <= r)
                    cost += dp[k + 1][r];
                if (cost < min_cost)
                    min_cost = cost;
            }
            dp[l][r] = min_cost;
        }
    }

    // 返回结果
    return dp[0][m - 1];
}

#endif // INTERVAL_DP_H