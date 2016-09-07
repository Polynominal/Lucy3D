#ifndef MIKUS_LUCIA_COLLIDER_MANAGER_H
#define MIKUS_LUCIA_COLLIDER_MANAGER_H
#include <Lucia/Collider/Collider.h>
#include <Lucia/Collider/Tools.h> // matrix<uint> and inf vector;
#include <Lucia/Collider/Algorithm/Octtree.h>

namespace Lucia {
class Collider::Manager
{
    public:
        friend class Shape;
        Manager();
        //set
        void setPrecision(float Precision){precision = Precision;};
        void setChanged(bool c);
        //get
        float getPrecision(){return precision;};
        // when new shape is added it does not cause a collision response, only if its moved!
        void addShape(std::shared_ptr<Shape>);
        void removeShape(std::shared_ptr<Shape>);
        void moveShape(std::shared_ptr<Shape>);
        void update();

        void addGhost(std::shared_ptr<Shape> A,Vertex dim);
        void scanCollisions(std::vector<std::shared_ptr<Shape>> Shapes);

        Vertex incrementPos(float x,float y,float z);
        //
        std::shared_ptr<Shape> addBox(float x,float y,float z,float w,float h,float d);
        std::shared_ptr<Shape> addBox(Vertex Center,Vertex Dimensions){return addBox(Center.x,Center.y,Center.z,
                                                                        Dimensions.x,Dimensions.y,Dimensions.z);};
        std::shared_ptr<Polygon> addPolygon(std::vector<Vertex>Points);
        std::shared_ptr<Point> addPoint(Vertex Center);
        std::shared_ptr<Sphere> addSphere(Vertex Center, float radius);
        std::shared_ptr<Ray> addRay(Vertex A,Vertex B);
        
        std::pair<float,float> getDepthLimits(){return Broadphase.getDepthLimits();};
        void cast(Vertex A,Vertex B,std::function <void(Shape *B)> collide,bool infinite_depth=false)
        {Broadphase.rayCast(A,B,collide,infinite_depth);};

        virtual ~Manager();


        // use basic construction buffers eg box and pass a model matrix for [position rotation and scale]

        // DATA = 1: Position 2: Rotation 3: Scale

        // functions for drawing stuff, eg wrappers, should be enough to provide drawing capabilities.
        std::function <int(std::vector<Vertex>)> CreatePolygon = [](std::vector<Vertex>Points){return -1;}; // returns id of the specific shape
        std::function <void(int)>DeletePolygon = [](int){};
        
        std::function <void()>PreDraw = [](){};

        std::function <void(Matrix<4>,Vertex)> DrawBox =        [](Matrix<4> Data,Vertex Color=Vertex(1.0,0.0,0.0)){};
        std::function <void(Matrix<4>,Vertex)> DrawPoint =      [](Matrix<4> Data,Vertex Color=Vertex(1.0,0.0,0.0)){};
        std::function <void(Matrix<4>,Vertex)> DrawSphere =     [](Matrix<4> Data,Vertex Color=Vertex(1.0,0.0,0.0)){};
        std::function <void(Vertex,Vertex,Vertex)> DrawRay =    [](Vertex A,Vertex B,Vertex Color=Vertex(1.0,0.0,0.0)){};
        // id is the id returned from Create Polygon
        std::function <void(Matrix<4>,int id,Vertex)> DrawPolygon = [](Matrix<4> Data,int id,Vertex Color=Vertex(1.0,0.0,0.0)){};
        // fn after draw.
        std::function <void()>PostDraw = [](){};

        void drawChunks(bool blocks);



        // collision functions:
        std::function <void(Shape*,Shape*)> OnCollision = [](Shape *A,Shape *B){};
        std::function <void(Shape*,Shape*)> OnRelease = [](Shape *A,Shape *B){};
        
        Algorithm::Octtree Broadphase = Algorithm::Octtree();
        
    private:
        float precision = 0.01f;
};
}
#endif // COLLIDER_MANAGER_H
