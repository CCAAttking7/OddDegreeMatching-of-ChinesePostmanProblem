/*
generate_data.h - 统一测试数据生成接口

功能：整合现有的数据生成函数，提供统一的测试数据接口
*/

#ifndef GENERATE_DATA_H
#define GENERATE_DATA_H

#include "generate_matrix.h" // 距离矩阵生成
#include "generate_point.h"  // 点集生成
#include <vector>
#include <algorithm>

using namespace std;

/**
 * 将点集转换为距离矩阵
 * @param points 排序后的点集
 * @return n×n的欧氏距离矩阵
 */
inline vector<vector<long long>> pointsToMatrix(const vector<Point> &points)
{
    int n = points.size();
    vector<vector<long long>> dist(n, vector<long long>(n, 0));

    for (int i = 0; i < n; ++i)
    {
        for (int j = i + 1; j < n; ++j)
        {
            // 计算欧氏距离并取整（保持与整数算法兼容）
            double dx = points[i].x - points[j].x;
            double dy = points[i].y - points[j].y;
            long long d = static_cast<long long>(sqrt(dx * dx + dy * dy) * 1000); // 放大1000倍保留精度
            dist[i][j] = d;
            dist[j][i] = d;
        }
    }
    return dist;
}

/**
 * 生成固定种子的测试数据（确保可重复性）
 * @param n 顶点数
 * @param use_euclidean 是否生成欧氏点集（true）或一般距离矩阵（false）
 * @return 统一格式的测试数据
 */
inline vector<vector<long long>> generateTestData(int n, bool use_euclidean = true)
{
    if (use_euclidean)
    {
        // 生成欧氏点集并转换为距离矩阵
        auto points = generateSortedPoints(n);
        return pointsToMatrix(points);
    }
    else
    {
        // 生成一般距离矩阵
        return generateRandomDistMatrix(n);
    }
}

/**
 * 生成特定类型的距离矩阵
 * @param n 顶点数
 * @param matrix_type 矩阵类型：
 *                   0: 欧氏距离矩阵（满足三角不等式）
 *                   1: 一般随机距离矩阵
 *                   2: 稀疏距离矩阵（部分边为极大值）
 * @return 距离矩阵
 */
inline vector<vector<long long>> generateCustomMatrix(int n, int matrix_type = 0)
{
    switch (matrix_type)
    {
    case 0:
    { // 欧氏距离矩阵
        auto points = generateSortedPoints(n);
        return pointsToMatrix(points);
    }
    case 1: // 一般随机距离矩阵
        return generateRandomDistMatrix(n);
    case 2:
    { // 稀疏距离矩阵（约50%的边）
        auto dist = generateRandomDistMatrix(n);
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(0, 1);

        for (int i = 0; i < n; ++i)
        {
            for (int j = i + 1; j < n; ++j)
            {
                if (dis(gen) == 0)
                { // 50%概率设置为极大值
                    dist[i][j] = 1e9;
                    dist[j][i] = 1e9;
                }
            }
        }
        return dist;
    }
    default:
        return generateRandomDistMatrix(n);
    }
}

/**
 * 验证距离矩阵是否满足三角不等式
 * @param dist 距离矩阵
 * @return 是否满足三角不等式
 */
inline bool satisfiesTriangleInequality(const vector<vector<long long>> &dist)
{
    int n = dist.size();
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            for (int k = 0; k < n; ++k)
            {
                if (dist[i][j] > dist[i][k] + dist[k][j])
                {
                    return false;
                }
            }
        }
    }
    return true;
}

#endif // GENERATE_DATA_H