#ifndef COLLIDER_SHAPE_H
#define COLLIDER_SHAPE_H
#include <Collider/Collider.h>
#include <GJK.h>
#include <utilities/Utils.h>

class Collider::BoundingBox
{
    public:
        BoundingBox();
        BoundingBox(float x,float y,float z,float w,float h,float d);
        bool collidesWith(BoundingBox *Box);
        bool engulfs(BoundingBox *Box);
        bool contains(float x,float y,float z);
        bool contains(Vertex v){return contains(v.x,v.y,v.z);};
        void refresh();
        Vertex getDimensions(); // width,height,depth
        Vertex getMaxDimensions(); // width height and depth and rotation.
        virtual ~BoundingBox();

        std::vector<Vertex> Points;
        std::vector<Vertex> Faces;
        Tools::Matrix<4> Transformation;
        Vertex Center;

        Vertex Min;
        Vertex Max;
        Vertex Dimensions;
};
class Collider::Shape
: public std::enable_shared_from_this<Collider::Shape>
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
        // Manager works only with moveTo, it does not passively check collision
        void synthesize(Vertex Center,Vertex Dimensions,bool t=true);

        BoundingBox* getBox();
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

        // translations
        void update_translation(bool a=false);
        void refresh(); // used for applying translations to non-translated points!
        // draws
        void drawBox();
        //application of translations: [void to's]

        virtual void setDimensions(float x,float y,float z);
        virtual void scale(float x,float y,float z);
        virtual void scaleTo(float x,float y,float z);

        void scale(Vertex &v);
        void scaleTo(Vertex &v);
        //

        virtual void move(float x,float y,float z);
        virtual void moveTo(float x,float y,float z);

        void move(Vertex &v);
        void moveTo(Vertex &v);
        //

        virtual void rotate(float x,float y,float z);
        virtual void rotateTo(float x,float y,float z);

        void rotate(Vertex &v);
        void rotateTo(Vertex &v);

        // Gets
        Vertex getPosition(); // the centre.
        Vertex getMaxDimensions();// the maximum dimensions of the shape

        Vertex Color =  Vertex(0.0,0.0,1.0);

        //lambdas
        std::function <void(Shape *B)> OnCollide = [](Shape *B){};
        std::function <void(Shape *B)> OnRelease = [](Shape *B){}; // when collision is done.
        std::function <void()> OnChange = [](){};
        // containers
        std::vector<std::shared_ptr<Shape>> Collisions; // currently colliding with. used to determine if one should call OnCollide.

        std::vector<Vertex> Points;
        void* UserData;

        // destructors
        void remove(); // required to mark for removal.
        virtual ~Shape();

    protected:
        std::vector<Vertex> Translated;
        int polygonID=0;

        Tools::Matrix<4> Translation;
        BoundingBox Box = BoundingBox(0,0,0,10,10,1);

    private:
        void cleanCollisions();
        void sendMoveSignal();

        void push(std::shared_ptr<Shape> B);    // pushes the specific shape to collision list.
        void pop(std::shared_ptr<Shape> B);    // pops


        // Translation = the above combined.
        std::shared_ptr<Space> space;
        //scale,rotation and position
        bool needsRefresh = true;
        bool removed = true;

        Vertex Scale = Vertex(1,1,1);
        Vertex Rotation = Vertex(0,0,0);
        Vertex Position = Vertex(0,0,0);
        Vertex Dimensions = Vertex(0,0,0);

        Manager *Parent=nullptr;
};
class Collider::Space
{
    public:

        Space(Vertex Center,int degree); // the degree to which you wish to divide
        Space(Vertex Center,Vertex dim); // dimensions for ghosts
        void erase(Manager &Manager);
        void setGhost(bool t);
        ~Space();

        BoundingBox Box;
        std::vector<std::shared_ptr<Shape>>Shapes; // the shapes within the spaces.
        int degree =0;
        bool changed = true;
        bool ghost = false;
};
#endif // COLLIDER_SHAPE_H
