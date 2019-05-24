#include "utils.hpp"

using namespace cv;

Point midpoint(const Point& a, const Point& b) {
    Point ret;
    ret.x = (a.x + b.x) / 2;
    ret.y = (a.y + b.y) / 2;
    return ret;
}
