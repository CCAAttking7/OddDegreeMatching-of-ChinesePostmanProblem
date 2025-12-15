/*
状态压缩DP算法性能测试程序

测试目的：
1. 评估DP算法在不同规模下的时间与空间性能
2. 验证算法相对于暴力算法的效率提升
3. 提供算法适用性分析

测试规模：重点测试状态DP的优势区间（小到中等规模，m ≤ 22）
测试方法：重复多次取平均，减少计时波动
*/

#include "../algorithms/state_dp.h"
#include "../utils/generate_matrix.h"
#include <iostream>
#include <chrono>
#include <random>
#include <iomanip>
#include <vector>

using namespace std;
using namespace std::chrono;

/**
 * 统一测试函数：评估状态压缩DP算法在不同规模下的性能
 */
void runStateDPTest()
{
    cout << "=== 状态压缩DP算法性能评估 ===" << endl;
    cout << "测试目的：评估DP算法在不同规模下的时间与空间性能" << endl
         << endl;

    // 测试规模：重点测试状态DP的优势区间（小到中等规模）
    vector<int> test_sizes = {4, 6, 8, 10, 12, 14, 16, 18, 20, 22};
    const int REPEAT_TIMES = 5; // 重复次数，取平均以减少波动
    const int MAX_DIST = 50;

    cout << fixed << setprecision(2);
    cout << left << setw(8) << "规模(m)"
         << left << setw(18) << "状态数(2^m)"
         << left << setw(20) << "平均耗时(μs)"
         << left << setw(20) << "DP结果"
         << "性能评估" << endl;
    cout << string(85, '-') << endl;

    for (int m : test_sizes)
    {
        if (m % 2 != 0)
            continue;

        // 生成固定测试数据，确保测试的确定性
        mt19937 gen(m); // 用m作为随机种子，使每个规模的矩阵固定
        uniform_int_distribution<> dis(1, MAX_DIST);
        vector<vector<long long>> dist(m, vector<long long>(m, 0));
        for (int i = 0; i < m; ++i)
        {
            for (int j = i + 1; j < m; ++j)
            {
                dist[i][j] = dis(gen);
                dist[j][i] = dist[i][j];
            }
        }

        // 预热：避免冷启动影响
        dpMatching(m, dist);

        // 正式计时
        long long total_time = 0;
        long long result = 0;
        for (int r = 0; r < REPEAT_TIMES; ++r)
        {
            auto start = high_resolution_clock::now();
            result = dpMatching(m, dist);
            auto stop = high_resolution_clock::now();
            total_time += duration_cast<microseconds>(stop - start).count();
        }
        long long avg_time = total_time / REPEAT_TIMES;

        // 输出结果
        long long state_count = (1LL << m);
        cout << left << setw(8) << m;
        cout << left << setw(18) << state_count;
        cout << left << setw(20) << avg_time;
        cout << left << setw(20) << result;

        // 性能评估备注
        if (avg_time < 10)
        {
            cout << "极快";
        }
        else if (avg_time < 1000)
        {
            cout << "快速";
        }
        else if (avg_time < 10000)
        {
            cout << "适中";
        }
        else
        {
            cout << "较慢";
        }
        cout << endl;
    }

    // ========== 空间复杂度分析 ==========
    cout << endl
         << "=== 空间复杂度分析 ===" << endl;
    cout << "注：主要空间占用来自DP数组dp[1<<m]，每个元素为8字节（long long）。" << endl;
    cout << left << setw(10) << "规模(m)"
         << left << setw(15) << "DP数组大小"
         << left << setw(20) << "理论内存(KB)"
         << "内存评估" << endl;
    cout << string(65, '-') << endl;

    vector<int> mem_sizes = {14, 16, 18, 20, 22, 24};
    for (int m : mem_sizes)
    {
        long long array_size = (1LL << m);
        double memory_kb = (array_size * 8.0) / 1024.0; // 转换为KB
        cout << left << setw(10) << m
             << left << setw(15) << array_size
             << left << setw(20) << fixed << setprecision(1) << memory_kb;

        // 内存使用评估
        if (memory_kb < 100)
        {
            cout << "内存占用很小";
        }
        else if (memory_kb < 1024)
        {
            cout << "内存占用较小";
        }
        else if (memory_kb < 1024 * 10)
        {
            cout << "占用约 " << fixed << setprecision(1) << memory_kb / 1024.0 << " MB";
        }
        else if (memory_kb < 1024 * 1024)
        {
            cout << "占用约 " << fixed << setprecision(1) << memory_kb / 1024.0 << " MB（较大）";
        }
        else
        {
            cout << "占用约 " << fixed << setprecision(1) << memory_kb / (1024.0 * 1024.0) << " GB（过大）";
        }
        cout << endl;
    }

    // ========== 算法适用性总结 ==========
    cout << endl
         << "=== 算法适用性总结 ===" << endl;
    cout << "1. 状态压缩DP将暴力算法的双阶乘复杂度O((m-1)!!)优化为指数复杂度O(2^m)" << endl;
    cout << "2. 可处理规模：m ≤ 22（此时内存约32MB，时间约10ms级）" << endl;
    cout << "3. 主要限制：O(2^m)的空间复杂度，当m > 24时内存需求超过GB级" << endl;
    cout << "4. 适用场景：需要精确解的中小规模问题，或作为验证其他算法正确性的基准" << endl;
}

int main()
{
    runStateDPTest();
    return 0;
}