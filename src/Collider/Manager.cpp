#include "Manager.h"
#include <Collider/Shape.h>

#include <Collider/Shape/Polygon.h>
#include <Collider/Shape/Point.h>
#include <Collider/Shape/Sphere.h>
#include <Collider/Shape/Ray.h>

using namespace Collider;
using namespace Collider::Tools;

Manager::Manager()
{
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
void Manager::addGhost(std::shared_ptr<Shape> A,Vertex dim)
{
    std::shared_ptr<Space> connected_space = std::make_shared<Space>(A->Box.Center,dim);
    connected_space->Shapes.push_back(A);
    A->space = connected_space;
    Spaces.push_back(connected_space);
}
void Manager::addToSpace(std::shared_ptr<Shape>A,int degree)
{
    // the map in this case is representative of the scale so that we can completely ignore scale.
    // The only way that scale is relevant is when in concern of Fusion! aka 2 small boxes joining to one BIG box

    // now this delta is a little heavy so make sure to only call it when necessary!
    int size = degree*minimumSize;
    std::shared_ptr<Space> connected_space = std::make_shared<Space>(A->Box.Center,size);
    connected_space->degree = degree;
    connected_space->Shapes.push_back(A);
    A->space = connected_space;
    Spaces.push_back(connected_space);
}
// find the exteremes of a few shapes
void Manager::scanCollisions(std::vector<std::shared_ptr<Shape>> Shapes)
{
    for (unsigned int i=0;i < Shapes.size();i++)
    {
        auto Shape1 = Shapes[i];
        Shape1->OnChange();
        for (unsigned int j=i+1;j < Shapes.size();j++)
        {
            auto Shape2 = Shapes[j];
            if ((Shape1.get() != Shape2.get()))
            {
                if (Shape1->collidesWith(Shape2))
                {
                    if (!Shape1->collidesLast(Shape2))
                    {
                        Shape1->push(Shape2);
                        OnCollision(Shape1.get(),Shape2.get());
                    }
                }else
                {
                    if (Shape1->collidesLast(Shape2))
                    {
                        Shape1->pop(Shape2);
                        OnRelease(Shape1.get(),Shape2.get());
                    }
                }
            }
        }
    }
}
void Manager::scanShapes()
{
    for (unsigned int i=0; i < Spaces.size(); i++)
    {
        for (unsigned int j=i+1; j < Spaces.size(); j++)
        {
            auto Space1  = Spaces[i];
            auto Space2 = Spaces[j];
            if (Space1->Box.collidesWith(&Space2->Box))
            {
                if (Space1->ghost || Space2->ghost) // if space is a ghost, we must check all of the contents if they intersect, otherwise procede.
                {
                        std::vector<std::shared_ptr<Shape>> Shapes;
                        for (auto v: Space2->Shapes)
                        {
                            Shapes.push_back(v);
                        }
                        for (auto v : Space1->Shapes)
                        {
                            Shapes.push_back(v);
                        };
                        scanCollisions(Shapes);
                }else
                {
                    std::map<std::shared_ptr<Shape>,int> Shapes;
                    for (auto v: Space2->Shapes)
                    {
                        Shapes.insert({v,1});
                    }
                    for (auto v : Space1->Shapes)
                    {
                        Shapes.insert({v,1});
                    }

                    Vertex Max = Vertex(0,0,0);
                    Vertex Min = Vertex(0,0,0);
                    bool set = false;
                    for (auto M: Shapes)
                        {
                            auto s = M.first->Box;
                            for (auto vert: s.Points)
                            {
                                if (!set)
                                {
                                    Max = vert;
                                    Min = vert;
                                    set = true;
                                }
                                Max.x = std::max(vert.x,Max.x);
                                Max.y = std::max(vert.y,Max.y);
                                Max.z = std::max(vert.z,Max.z);

                                Min.x = std::min(vert.x,Min.x);
                                Min.y = std::min(vert.y,Min.y);
                                Min.z = std::min(vert.z,Min.z);

                            }
                        };
                    Vertex Dimensions = Max - Min;
                    Vertex Center = Max - Dimensions/2;

                    int approximate_degree = std::max(Dimensions.x,std::max(Dimensions.y,Dimensions.z))/minimumSize;
                    int size = minimumSize*approximate_degree;
                    while(Vertex(size,size,size) < Dimensions)
                    {
                        approximate_degree++;
                        size = approximate_degree * minimumSize;
                    }
                        approximate_degree++;
                        size = approximate_degree * minimumSize;
                    Spaces.erase(Spaces.begin() + j);
                    for (unsigned int n=0; n < Spaces.size(); n++)
                    {
                        if (Spaces[n].get() == Space1.get())
                        {
                            Spaces.erase(Spaces.begin() + n);
                        }
                    }
                    auto space = std::make_shared<Space>(Center,size);
                    for (auto v: Shapes)
                    {
                        auto Shape = v.first;
                        Shape->space = space;
                        space->Shapes.push_back(Shape);
                    }
                    Spaces.push_back(space);
                    scanShapes();
                }
            }
        }
    }
}
void Manager::update()
{
    // we need to find all the spaces without bias of size.

    bool requiresUpdate = false;
    for (auto v: Spaces)
    {
        if (v->changed)
        {
            auto Shapes = v->Shapes;
            for (auto v: Shapes)
            {
                auto Shape = v;
            }
            requiresUpdate = true;
        }
    }
    if (requiresUpdate)
    {
        scanShapes();
        for (auto v: Spaces)
        {
            if (!v->ghost && v->changed)
            {
                scanCollisions(v->Shapes);
            }
             v->changed = false;
        }
    }
    // check the shape collisions within the shapes.
}
int Manager::addShape(std::shared_ptr<Shape>A)
{

    int approximate_degree = A->Box.Dimensions.max()/minimumSize;
    int size = approximate_degree * minimumSize;

    while(Vertex(size,size,size) < A->Box.Dimensions)
    {
        approximate_degree++;
        size = approximate_degree * minimumSize;
    }
    addToSpace(A,approximate_degree);
    return approximate_degree;
}
// must be initialized shapes!!!
// Otherwise undefined behaviour occurs
void Manager::removeShape(std::shared_ptr<Shape>A)
{
   A->Color = Vertex(1.0,0.0,0.0);
    // time for seperation!
    auto space = A->space;
    if (!space->ghost)
    {
        Vertex Max = Vertex(0,0,0);
        Vertex Min = Vertex(0,0,0);
        bool set = false;
        bool empty = true;
        for (auto M: space->Shapes)
        {
            if (M.get() != A.get())
            {
                auto s = M->Box;

                for (auto vert: s.Points)
                {
                    if (!set)
                    {
                        Max = vert;
                        Min = vert;
                        set = true;
                        empty = false;
                    }
                    Max.x = std::max(vert.x,Max.x);
                    Max.y = std::max(vert.y,Max.y);
                    Max.z = std::max(vert.z,Max.z);

                    Min.x = std::min(vert.x,Min.x);
                    Min.y = std::min(vert.y,Min.y);
                    Min.z = std::min(vert.z,Min.z);

                }
            }
        };
        if (!empty)
        {
            Vertex Dimensions = (Max - Min);
            Vertex Center = Max - Dimensions/2;

            int approximate_degree = std::max(Dimensions.x,std::max(Dimensions.y,Dimensions.z))/minimumSize;
            int size = minimumSize*approximate_degree;
            while(Vertex(size,size,size) < Dimensions)
            {
                approximate_degree++;
                size = approximate_degree * minimumSize;
            }
            Center = Max - Vertex(size,size,size)/2;
            auto new_space = std::make_shared<Space>(Center,size);
            new_space->degree = approximate_degree;
            // erase the space!
            for (auto v: space->Shapes)
            {
                if (v.get() != A.get())
                {
                    v->space = new_space;
                    new_space->Shapes.push_back(v);
                }
            }
            Spaces.push_back(new_space);
        }
    }
    for (unsigned int n=0; n < Spaces.size(); n++)
    {
        if (Spaces[n].get() == space.get())
        {
            Spaces.erase(Spaces.begin() + n);
        }
    }
}
void Manager::moveShape(std::shared_ptr<Shape> A)
{
    if (!A->Box.engulfs(&A->space->Box))
    {
        // we need to remove the shape and then add it again to prevent indexing of non-existent shapes and to reset space
        removeShape(A);
        if (A->space->ghost)
        {
            addGhost(A,A->Dimensions);
        }else
        {
            addShape(A);
        }
    }else
    {
        A->space->changed = true;
        A->Color = Vertex(0.0,0.0,0.0);
    }
}
void Manager::drawChunks(bool blocks)
{
    PreDraw();
    for (auto var: Spaces)
    {
        auto Box = var->Box;
        DrawBox(Box.Transformation,Vertex(0.0,0,1.0));
        for (auto v : var->Shapes)
        {
            v->draw();
        }
    }
    PostDraw();
}
std::shared_ptr<Polygon> Manager::addPolygon(std::vector<Vertex>Points)
{
    auto a = std::make_shared<Polygon>();
    a->make(this,Points);
    a->polygonID = CreatePolygon(Points);
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
std::shared_ptr<Shape> Manager::addBox(float x,float y,float z,float w,float h,float d)
{
    auto shape = std::make_shared<Shape>(this);
    shape->synthesize(Vertex(x,y,z),Vertex(w,h,d));
    shape->Dimensions = Vertex(w,h,d);
    shape->Position = Vertex(x,y,z);
    shape->update_translation(false);
    addShape(shape);
    return shape;
}
std::vector<std::shared_ptr<Shape>> Manager::cast(Vertex A,Vertex B)
{
    std::vector<std::shared_ptr<Shape>> collisions;
    auto a = std::make_shared<Ray>();
    a->make(this,A,B);
    addGhost(a,a->Dimensions);
    auto Space = a->space;
    for (auto v: Spaces)
    {
        if (v.get() != Space.get() && Space->Box.collidesWith(&v->Box))
        {
            for (auto shape: v->Shapes)
            {
                if (shape->collidesWith(a))
                {
                    collisions.push_back(shape);
                }
            }
        }
    }
    removeShape(a);
    return collisions;
}
Manager::~Manager()
{
    //dtor
}
