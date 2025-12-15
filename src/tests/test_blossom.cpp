/*
Blossom算法性能测试程序

测试目的：
1. 验证LEMON库Blossom算法的正确性
2. 评估算法在不同规模下的性能表现
3. 验证多项式时间复杂度特性

测试规模：10-1000个顶点（偶数）
测试方法：重复测试取平均值，保证结果稳定性
*/

#include "../algorithms/blossom_lemon.h"
#include "../utils/generate_matrix.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <vector>
#include <cmath>

using namespace std;
using namespace chrono;

/**
 * 统一性能测试函数
 */
void runConsistentTest()
{
    cout << "=== Blossom算法性能测试 ===" << endl;
    cout << fixed << setprecision(2);

    // 测试规模：从10到1000，覆盖小、中、大规模
    vector<int> sizes = {10, 20, 30, 50, 80, 100, 150, 200, 300, 400, 500, 800, 1000};
    const int REPEAT = 3;
    const int MAX_DIST = 100;

    // 预热
    auto warmup = generateRandomDistMatrix(100, MAX_DIST);
    blossomMatching(100, warmup);

    // 主性能表
    cout << "\nBlossom算法性能分析" << endl;
    cout << left << setw(8) << "n"
         << left << setw(15) << "平均耗时(μs)"
         << left << setw(18) << "状态" << endl;
    cout << string(50, '-') << endl;

    vector<pair<int, double>> size_avg_time;

    for (int n : sizes)
    {
        if (n % 2 != 0)
            continue;

        long long total_time = 0;
        bool success = true;

        // 固定随机数据源，确保每次运行输入相同
        auto fixed_matrix = generateRandomDistMatrix(n, MAX_DIST);

        for (int r = 0; r < REPEAT; ++r)
        {
            auto start = high_resolution_clock::now();
            long long cost = blossomMatching(n, fixed_matrix);
            auto stop = high_resolution_clock::now();

            total_time += duration_cast<microseconds>(stop - start).count();
            if (cost < 0)
                success = false;
        }

        double avg_time = total_time / (double)REPEAT;

        cout << left << setw(8) << n
             << left << setw(15) << avg_time
             << left << setw(18) << (success ? "✓ 成功" : "✗ 失败") << endl;

        size_avg_time.push_back({n, avg_time});
    }

    // 时间复杂度验证表
    cout << "\n时间复杂度验证（基于平均耗时）" << endl;
    cout << left << setw(8) << "n"
         << left << setw(15) << "T(n)(μs)"
         << left << setw(15) << "n³"
         << left << setw(20) << "比值 T(n)/n³" << endl;
    cout << string(60, '-') << endl;

    for (auto &[n, avg_time] : size_avg_time)
    {
        if (n < 30)
            continue; // 只验证足够大的规模
        double n3 = pow(n, 3);
        double ratio = avg_time / n3;
        cout << left << setw(8) << n
             << left << setw(15) << avg_time
             << left << setw(15) << fixed << setprecision(0) << n3
             << left << setw(20) << scientific << setprecision(2) << ratio << endl;
    }

    // 极限测试
    cout << "\n=== 极限规模测试 ===" << endl;
    vector<int> large_sizes = {1200, 1500, 2000};
    for (int n : large_sizes)
    {
        if (n % 2 != 0)
            continue;
        auto matrix = generateRandomDistMatrix(n, MAX_DIST);
        auto start = high_resolution_clock::now();
        long long cost = blossomMatching(n, matrix);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(stop - start);
        cout << "n=" << n << ": " << duration.count() << " ms, "
             << "cost=" << cost << endl;
    }
}

int main()
{
    runConsistentTest();
    return 0;
}