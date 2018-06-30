#include "kdtree.h"

#include <iostream>
#include <array>
#include <vector>
#include <Eigen/Dense>

#include <emscripten/bind.h>

using namespace emscripten;

using Point = Eigen::Vector3d;
using Edge = std::pair<int, int>;
using Segment = std::pair<Point, Point>;
using SimplePoint = std::array<float, 3>;

using Tree = jk::tree::KDTree<Point, 3>;

struct Hypha
{
    Point position{0,0,0};
    Point velocity{1,0,0};
    Eigen::Quaterniond orientation;
};

SimplePoint makeSimplePoint(const Point& point)
{
    SimplePoint p;
    p[0] = point.x();
    p[1] = point.y();
    p[2] = point.z();
    return p;
}

Point makePoint(const SimplePoint& point)
{
    Point p;
    p.x() = point[0];
    p.y() = point[1];
    p.z() = point[2];
    return p;
}

class Fungus
{
public:
    Fungus()
    {
        Hypha h1, h2, h3;
        h1.position = {1, 0, 0};
        h1.velocity = {.1, 0, 0};
        h2.position = {0, .1, 0};
        h2.velocity = {0, .1, 0};
        h3.position = {-.1, 0, 0};
        h3.velocity = {-.1, 0, 0};
        tips.push_back(h1);
        tips.push_back(h2);
        tips.push_back(h3);
    }

    void newTips()
    {
        for (auto& tip : tips)
        {
            points.push_back(tip.position);
            tip.position += tip.velocity;
        }
    }

    void collisionAvoidance()
    {
        Tree tree;
        bool autoSplit{false};
        for (const auto& point : points)
        {
            tree.addPoint(makeSimplePoint(point), 0.f, autoSplit);
        }
        tree.splitOutstanding();

        std::vector<Point> offsets(points.size(), Point::Zeros());

        size_t idx{0};
        double radius{10};
        for (const auto& point : points)
        {
            auto nearPoints = tree.searchBall(makeSimplePoint(point), radius * radius);

            for (const auto& nearPoint : nearPoints)
            {
                Point otherPoint = makePoint(nearPoint);
                Point offset = otherPoint - point;
                double distance = Eigen::squaredNorm(offset);

                offsets[idx] += offset * (1.0 / std::max(0.1, distance));
            }
            idx++;
        }

        size_t idx{0}
        for (const auto& point : points)
        {
            point += offsets[idx];
            idx++;
        }
    }

    void update()
    {
        newTips();
        collisionAvoidance();
    }

    std::vector<SimplePoint> getNewPoints()
    {
        std::vector<SimplePoint> simplePoints;
		simplePoints.reserve(points.size());
        for (const auto& point : points)
        {
            SimplePoint p = makeSimplePoint(point);
            simplePoints.push_back(p);
        }
		points.clear();

        return simplePoints;
    }

private:
    std::vector<Hypha> tips;
    std::vector<Segment> segments;
    std::vector<Point> points;
};


EMSCRIPTEN_BINDINGS(fungus)
{
    value_array<SimplePoint>("Point")
        .element(&SimplePoint[0])
        .element(&SimplePoint[1])
        .element(&SimplePoint[2]);

    register_vector<SimplePoint>("VectorPoint");

    class_<Fungus>("Fungus")
        .constructor<>()
        .function("update", &Fungus::update)
        .function("getNewPoints", &Fungus::getNewPoints);
};
