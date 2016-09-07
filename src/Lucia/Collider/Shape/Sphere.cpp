#include "Lucia/Collider/Shape/Sphere.h"
using namespace Lucia;
using namespace Collider;
bool Sphere::collidesWith(std::shared_ptr<Shape> A)
{
    if (Box.collidesWith(A->getBox()))
    {
        switch (A->type)
        {
            case Shape::box:
            {
                A->refresh();
                refresh();

                GJK test;

                if (test.collidesWith(this,A.get()))
                {return true;};

                break;
            }
            case Shape::point:
            {
                A->refresh();
                refresh();

                auto V = A->Position;
                if (sqrt(pow(V.x - Position.x,2) + pow(V.y - Position.y,2) + pow(V.z - Position.z,2)) < radius){
                    return true;
                };
                break;
            }
            case Shape::sphere:
            {
                Sphere *a = static_cast<Sphere*>(A.get());
                Vertex d = Points[0] - a->Points[0];

                double da = sqrt(pow(d.x,2) + pow(d.y,2) + pow(d.z,2));
                float sumrad = a->radius + radius;
                if (da <= sumrad)
                {
                    return true;
                };
                break;
            }
            case Shape::polygon:
            {
                // identical to Box
                A->refresh();
                refresh();
                GJK test;
                if (test.collidesWith(this,A.get()))
                {
                    return true;
                }
                break;
            }
        }
    }
    return false;
}
void Sphere::make(Manager *M,Vertex C,float r)
{
    Parent = M;
    Position = C;
    Dimensions = Vertex(r*2,r*2,r*2);
    radius = r;
    generate(Position,Dimensions);
    type = Shape::Type::sphere;
    // make points here use subdivision!
}
Vertex Sphere::support(Vertex dir)
{
//support(dir) = center + radius*(dir / ||dir|| )
    return Vertex(Position + (((Dimensions/2)*Scale))*(dir/dir.magnitude()));
};
bool Sphere::contains(Vertex V)
{
    if (Box.contains(V))
    {
        if (sqrt(pow(V.x - Position.x,2) + pow(V.y - Position.y,2) + pow(V.z - Position.z,2)) < radius){
            return true;
        };
    }
    return false;
}
void Sphere::draw()
{
    Parent->DrawSphere(*getMatrix(),Color);
}
