#include "Lucia/Collider/Shape/Polygon.h"
using namespace Lucia;
using namespace Collider;
//http://in2gpu.com/2014/05/18/gjk-algorithm-3d/ -- record this somewhere here!
void Polygon::make(Manager* P,std::vector<Vertex> Points)
{
    Parent = P;
    //create(Points);
}
bool removeDuplicates(std::vector<Vertex>* v,int start)
{
    auto one = v[0][start];
    int id = 0;
    bool hit = false;
    for (auto m: *v)
    {
        if (m == one and id != start)
        {
            hit = true;
            v->erase(v->begin() + id);
            return true;
        }
        id++;
    }
    if (not hit)
    {
        int n = start + 1;
        if (v->size() > n)
        {
            return removeDuplicates(v,n);
        }
    }
    return false;
}
void removeDuplicatesAgent(std::vector<Vertex>* v)
{
    int id = 0;
    for (auto m: *v)
    {
       if (removeDuplicates(v,id))
       {
           removeDuplicatesAgent(v);
       }
        id++;
    };
}
void Polygon::Polygon::create(std::vector<Vertex>P)
{
    if (polygonID != -1){Parent->DeletePolygon(polygonID);};
    polygonID = -1;
    
    auto Extremes = extremes(P);
    auto Min = Extremes.first;
    auto Max = Extremes.second;

    Vertex Dimensions = Max - Min;
    generate(getPosition(),Dimensions);
    //generate(getPorition(),Dimensions);
    triangulated = P;
    Points = triangulated;
    removeDuplicatesAgent(&Points);
    type = Shape::Type::polygon;
}
void Polygon::draw()
{
    if (polygonID == -1){
        polygonID = Parent->CreatePolygon(triangulated);
    }
    drawBox();
    Parent->DrawPolygon(*getMatrix(),polygonID,Vertex(1.0,0,0));
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

