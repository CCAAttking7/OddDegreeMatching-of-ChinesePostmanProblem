/*
暴力搜索算法性能测试程序

测试目的：
1. 验证算法正确性
2. 展示指数爆炸特性
3. 提供算法性能基准数据

测试规模：暴力算法仅能处理极小规模问题（m ≤ 12）
测试方法：重复多次取平均，减少计时波动
*/

#include "../algorithms/brute_force.h"
#include "../utils/generate_matrix.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <vector>

using namespace std;
using namespace std::chrono;

/**
 * 暴力算法性能测试主函数
 */
void runBruteForceTest()
{
    cout << "=== 暴力搜索算法性能评估 ===" << endl;
    cout << "测试目的：验证算法正确性并展示其指数爆炸特性" << endl
         << endl;

    // 测试规模：暴力算法仅能处理极小规模问题
    vector<int> test_sizes = {4, 6, 8, 10, 12};
    const int REPEAT_TIMES = 5; // 重复次数，取平均减少波动
    const int MAX_DIST = 50;

    cout << fixed << setprecision(2);
    cout << left << setw(10) << "规模(m)"
         << left << setw(20) << "枚举方案数( (m-1)!! )"
         << left << setw(20) << "平均耗时(μs)"
         << left << setw(20) << "最小匹配成本"
         << "性能趋势" << endl;
    cout << string(90, '-') << endl;

    for (int m : test_sizes)
    {
        if (m % 2 != 0)
            continue;

        // 生成测试数据
        auto dist = generateRandomDistMatrix(m, MAX_DIST);

        // 预热：避免冷启动影响计时
        bruteForceMatching(m, dist);

        // 正式计时：重复运行取平均值
        long long total_time = 0;
        long long result = 0;
        for (int r = 0; r < REPEAT_TIMES; ++r)
        {
            auto start = high_resolution_clock::now();
            result = bruteForceMatching(m, dist);
            auto stop = high_resolution_clock::now();
            total_time += duration_cast<microseconds>(stop - start).count();
        }

        long long avg_time = total_time / REPEAT_TIMES;
        long long enum_count = doubleFactorial(m); // 计算理论枚举方案数

        // 输出结果
        cout << left << setw(10) << m;
        cout << left << setw(20) << enum_count;
        cout << left << setw(20) << avg_time;
        cout << left << setw(20) << result;

        // 性能趋势分析
        if (avg_time < 10)
        {
            cout << "极快（毫秒级以下）";
        }
        else if (avg_time < 1000)
        {
            cout << "快速（毫秒级）";
        }
        else if (avg_time < 10000)
        {
            cout << "较慢（10+毫秒）";
        }
        else
        {
            cout << "很慢（100+毫秒）";
        }
        cout << endl;
    }

    // ========== 极限规模测试 ==========
    cout << "\n=== 极限规模测试 m=14 ===" << endl;
    cout << "理论枚举方案数: " << doubleFactorial(14) << endl;
    cout << "开始计算...（可能需要数十秒）" << endl;

    auto dist14 = generateRandomDistMatrix(14, 50);
    auto start = high_resolution_clock::now();
    long long result14 = bruteForceMatching(14, dist14);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);

    cout << "最小匹配权和: " << result14 << endl;
    cout << "实际耗时: " << duration.count() << " ms ("
         << fixed << setprecision(3) << duration.count() / 1000.0 << " s)" << endl;

    // ========== 算法复杂度与局限性分析 ==========
    cout << "\n=== 算法复杂度与局限性分析 ===" << endl;
    cout << "1. 时间复杂度: O((m-1)!! × m)" << endl;
    cout << "   - (m-1)!! 随 m 呈超指数增长" << endl;
    cout << "   - m=12: (11)!! = 10,395 方案" << endl;
    cout << "   - m=14: (13)!! = 135,135 方案（已需秒级计算）" << endl;
    cout << "   - m=16: (15)!! = 2,027,025 方案（分钟级）" << endl;
    cout << "2. 空间复杂度: O(m)（仅需存储顶点排列）" << endl;
    cout << "3. 适用上限: m ≤ 12（实际可接受规模）" << endl;
    cout << "4. 主要价值: 作为精确解基准，验证优化算法的正确性" << endl;
}

int main()
{
    runBruteForceTest();
    return 0;
}