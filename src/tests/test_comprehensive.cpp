/*
综合对比测试程序（第四章使用）

测试目的：
1. 四种算法（暴力、状态压缩DP、区间DP、Blossom）的全面性能对比
2. 生成时间、空间复杂度对比曲线
3. 分析各算法的适用边界与优势场景

输出格式：CSV数据文件 + Python可视化脚本
*/

#include "../algorithms/brute_force.h"
#include "../algorithms/state_dp.h"
#include "../algorithms/interval_dp.h"
#include "../algorithms/blossom_lemon.h"
#include "../utils/generate_data.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <vector>

using namespace std;
using namespace chrono;

// 测试结果结构体
struct TestResult
{
    int n;
    double time_brute;         // μs
    double time_state_dp;      // μs
    double time_interval_dp;   // μs
    double time_blossom;       // μs
    double memory_state_dp;    // KB
    double memory_interval_dp; // KB
    double memory_blossom;     // KB
    bool success_brute;
    bool success_state_dp;
    bool success_interval_dp;
    bool success_blossom;
};

/**
 * 分阶段测试函数：不同算法适用不同规模
 */
vector<TestResult> runComprehensiveTest()
{
    vector<TestResult> results;

    cout << "=== 综合性能对比测试 ===" << endl;
    cout << left << setw(8) << "n"
         << left << setw(15) << "暴力(μs)"
         << left << setw(15) << "状态DP(μs)"
         << left << setw(15) << "区间DP(μs)"
         << left << setw(15) << "Blossom(μs)" << endl;
    cout << string(70, '-') << endl;

    // 阶段1：小规模测试（n≤12，全部算法）
    vector<int> small_sizes = {4, 6, 8, 10, 12};
    for (int n : small_sizes)
    {
        if (n % 2 != 0)
            continue;

        TestResult res;
        res.n = n;

        // 生成测试数据（欧氏点集，兼顾区间DP的前提）
        auto points = generateSortedPoints(n, 1000.0);
        auto matrix = pointsToMatrix(points); // 将点集转换为距离矩阵

        // 测试暴力搜索
        auto start = high_resolution_clock::now();
        double cost_brute = bruteForceMatching(n, matrix);
        auto end = high_resolution_clock::now();
        res.time_brute = duration_cast<microseconds>(end - start).count();
        res.success_brute = (cost_brute >= 0);

        // 测试状态压缩DP
        start = high_resolution_clock::now();
        double cost_dp = stateDPMatching(n, matrix);
        end = high_resolution_clock::now();
        res.time_state_dp = duration_cast<microseconds>(end - start).count();
        res.success_state_dp = (cost_dp >= 0);

        // 测试区间DP
        start = high_resolution_clock::now();
        double cost_interval = intervalDPMatching(points);
        end = high_resolution_clock::now();
        res.time_interval_dp = duration_cast<microseconds>(end - start).count();
        res.success_interval_dp = (cost_interval >= 0);

        // 测试Blossom（仅当n≤12时测试，用于正确性验证）
        start = high_resolution_clock::now();
        long long cost_blossom = blossomMatching(n, matrix);
        end = high_resolution_clock::now();
        res.time_blossom = duration_cast<microseconds>(end - start).count();
        res.success_blossom = (cost_blossom >= 0);

        // 内存占用估计
        res.memory_state_dp = (pow(2, n) * sizeof(double)) / 1024.0;
        res.memory_interval_dp = (2.0 * n * n * sizeof(double)) / 1024.0;
        res.memory_blossom = (n * n * sizeof(double)) / 1024.0;

        results.push_back(res);

        cout << left << setw(8) << n
             << left << setw(15) << fixed << setprecision(1) << res.time_brute
             << left << setw(15) << res.time_state_dp
             << left << setw(15) << res.time_interval_dp
             << left << setw(15) << res.time_blossom << endl;
    }

    // 阶段2：中等规模（12<n≤22，跳过暴力搜索）
    vector<int> medium_sizes = {14, 16, 18, 20, 22};
    for (int n : medium_sizes)
    {
        TestResult res;
        res.n = n;
        res.time_brute = -1; // 标记为未测试

        auto points = generateSortedPoints(n, 1000.0);
        auto matrix = pointsToMatrix(points);

        // 状态压缩DP
        auto start = high_resolution_clock::now();
        double cost_dp = stateDPMatching(n, matrix);
        auto end = high_resolution_clock::now();
        res.time_state_dp = duration_cast<microseconds>(end - start).count();
        res.success_state_dp = (cost_dp >= 0);

        // 区间DP
        start = high_resolution_clock::now();
        double cost_interval = intervalDPMatching(points);
        end = high_resolution_clock::now();
        res.time_interval_dp = duration_cast<microseconds>(end - start).count();
        res.success_interval_dp = (cost_interval >= 0);

        // Blossom
        start = high_resolution_clock::now();
        long long cost_blossom = blossomMatching(n, matrix);
        end = high_resolution_clock::now();
        res.time_blossom = duration_cast<microseconds>(end - start).count();
        res.success_blossom = (cost_blossom >= 0);

        res.memory_state_dp = (pow(2, n) * sizeof(double)) / 1024.0;
        res.memory_interval_dp = (2.0 * n * n * sizeof(double)) / 1024.0;
        res.memory_blossom = (n * n * sizeof(double)) / 1024.0;

        results.push_back(res);

        cout << left << setw(8) << n
             << left << setw(15) << "N/A"
             << left << setw(15) << fixed << setprecision(1) << res.time_state_dp
             << left << setw(15) << res.time_interval_dp
             << left << setw(15) << res.time_blossom << endl;
    }

    // 阶段3：大规模（n>22，只测试区间DP和Blossom）
    vector<int> large_sizes = {30, 50, 100, 200, 300, 500};
    for (int n : large_sizes)
    {
        TestResult res;
        res.n = n;
        res.time_brute = -1;
        res.time_state_dp = -1;

        auto points = generateSortedPoints(n, 1000.0);
        auto matrix = pointsToMatrix(points);

        // 区间DP
        auto start = high_resolution_clock::now();
        double cost_interval = intervalDPMatching(points);
        auto end = high_resolution_clock::now();
        res.time_interval_dp = duration_cast<microseconds>(end - start).count();
        res.success_interval_dp = (cost_interval >= 0);

        // Blossom
        start = high_resolution_clock::now();
        long long cost_blossom = blossomMatching(n, matrix);
        end = high_resolution_clock::now();
        res.time_blossom = duration_cast<microseconds>(end - start).count();
        res.success_blossom = (cost_blossom >= 0);

        res.memory_interval_dp = (2.0 * n * n * sizeof(double)) / 1024.0;
        res.memory_blossom = (n * n * sizeof(double)) / 1024.0;

        results.push_back(res);

        cout << left << setw(8) << n
             << left << setw(15) << "N/A"
             << left << setw(15) << "N/A"
             << left << setw(15) << fixed << setprecision(1) << res.time_interval_dp
             << left << setw(15) << res.time_blossom << endl;
    }

    return results;
}

