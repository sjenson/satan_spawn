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
        Hypha h1, h2, h3;
        h1.position = {1, 0, 0};
        h1.velocity = {1, 0, 0};
        h2.position = {0, 1, 0};
        h2.velocity = {0, 1, 0};
        h3.position = {-1, 0, 0};
        h3.velocity = {-1, 0, 0};
        tips.push_back(h1);
        tips.push_back(h2);
        tips.push_back(h3);
    }

    void update()
    {
        for (auto& tip : tips)
        {
            points.push_back(tip.position);
            tip.position += tip.velocity;
        }
    }

    std::vector<Point3f> getNewPoints()
    {
        std::vector<Point3f> simplePoints;
		simplePoints.reserve(points.size());
        for (const auto& point : points)
        {
            Point3f p;
            p.x = point.x();
            p.y = point.y();
            p.z = point.z();
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
    value_array<Point3f>("Point")
        .element(&Point3f::x)
        .element(&Point3f::y)
        .element(&Point3f::z);

    register_vector<Point3f>("VectorPoint");

    class_<Fungus>("Fungus")
        .constructor<>()
        .function("update", &Fungus::update)
        .function("getNewPoints", &Fungus::getNewPoints);
};
