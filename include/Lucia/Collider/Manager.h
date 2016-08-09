#ifndef MIKUS_LUCIA_COLLIDER_MANAGER_H
#define MIKUS_LUCIA_COLLIDER_MANAGER_H
#include <Lucia/Collider/Collider.h>
#include <Lucia/Collider/Tools.h> // matrix<uint> and inf vector;
namespace Lucia {
class Collider::Manager
{
    public:
        friend class Shape;
        Manager();
        // when new shape is added it does not cause a collision response, only if its moved!
        int addShape(std::shared_ptr<Shape>);
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

        std::vector<std::shared_ptr<Shape>> cast(Vertex A,Vertex B);

        virtual ~Manager();


        // use basic construction buffers eg box and pass a model matrix for [position rotation and scale]

        // DATA = 1: Position 2: Rotation 3: Scale

        // functions for drawing stuff, eg wrappers, should be enough to provide drawing capabilities.
        std::function <int(std::vector<Vertex>)> CreatePolygon = [](std::vector<Vertex> Points){return -1;}; // returns id of the specific shape

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

    protected:
    private:
        void scanShapes();
        void addToSpace(std::shared_ptr<Shape>A,int degree);
        // the size of the block, the chunk will be sizeModifier^2
        int minimumSize = 1;
        // rule of thumb minimum size should NOT be lower than fusion threshold.
        int fussionThreshold = 30;
        int critical_mass = 10000000; // the maximum size and if this size is achieved it will cause the shape to split!
        // first int stands for degree!
        // map is required to prevent sorting as space index in the second map is required to be constant!.
        std::vector <std::shared_ptr<Space>> Spaces;
};
}
#endif // COLLIDER_MANAGER_H
