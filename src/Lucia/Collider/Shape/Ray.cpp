#include "Lucia/Collider/Shape/Ray.h"
#include <Lucia/Collider/Algorithm/GJK.h>
using namespace Lucia;
using namespace Collider;

Ray::Ray()
{

}
void Ray::make(Manager *M,Vertex a,Vertex b)
{
    A = a;
    B = b;
    Vertex Max = a.max(b);
    Vertex Min = a.min(b);
    Parent = M;

    Dimensions = (Max-Min);
    Position = Max - Dimensions/2;

    synthesize(Position,Dimensions);
}
void Ray::draw()
{
    Parent->DrawRay(A,B,Color);
}
bool Ray::collidesWith(std::shared_ptr<Shape> A)
{
    GJK test = GJK();
    if (test.collidesWith(this,A.get()))
    {
        return true;
    }
    return false;
}
Vertex Ray::recrusiveContact(std::vector<Vertex> points,Shape *shape,float minRange)
{
    Vertex start = points[0];
    Vertex normal = (points[0] + points[1])/points[1];
    float limit = points[0].distance(points[1]).magnitude();
    normal.validate();
    int no = 1;
    GJK test = GJK();
    bool t = false;
    while(!t)
    {
        t = test.collidesWith(points,shape);
        points[0] = points[0] + (normal*no);
        no++;
        if (points[0] == points[1])
        {
            return start;
        }
        if (no > limit){t = true;};
    }
    return points[0];
}
Vertex Ray::getDistance(Shape *b,float range,bool a_is_first)
{
    std::vector<Vertex> points;
    if (a_is_first)
    {
        points.push_back(A);
        points.push_back(B);
    }
    else
    {
        points.push_back(B);
        points.push_back(A);
    }
    return recrusiveContact(points,b,range);
}
Vertex Ray::support(Vertex dir)
{
    float maxDot = A.dot(dir);
    Vertex maxPoint = A;
    float dot = B.dot(dir);
    if (dot > maxDot)
    {
        maxPoint = B;
    }
    return maxPoint;
}
Ray::~Ray()
{

}
