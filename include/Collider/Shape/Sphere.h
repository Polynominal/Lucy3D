#ifndef COLLIDER_SPHERE_H
#define COLLIDER_SPHERE_H

#include <Collider/Collider.h>
#include <Collider/Shape/Polygon.h>
#include <Collider/Shape.h>
class Collider::Sphere : public Collider::Shape
{
    public:
        friend class Collider::Shape;

        Sphere(){};
        void make(Manager *M,Vertex Center,float radius);

        bool collidesWith(std::shared_ptr<Shape> A);
        bool contains(Vertex A);
        bool contains(float x,float y,float z){return contains(Vertex(x,y,z));};

        void draw();
        Vertex support(Vertex dir);
        float radius = 0;
        virtual ~Sphere(){};
    protected:
    private:
};
#endif // SPHERE_H