/**
 * 导出CSV数据文件（用于Python可视化）
 */
void exportToCSV(const vector<TestResult> &results, const string &filename)
{
    ofstream file(filename);
    if (!file.is_open())
    {
        cerr << "无法创建CSV文件" << endl;
        return;
    }

    // 写入表头
    file << "n,time_brute,time_state_dp,time_interval_dp,time_blossom,"
         << "memory_state_dp,memory_interval_dp,memory_blossom,"
         << "success_brute,success_state_dp,success_interval_dp,success_blossom" << endl;

    // 写入数据
    for (const auto &res : results)
    {
        file << res.n << ","
             << res.time_brute << ","
             << res.time_state_dp << ","
             << res.time_interval_dp << ","
             << res.time_blossom << ","
             << res.memory_state_dp << ","
             << res.memory_interval_dp << ","
             << res.memory_blossom << ","
             << res.success_brute << ","
             << res.success_state_dp << ","
             << res.success_interval_dp << ","
             << res.success_blossom << endl;
    }

    file.close();
    cout << "\n数据已导出至: " << filename << endl;
}

int main()
{
    auto results = runComprehensiveTest();
    exportToCSV(results, "algorithm_comparison.csv");
    cout << "\n测试完成！数据已导出到 algorithm_comparison.csv" << endl;

    // 提示用户运行Python脚本
    cout << "\n请运行以下命令生成图表：" << endl;
    cout << "python algorithm_visualization.py" << endl;
    return 0;
}