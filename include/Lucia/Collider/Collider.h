#ifndef MIKUS_LUCIA_COLLIDER_MAIN_H
#define MIKUS_LUCIA_COLLIDER_MAIN_H
#include <cmath>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <functional>
#include <memory>
#include <math.h>

#include <Lucia\Maths\Matrix.h>
#include <Lucia\Maths\Quaternion.h>
#include <Lucia\Maths\Vertex.h>
#include <Lucia\Maths\Vector.h>

using namespace std;
namespace Lucia {
namespace Collider{
    using namespace Lucia::Maths;
    class BoundingBox;
    class Shape;
    // SHAPES
        class Sphere;
        class Polygon;
        class Point;
        class Ray;

        class GJK;
    //
    class Space;
    class Manager;
}
}
#include <Lucia/Collider/Tools.h>
#include <Lucia/Collider/Algorithm/GJK.h>
#include <Lucia/Collider/Shape.h>
#include <Lucia/Collider/Manager.h>

#endif //COLLIDER_MATHS_2_H
