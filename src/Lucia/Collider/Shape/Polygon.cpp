#include "Lucia/Collider/Shape/Polygon.h"
using namespace Lucia;
using namespace Collider;
//http://in2gpu.com/2014/05/18/gjk-algorithm-3d/ -- record this somewhere here!
void Polygon::make(Manager* P,std::vector<Vertex> Points)
{
    Parent = P;
    create(Points);
}
void Polygon::Polygon::create(std::vector<Vertex>P)
{
    Points = P;
    auto Extremes = extremes(Points);
    auto Min = Extremes.first;
    auto Max = Extremes.second;
    Vertex Dimensions = Max - Min;
    Vertex Center = Max - Dimensions/2;
    synthesize(Center,Dimensions);
    type = Shape::Type::polygon;
}
void Polygon::draw()
{
    drawBox();
    Parent->DrawPolygon(*getMatrix(),polygonID,Vertex(1.0,0,0));
}
void Polygon::link(Manager *m)
{
    Parent = m;
}
bool Polygon::collidesWith(std::shared_ptr<Shape> A)
{
    // you should fix this because otherwise you will have a bad time!
    if (Box.collidesWith(A->getBox()))
    {
        switch (A->type)
        {
            case Shape::box:
            {
                GJK test = GJK();
                refresh();
                A->refresh();
                if (test.collidesWith(A.get(),this))
                {
                    return true;
                };
                break;
            }
            case Shape::point:
            {
                // check if the point is in the polygon, you can use GJK if you wish.
                GJK test = GJK();
                refresh();
                A->refresh();
                if (test.collidesWith(A.get(),this))
                {
                    return true;
                };
                break;
            }
            case Shape::sphere:
            {
                GJK test = GJK();
                refresh();
                A->refresh();
                if (test.collidesWith(A.get(),this))
                {
                    return true;
                };
                break;
            }
            case Shape::polygon:
            {
                GJK test = GJK();
                refresh();
                A->refresh();
                if (test.collidesWith(A.get(),this))
                {
                    return true;
                };
                break;
            }
        }
    }
    return false;
}

