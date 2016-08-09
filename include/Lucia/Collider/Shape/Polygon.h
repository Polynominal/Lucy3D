#ifndef MIKUS_LUCIA_COLLIDER_POLYGON_H
#define MIKUS_LUCIA_COLLIDER_POLYGON_H
#include <Lucia/Collider/Shape.h>
namespace Lucia {
class Collider::Polygon: public Collider::Shape
{
    public:
        friend class Collider::Shape;

        void create(std::vector<Vertex>Points);
        void link(Manager *m);

        void draw();
        bool collidesWith(std::shared_ptr<Shape> A);
        Polygon(){};
        void make(Manager* Parent,std::vector<Vertex> Points);
        ~Polygon(){};
    protected:
    private:
        Manager* parent;

};
}
#endif // POLYGON_H
