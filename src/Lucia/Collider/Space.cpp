#include "Lucia/Collider/Space.h"
#include <Lucia/Collider/Shape.h>
using namespace Lucia;
using namespace Utils;
using namespace Collider;

Space::Space(Vertex Center,Vertex dim)
{
    Box = BoundingBox(Center.x,Center.y,Center.z,dim.x,dim.y,dim.z);
}
Space::~Space()
{
    
}
std::vector<std::shared_ptr<Space>> Space::subdivide()
{
    if (Children.size() == 0)
    {
        //create 4 new spaces.
        auto myCenter = Box.Center - Box.Dimensions/2;
        auto increment = Box.Dimensions/2.0f;
        for (unsigned int x=1; x < 3; x++)
        {
            for (unsigned int y=1; y < 3; y++)
            {
                for (unsigned int z=1; z < 3; z++)
                {
                    
                    Vertex dimensions = (Vertex(x,y,z)*increment);
                    dimensions.validate();
                    
                    Vertex Center = (myCenter + dimensions) - increment/2.0f;
                    auto space = std::make_shared<Space>(Center,increment);
                    space->Parent = shared_from_this();
                    Children.push_back(space);
                }
            }
        }
    }else 
    {
        std::vector<std::shared_ptr<Space>> Spaces; 
        for (auto v: Children)
        {
            auto nv = v->subdivide();
            for (auto shape: nv)
            {
                Spaces.push_back(shape);
            }
        }
        return Spaces;
    }
    return Children;
}
void Space::clear()
{
    Shapes.clear();
    for (auto v: Children)
    {
        v->clear();
    }
    Parent = nullptr;
    Children.clear();
}
void Space::remove(Shape* shape)
{
    Utils::erase_if(&Shapes,[shape](std::shared_ptr<Shape> a){return a.get() == shape;});
}
void Space::add(std::shared_ptr<Shape> shape)
{
    Shapes.push_back(shape);
}
bool Space::isEmpty()
{
    if (Children.size() == 0)
    {
        for (auto v: Children)
        {
            if (not v->isEmpty())
            {
                return false;
            }
        }
        return true;
        
    }else{return false;}
};
void Space::draw(std::function<void(Vertex,Vertex)> fn)
{
    fn(Box.Center,Box.Dimensions);
    for (auto space: Children)
    {
        space->draw(fn);
    }
    for (auto shape: Shapes)
    {
        shape->draw();
    }
};