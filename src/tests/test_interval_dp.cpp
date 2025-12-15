/*
区间DP算法性能测试程序

测试目的：
1. 验证算法在欧氏平面点集上的正确性
2. 评估算法在不同规模下的时间与空间性能
3. 验证O(m^3)时间复杂度特性

测试规模：10-1000个点（偶数）
测试方法：固定随机种子确保结果可重现
*/

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include "../algorithms/interval_dp.h"
#include "../utils/generate_point.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <vector>

using namespace std;
using namespace std::chrono;

/**
 * 统一测试函数：确保所有表格数据来源一致
 */
void runConsistentTest()
{
    cout << "=== 区间DP算法性能一致性测试 ===" << endl;
    cout << fixed << setprecision(2);

    // 测试规模
    vector<int> sizes = {10, 20, 30, 50, 80, 100, 150, 200, 300, 400, 500};
    const int REPEAT = 5;
    const double MAX_COORD = 1000.0;

    // 预热
    auto warmup = generateSortedPoints(100, MAX_COORD);
    intervalDPMatching(warmup);

    // 主性能表
    cout << "\n表3:区间DP算法性能分析" << endl;
    cout << left << setw(8) << "m"
         << left << setw(15) << "平均耗时(μs)"
         << left << setw(18) << "理论内存(KB)"
         << left << setw(15) << "匹配成本" << endl;
    cout << string(60, '-') << endl;

    vector<pair<int, double>> size_avg_time; // 保存平均耗时用于验证表

    for (int m : sizes)
    {
        if (m % 2 != 0)
            continue;

        long long total_time = 0;
        double total_cost = 0;

        // 固定随机数据源，确保每次运行输入相同
        auto fixed_points = generateSortedPoints(m, MAX_COORD);

        for (int r = 0; r < REPEAT; ++r)
        {
            auto start = high_resolution_clock::now();
            double cost = intervalDPMatching(fixed_points);
            auto stop = high_resolution_clock::now();
            total_time += duration_cast<microseconds>(stop - start).count();
            total_cost += cost;
        }

        double avg_time = total_time / (double)REPEAT;
        double avg_cost = total_cost / REPEAT;
        double memory_kb = (2.0 * m * m * sizeof(double)) / 1024.0; // 理论内存

        cout << left << setw(8) << m
             << left << setw(15) << avg_time
             << left << setw(18) << fixed << setprecision(1) << memory_kb
             << left << setw(15) << fixed << setprecision(2) << avg_cost << endl;

        size_avg_time.push_back({m, avg_time});
    }

    // 理论复杂度验证表（使用上面计算出的平均耗时）
    cout << "\n表4:时间复杂度验证（基于平均耗时）" << endl;
    cout << left << setw(8) << "m"
         << left << setw(15) << "T(m)(μs)"
         << left << setw(15) << "m³"
         << left << setw(20) << "比值 T(m)/m³" << endl;
    cout << string(60, '-') << endl;

    for (auto &[m, avg_time] : size_avg_time)
    {
        if (m < 20)
            continue; // 只验证足够大的规模
        double m3 = pow(m, 3);
        double ratio = avg_time / m3;
        cout << left << setw(8) << m
             << left << setw(15) << avg_time
             << left << setw(15) << fixed << setprecision(0) << m3
             << left << setw(20) << scientific << setprecision(2) << ratio << endl;
    }

    // 极限测试
    cout << "\n=== 极限规模测试 ===" << endl;
    vector<int> large_sizes = {600, 800, 1000};
    for (int m : large_sizes)
    {
        if (m % 2 != 0)
            continue;
        auto points = generateSortedPoints(m, MAX_COORD);
        auto start = high_resolution_clock::now();
        double cost = intervalDPMatching(points);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(stop - start);
        double memory_mb = (2.0 * m * m * sizeof(double)) / (1024.0 * 1024.0);
        cout << "m=" << m << ": " << duration.count() << " ms, "
             << fixed << setprecision(1) << memory_mb << " MB, "
             << "cost=" << fixed << setprecision(2) << cost << endl;
    }
}

int main()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    runConsistentTest();
    return 0;
}