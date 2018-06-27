//#include "kdtree.h"

#include <iostream>
#include <array>
#include <vector>
#include <Eigen/Dense>

#include <emscripten/bind.h>

using namespace emscripten;

using Point = Eigen::Vector3d;
using Edge = std::pair<int, int>;
using Segment = std::pair<Point, Point>;

struct Point3f
{
    float x, y, z;
};

struct Hypha
{
    Point position{0,0,0};
    Point velocity{1,0,0};
    Eigen::Quaterniond orientation;
};

class Fungus
{
public:
    Fungus()
    {
        tips.emplace_back(Hypha());
    }

    void update()
    {
        for (auto& tip : tips)
        {
            Point oldPos = tip.position;
            tip.position += tip.velocity;
            segments.emplace_back(Segment(oldPos, tip.position));
            std::cout << oldPos.transpose() << std::endl;
        }
    }

    std::vector<Point3f> getPoints()
    {
        std::vector<Point3f> simplePoints(points.size());
        for (const auto& point : points)
        {
            Point3f p;
            p.x = point.x();
            p.y = point.y();
            p.z = point.z();
            simplePoints.push_back(p);
        }

        return simplePoints;
    }

private:
    std::vector<Hypha> tips;
    std::vector<Segment> segments;
    std::vector<Point> points;
};


EMSCRIPTEN_BINDINGS(fungus)
{
    value_array<Point3f>("Point")
        .element(&Point3f::x)
        .element(&Point3f::y)
        .element(&Point3f::z);

    register_vector<Point3f>("VectorPoint");

    class_<Fungus>("Fungus")
        .constructor<>()
        .function("update", &Fungus::update)
        .function("getPoints", &Fungus::getPoints);
};
