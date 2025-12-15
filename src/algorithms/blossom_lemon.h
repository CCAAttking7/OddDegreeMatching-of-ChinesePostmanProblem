/*
基于LEMON库的Blossom算法实现最小权完美匹配

算法背景：
1. 理论依据：Edmonds' Blossom算法，解决一般图上的最大权完美匹配问题
2. 库依赖：LEMON (Library for Efficient Modeling and Optimization)
3. 算法思想：通过寻找"花"（blossom）并进行收缩/展开操作，将一般图匹配问题
   转化为二分图匹配问题的扩展
4. 时间复杂度：O(n³)，多项式时间算法

实现细节：
1. 最小权转换：通过取负将最小权问题转化为最大权问题
2. 完全图构造：将所有奇点构建为完全图，边权为最短路径距离
3. 精确求解：返回精确的最小权完美匹配解
*/
// 添加在blossom_lemon.h最顶部
#ifndef LEMON_WIN32_THREADS
#define LEMON_WIN32_THREADS 0
#endif

#ifndef LEMON_HAVE_PTHREAD
#define LEMON_HAVE_PTHREAD 0
#endif
#ifndef BLOSSOM_LEMON_H
#define BLOSSOM_LEMON_H

#include <vector>
#include <iostream>

// LEMON库头文件
#include <lemon/list_graph.h>
#include <lemon/matching.h>

/**
 * 使用LEMON Blossom算法求解最小权完美匹配
 * @param n 顶点数（必须为偶数）
 * @param dist 距离矩阵，dist[i][j]表示顶点i到顶点j的距离
 * @return 最小匹配成本，如果无法找到完美匹配返回-1
 */
inline long long blossomMatching(int n, const std::vector<std::vector<long long>> &dist)
{
    if (n % 2 == 1)
    {
        std::cerr << "错误：顶点数必须为偶数！" << std::endl;
        return -1;
    }

    // 1. 创建图
    lemon::ListGraph g;
    lemon::ListGraph::EdgeMap<long long> weight(g);
    std::vector<lemon::ListGraph::Node> nodes;

    // 添加顶点
    for (int i = 0; i < n; ++i)
    {
        nodes.push_back(g.addNode());
    }

    // 添加边（完全图）
    for (int i = 0; i < n; ++i)
    {
        for (int j = i + 1; j < n; ++j)
        {
            lemon::ListGraph::Edge e = g.addEdge(nodes[i], nodes[j]);
            weight[e] = dist[i][j];
        }
    }

    // 2. 创建最大权匹配对象（最小权问题取负）
    lemon::ListGraph::EdgeMap<long long> negWeight(g);
    for (lemon::ListGraph::EdgeIt e(g); e != lemon::INVALID; ++e)
    {
        negWeight[e] = -weight[e];
    }

    // 3. 运行Blossom算法
    lemon::MaxWeightedPerfectMatching<lemon::ListGraph, lemon::ListGraph::EdgeMap<long long>>
        blossom(g, negWeight);

    bool success = blossom.run();

    if (!success)
    {
        std::cerr << "警告：未找到完美匹配" << std::endl;
        return -1;
    }

    // 4. 计算总成本
    long long totalCost = 0;
    for (lemon::ListGraph::EdgeIt e(g); e != lemon::INVALID; ++e)
    {
        if (blossom.matching(e))
        {
            totalCost += weight[e];
        }
    }

    return totalCost;
}

#endif // BLOSSOM_LEMON_H