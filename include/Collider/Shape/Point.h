#ifndef COLLIDER_POINT_H
#define COLLIDER_POINT_H

#include <Collider/Collider.h>
#include <Collider/Shape/Polygon.h>

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

#endif // POINT_H
