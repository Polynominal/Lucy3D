#include "Collider/Shape/Point.h"
#include <Collider/Shape/Sphere.h>
#include <Collider/Algorithm/GJK.h>
using namespace Collider;
void Point::make(Manager *M,Vertex p)
{
    Parent = M;
    Points.push_back(p);
    synthesize(p,Vertex(1,1,1));
}
void Point::draw()
{
    Vertex p = Points[0];
    Parent->DrawPoint(Translation,Color);
}
bool Point::collidesWith(std::shared_ptr<Shape> A)
{
    if (Box.collidesWith(A->getBox()))
    {
        switch (A->type)
        {
            case Shape::box:
            {
                return true;
                break;
            }
            case Shape::point:
            {
                if (A->Points[0] == Points[0])
                {
                    return true;
                }
            }
            case Shape::sphere:
            {
                Sphere *a = static_cast<Sphere*>(A.get());
                return insideSphere(Points[0], a->radius);
                break;
            }
            case Shape::polygon:
            {
                GJK test = GJK();
                if (test.checkCollision(Points,A->Points))
                {
                    return true;
                };
                break;
            }
        }
    }
    return false;
}
