#ifndef MIKUS_LUCIA_COLLIDER_POINT_H
#define MIKUS_LUCIA_COLLIDER_POINT_H

#include <Lucia/Collider/Collider.h>
#include <Lucia/Collider/Shape/Polygon.h>
namespace Lucia{
class Collider::Point: public Collider::Shape
{
    public:
        friend class Collider::Shape;

        Point(){}
        void make(Manager *M,Vertex C);
        bool collidesWith(std::shared_ptr<Shape> A);
        void draw();
        virtual ~Point(){};
    protected:
    private:
};
}
#endif // POINT_H
