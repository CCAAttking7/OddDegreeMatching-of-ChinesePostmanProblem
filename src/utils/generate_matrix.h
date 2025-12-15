#ifndef GENERATE_MATRIX_H
#define GENERATE_MATRIX_H

#include <vector>
#include <random>

using namespace std;

/**
 * 生成随机对称距离矩阵
 * @param n 矩阵维度（顶点数）
 * @param maxDist 最大距离值
 * @return n×n的对称距离矩阵，对角线为0
 */
inline vector<vector<long long>> generateRandomDistMatrix(int n, int maxDist = 100)
{
    vector<vector<long long>> dist(n, vector<long long>(n, 0));
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, maxDist);

    // 生成随机距离，并保证矩阵对称
    for (int i = 0; i < n; ++i)
    {
        for (int j = i + 1; j < n; ++j)
        {
            dist[i][j] = dis(gen);
            dist[j][i] = dist[i][j]; // 对称性保证
        }
    }
    return dist;
}

#endif // GENERATE_MATRIX_H