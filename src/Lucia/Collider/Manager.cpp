#include "Lucia/Collider/Manager.h"
#include <Lucia/Collider/Shape.h>

#include <Lucia/Collider/Shape/Polygon.h>
#include <Lucia/Collider/Shape/Point.h>
#include <Lucia/Collider/Shape/Sphere.h>
#include <Lucia/Collider/Shape/Ray.h>

using namespace Lucia;
using namespace Collider;

Manager::Manager()
{
    Broadphase = Algorithm::Octtree();
    //ctor
}
// Manager works by procedurally expanding world, meaning that new chunks will be added procedurally
void incrementDimension(float &c,float degree)
{
    if (c == 0){return;};
    float deg32 = remainder(c,degree);
    float left  = 0;
    float right = 0;
    if (deg32 != 0)
    {
        right = distance(c,c + deg32);
        left = distance(c,c - deg32);
        // determine how close we are to the boundary and adjust
        if (right > left)
        {
            c = c + deg32;

        }else
        {
            c = c - deg32;
        }
    }
}
Vertex Manager::incrementPos(float x,float y,float z)
{
    float degree =1;
    incrementDimension(x,degree);
    incrementDimension(y,degree);
    incrementDimension(z,degree);
    // if we arent incremented already
    return Vertex(x,y,z);
}
// find the exteremes of a few shapes
void Manager::scanCollisions(std::vector<std::shared_ptr<Shape>> Shapes)
{
}
void Manager::update()
{
    Broadphase.update();
}
void Manager::addShape(std::shared_ptr<Shape>A)
{

    A->link(this);
    Broadphase.addShape(A);
}
// must be initialized shapes!!!
// Otherwise undefined behaviour occurs
void Manager::removeShape(std::shared_ptr<Shape>A)
{
    Broadphase.removeShape(A.get());
}
void Manager::setChanged(bool c)
{
    Broadphase.setChanged(c);
}
void Manager::drawChunks(bool blocks)
{
    PreDraw();
    Broadphase.Root->draw([this](Vertex c,Vertex dimensions){
        Matrix<4> Mat;
        Mat.scale(dimensions);
        Mat.translate(c);
        DrawBox(Mat,Vertex(0.0f,1.0f,0.0f));
    });
    PostDraw();
}
std::shared_ptr<Polygon> Manager::addPolygon(std::vector<Vertex>Points)
{
    auto a = std::make_shared<Polygon>();
    a->make(this,Points);
    addShape(a);
    return a;

}
std::shared_ptr<Point> Manager::addPoint(Vertex Center)
{
    auto a = std::make_shared<Point>();
    a->make(this,Center);
    addShape(a);
    return a;
}
std::shared_ptr<Collider::Sphere> Manager::addSphere(Vertex Center, float radius)
{
    auto a = std::make_shared<Sphere>();
    a->make(this,Center,radius);
    addShape(a);
    return a;
}
std::shared_ptr<Collider::Ray> Manager::addRay(Vertex A, Vertex B)
{
    auto a = std::make_shared<Ray>();
    a->make(this,A,B);
    addGhost(a,a->Dimensions);
    return a;
}
void Manager::addGhost(std::shared_ptr<Shape> A,Vertex dim)
{
    addShape(A);
}
std::shared_ptr<Shape> Manager::addBox(float x,float y,float z,float w,float h,float d)
{
    auto shape = std::make_shared<Shape>(this);
    shape->generate(Vertex(x,y,z),Vertex(w,h,d));
    shape->setDimensions(Vertex(w,h,d));
    shape->moveTo(Vertex(x,y,z));
    shape->applyTranslations();
    addShape(shape);
    return shape;
}
Manager::~Manager()
{
    //dtor
}
