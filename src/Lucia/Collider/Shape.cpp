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
void Shape::synthesize(Vertex C,Vertex dim,bool t)
{
    // find the maximum dimension this is done to create the largest possible cube!
    enableImplicitTranslation(true);
    Points = BoundingBox(0,0,0,1,1,1).Points;
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
        moveTo(Box.Center);
    }
    applyTranslations();
}
void Shape::onScale()
{
    synthesize(Position,getDimensions(),false);
    sendMoveSignal();
}
void Shape::onMove()
{
    synthesize(Position,getDimensions(),false);
    sendMoveSignal();
}
void Shape::sendMoveSignal()
{
    if (space.get())
    {
        synthesize(Position,getDimensions(),false);
        Parent->moveShape(shared_from_this());   
    }else
    {
        std::cout << "Space is nil!" << std::endl;
    }
}
void Shape::refresh()
{
    if (needsRefresh())
    {
        applyTranslations();
        Translated.clear();
        for (auto v: Points)
        {
            Translated.push_back(getMatrix()->multiplyOut(v));
        }
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
    Collisions.push_back(B);
    OnCollide(B.get());
}
void Shape::pop(std::shared_ptr<Shape> B)
{
    Utils::erase_if(&Collisions,[B](std::shared_ptr<Shape> a){return a.get() == B.get();});
    OnRelease(B.get());
}
Shape::~Shape()
{
    //dtor
}

// BOUNDING BOX

// 6-------------5
// |##           ##
// |   ##          ##
// |     ##          ##
// 7        1-----------2
//  ##      |     4#    |
//    ##    |       ##  |
//      ##  |         ##|
//        ##0-----------3

BoundingBox::BoundingBox(){};
BoundingBox::BoundingBox(float x,float y,float z,float w,float h,float d)
{
    Center = Vertex(x,y,z);
    Dimensions = Vertex(w,h,d);
    refresh();
}
bool BoundingBox::contains(float x,float y,float z)
{
    bool collides = false;
    if (std::abs(Center.x - x)*2 < (Dimensions.x + 1)&&
        std::abs(Center.y - y)*2 < (Dimensions.y + 1)&&
        std::abs(Center.z - z)*2 < (Dimensions.z + 1))
    {
        collides = true;
    };
    return collides;
}
void BoundingBox::refresh()
{
    // make sure you set either Dimensions or Center before calling refresh!
    float w = Dimensions.x;
    float h = Dimensions.y;
    float d = Dimensions.z;
    float x = Center.x;
    float y = Center.y;
    float z = Center.z;
    
    Points.clear();
    Faces.clear();
    Points.push_back(Vertex(x - w/2,y - h/2,z - d/2)); //
    Points.push_back(Vertex(x - w/2,y + h/2,z - d/2));
    Points.push_back(Vertex(x + w/2,y + h/2,z - d/2));
    Points.push_back(Vertex(x + w/2,y - h/2,z - d/2));
    Points.push_back(Vertex(x + w/2,y -h/2,z + d/2));
    Points.push_back(Vertex(x + w/2,y +h/2,z + d/2));
    Points.push_back(Vertex(x - w/2, y + h/2, z + d/2));
    Points.push_back(Vertex(x - w/2,y -h/2,z + d/2));

    Max = Vertex(Points[0].x,Points[1].y,Points[4].z);
    Min = Vertex(Points[3].x,Points[7].y,Points[3].z);

    w++;
    h++;
    d++;
    Faces.push_back(Vertex(x,y,z + h/2)); // front
    Faces.push_back(Vertex(x,y,z -h/2)); // back
    Faces.push_back(Vertex(x-w/2,y,z)); // left
    Faces.push_back(Vertex(x+w/2,y,z)); // right
    Faces.push_back(Vertex(x,y + h/2,z)); // top
    Faces.push_back(Vertex(x,y-h/2,z)); // bottom

    Transformation = Matrix<4>();
    Transformation.scale(Dimensions);

    //Transformation.rotate(Rotation); - to be used with

    Transformation.translate(Center);
}
Vertex BoundingBox::getMaxDimensions()
{
    float w = 0;
    float M = Dimensions.max();
    unsigned int deg = 360/2;
    auto rad = Collider::radians(deg);
    w = std::max(w,(M*abs(sinf(rad)) + M*abs(cosf(rad)) + (M/2)*abs(cosf(rad))));
    return Vertex(w,w,w);
}
bool BoundingBox::collidesWith(BoundingBox *B)
{
    auto Max2 = B->Max;
    auto Min2 = B->Min;
    bool collides = false;

    auto Center2 = B->Center;
    if (std::abs(Center.x - Center2.x)*2 < (Dimensions.x + B->Dimensions.x)&&
        std::abs(Center.y - Center2.y)*2 < (Dimensions.y + B->Dimensions.y)&&
        std::abs(Center.z - Center2.z)*2 < (Dimensions.z + B->Dimensions.z))
    {
        collides = true;
    };
    return collides;
}
bool BoundingBox::engulfs(BoundingBox *B)
{
    if (Faces.size() == 0)
    {
        return false;
    }
    for (auto v : Faces)
    {
        if  (!B->contains(v))
        {
            return false;
        }
    }
    return true;
}
BoundingBox::~BoundingBox()
{

}



// AABB related functions.
Vertex Shape::getMaxDimensions()
{
    return Box.getMaxDimensions();
};
bool Shape::collidesWith(std::shared_ptr<Shape> B)
{
    if (space->ghost || Box.collidesWith(B->getBox()))
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
void Shape::setSpace(std::shared_ptr<Space> s)
{
    std::cout << "Setting Shape!" << std::endl;
    space = s;
};
//SPACE
Space::Space(Vertex pos,int degree)
{
    Box = BoundingBox(pos.x,pos.y,pos.z,degree,degree,degree);
}
// ghost space, used for raycasters and other devils
Space::Space(Vertex pos,Vertex d)
{
    Box = BoundingBox(pos.x,pos.y,pos.z,d.x,d.y,d.z);
    ghost = true;
}
void Space::setGhost(bool t)
{
    ghost = t;
}
void Space::erase(Collider::Manager &Manager)
{

}
Space::~Space()
{

}
