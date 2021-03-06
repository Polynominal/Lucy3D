#ifndef MIKUS_LUCIA_COLLIDER_SHAPE_H
#define MIKUS_LUCIA_COLLIDER_SHAPE_H
#include <Lucia/Collider/Collider.h>
#include <Lucia/Collider/Manager.h>
#include <Lucia/Collider/Algorithm/GJK.h>
#include <Lucia/Utils/Utils.h>
#include <Lucia/Maths/Moveable.h>
#include <Lucia/Collider/BoundingBox.h>
namespace Lucia {
class Collider::Shape
: public std::enable_shared_from_this<Collider::Shape>,
public Lucia::Maths::Moveable
{
    friend class Collider::Point;
    friend class Collider::Sphere;
    friend class Collider::Polygon;
    friend class Collider::Ray;

    public:
        // the bounding box itself is a primitive shape, please keep this in mind
        friend class Manager;
        enum Type
        {
            box,
            polygon,
            sphere,
            point,
            ray
        };
        int type = Type::box;

        Shape(){};
        Shape(Manager *Colly);
        void link(Manager *Colly);
        // Manager works only with moveTo, it does not passively check collision
        void generate(Vertex Center,Vertex Dimensions,bool t=true);
        virtual void onMove();
        virtual void onScale();
        
        // virtual as it is a requirement for other shapes to redefine their collision methods eg circle.

        //collision based
        bool boxCollidesWith(BoundingBox *B);
        bool collidesLast(std::shared_ptr<Shape> B);
        virtual bool collidesWith(std::shared_ptr<Shape> B); // virtual for different implementation across shapes
        virtual bool contains(Vertex A);
        bool contains(float x,float y,float z){return contains(Vertex(x,y,z));};


        /* The following should be the support functions for new shapes:
        *  -------------------------------------------s----------        *
        *   Box: Iterate over each point pick based on dot product.
        *
        *   Sphere: support(dir) = center + radius*(dir / ||dir|| )
        *
        * ------------------------------------------------------
        *  Support function is a simple function that must return the furtherest vertex in a direction (x,y,z)
        *
        */
        virtual Vertex support(Vertex dir);
        virtual void draw();
        virtual void onMorph(){refresh();};
        // translations
        void update_translation(bool a=false);
        void refresh(); // used for applying translations to non-translated points!
        // draws
        void drawBox();

        // Gets
        BoundingBox* getBox();
        Vertex getMaxDimensions();// the maximum dimensions of the shape
        Vertex getDimensions(){return Size*getScale();};
        //sets 
        Vertex Color =  Vertex(0.0,0.0,1.0);

        //lambdas
        std::function <void(Shape *B)> OnCollide = [](Shape *B){};
        std::function <void(Shape *B)> OnRelease = [](Shape *B){}; // when collision is done.
        std::function <void()> OnChange = [](){};
        // containers
        std::vector<std::shared_ptr<Shape>> Collisions; // currently colliding with. used to determine if one should call OnCollide.

        std::vector<Vertex> Points;
        std::vector<Vertex> Translated;
        Vertex Size;
        float BBoxSize=1;

        void* UserData;

        // destructors
        void remove(); // required to mark for removal.
        virtual ~Shape();
        
        void push(std::shared_ptr<Shape> B);    // pushes the specific shape to collision list.
        void pop(std::shared_ptr<Shape> B);    // pops

    protected:
        BoundingBox Box = BoundingBox(0,0,0,10,10,1);

    private:
        void cleanCollisions();
        void sendMoveSignal();

        bool removed = true;
        Manager *Parent=nullptr;
};
}
#endif // COLLIDER_SHAPE_H
