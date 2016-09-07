#ifndef MIKUS_LUCIA_COLLIDER_POLYGON_H
#define MIKUS_LUCIA_COLLIDER_POLYGON_H
#include <Lucia/Collider/Shape.h>
namespace Lucia {
class Collider::Polygon: public Collider::Shape
{
    public:
        friend class Collider::Shape;
        void create(std::vector<Vertex>Points);
        void draw();
        bool collidesWith(std::shared_ptr<Shape> A);
        void generate(Vertex center,Vertex dim)
        {
            Collider::Shape::generate(center,dim);
        };
        void drawBox()
        {
            auto v = getPosition();
            BBox = Maths::Matrix<4>();
            BBox.rotate(getRotation());
            BBox.scale(Vertex(BBoxSize,BBoxSize,BBoxSize)*getScale());
            BBox.translate(getPosition());
            refresh();
            Parent->DrawBox(BBox,Color);
        };
        Polygon(){};
        void make(Manager* Parent,std::vector<Vertex> Points);
        ~Polygon(){};
        
    private:
        Maths::Matrix<4> BBox;
        int polygonID = -1;
        Manager* parent;
        std::vector<Vertex> triangulated;

};
}
#endif // POLYGON_H
