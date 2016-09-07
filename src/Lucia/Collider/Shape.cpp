#include "Lucia/Collider/Shape.h"
using namespace Lucia;
using namespace Collider;
Shape::Shape(Manager *Colly)
{
    Parent = Colly;
    type = Type::box;
}
BoundingBox * Shape::getBox()
{
    return &Box;
};
void Shape::link(Manager *Colly){Parent = Colly;Parent->setChanged(true);};
void Shape::generate(Vertex C,Vertex dim,bool t)
{
    // find the maximum dimension this is done to create the largest possible cube!
    enableImplicitTranslation(true);
    if (Points.size() == 0){Points = BoundingBox(0,0,0,1,1,1).Points;};
    float w = 0;
    float M = dim.max();
    for (int i = 0;i < 360; i++)
    {
        auto rad = Collider::radians(i);
        w = std::max(w,(M*abs(sinf(rad)) + M*abs(cosf(rad)) + (M/2)*abs(cosf(rad))));
    };
    Box = BoundingBox(C.x,C.y,C.z,w,w,w);
    
    if (t)
    {
        Size = dim;
        BBoxSize = w;
        moveTo(Box.Center);
    }
    applyTranslations();
}
void Shape::onScale()
{
    generate(Position,Size*getScale(),false);
    sendMoveSignal();
}
void Shape::onMove()
{
    generate(Position,Size*getScale(),false);
    sendMoveSignal();
}
void Shape::sendMoveSignal()
{
    if (Parent != nullptr){Parent->setChanged(true);};
}
void Shape::refresh()
{
    applyTranslations();
    Translated.clear();
    for (auto v: Points)
    {
        Translated.push_back(getMatrix()->multiplyOut(v));
    }
}
void Shape::draw()
{
    drawBox();
}
void Shape::drawBox()
{
    refresh();
    Parent->DrawBox(*getMatrix(),Color);
}
// shape must be initialized before this is called otherwise may produce undefined results.
void Shape::remove()
{
    Parent->removeShape(shared_from_this());
    removed = true;
}

bool Shape::collidesLast(std::shared_ptr<Shape> B)
{
     for (auto v: Collisions)
     {
         if (v.get() == B.get())
         {
             return true;
         }
     }
     return false;
}
void Shape::cleanCollisions()
{
    Utils::erase_if(&Collisions,[](std::shared_ptr<Shape> a){return a->removed;});
}
void Shape::push(std::shared_ptr<Shape> B)
{
    if (not Utils::find_if(&Collisions,[B](std::shared_ptr<Shape> a){return a.get() == B.get();}))
    {
        Collisions.push_back(B);
        OnCollide(B.get());
    }
    Color = Vertex(1,0,0);
}
void Shape::pop(std::shared_ptr<Shape> B)
{
    if (Utils::find_if(&Collisions,[B](std::shared_ptr<Shape> a){return a.get() == B.get();}))
    {
        Utils::erase_if(&Collisions,[B](std::shared_ptr<Shape> a){return a.get() == B.get();});
        OnRelease(B.get());
    }
    if (Collisions.size() == 0){Color = Vertex(0,0,1);};
}
Shape::~Shape()
{
    //dtor
}

// AABB related functions.
Vertex Shape::getMaxDimensions()
{
    return Box.getMaxDimensions();
};
bool Shape::collidesWith(std::shared_ptr<Shape> B)
{
    if (Box.collidesWith(B->getBox()))
    {
        refresh();
        B->refresh();
        GJK test = GJK();
        if (test.collidesWith(this,B.get()))
        {
            return true;
        };
    };
    return false;
}
bool Shape::contains(Vertex A)
{
    if (Box.contains(A))
    {
        refresh();
        GJK test = GJK();
        if (test.collidesWith(this,A))
        {
            return true;
        };
    };
    return false;
}
bool Shape::boxCollidesWith(BoundingBox *B)
{
    if (Box.collidesWith(B))
    {
        return true;
    };
    return false;
};
Vertex Shape::support(Vertex dir)
{
    refresh();
    std::vector<Vertex> points;
    if (Points.size() == 0)
    {points = Box.Points;}
    else
    {points = Translated;};
    Vertex Center = points[0];
    float maxDot = Center.dot(dir);
    Vertex maxPoint = Center;
    // translated points go here ->
    for (auto v: points)
    {
        float dot = v.dot(dir);
        if (dot > maxDot)
        {
            maxDot = dot;
            maxPoint = v;
        }
    }
    return maxPoint;
}
