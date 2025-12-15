#ifndef GENERATE_POINT_H
#define GENERATE_POINT_H

#include <vector>
#include <random>
#include <algorithm>
#include "../algorithms/interval_dp.h" // 包含Point结构

/**
 * 生成m个在[0,maxCoord)*[0,maxCoord)范围内均匀分布的随机点，
 * 并按照先x后y的字典序排序后返回
 */
inline std::vector<Point> generateSortedPoints(int m, double maxCoord = 1000.0)
{
    std::vector<Point> points(m);
    std::mt19937 gen(12345); // 固定种子保证可重复性
    std::uniform_real_distribution<> dis(0.0, maxCoord);

    for (int i = 0; i < m; ++i)
        points[i] = Point(dis(gen), dis(gen));

    // 按x坐标排序，x相同则按y坐标
    std::sort(points.begin(), points.end(),
              [](const Point &a, const Point &b)
              {
                  return (a.x == b.x) ? (a.y < b.y) : (a.x < b.x);
              });

    return points;
}

#endif // GENERATE_POINT_H