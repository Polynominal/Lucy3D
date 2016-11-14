#include "Lucia\Collider\Algorithm\Octtree.h"
#include <Lucia/Collider/Shape.h>
#include <Lucia/Collider/Shape/Ray.h>

using namespace Lucia;
using namespace Collider;
using namespace Algorithm;

Octtree::Octtree()
{
    
}
Octtree::~Octtree()
{
    
}
void Octtree::addShape(std::shared_ptr<Shape> target)
{
    Shapes.push_back(target);
    changed = true;
};
void Octtree::removeShape(Shape* target)
{
    Utils::erase_if(&Shapes,[target](std::shared_ptr<Shape> A){return A.get() == target;});
}
void Octtree::form()
{
    auto m = std::numeric_limits<float>::min();
    auto n = std::numeric_limits<float>::max();
    Vertex max = Vertex(m,m,m);
    Vertex min = Vertex(n,n,n);
    //get the minimum poitns to establish the space we are going to use.
    for (auto v: Shapes)
    {
        auto bbox = v->getBox();
        auto dims = bbox->getMaxDimensions();
        auto center = bbox->Center;
        
        auto top = center + dims/2;
        auto bottom = center - dims/2;
        
        if (max.x < top.x) {max.x = top.x;};
        if (max.y < top.y){max.y = top.y;};
        if (max.z < top.z){max.z = top.z;};
        
        if (min.x > bottom.x){min.x = bottom.x;};
        if (min.y > bottom.y){min.y = bottom.y;};
        if (min.z > bottom.z){min.z = bottom.z;};
        
    }
    //find the lowest variable between all of them and use that.
    auto dim = max - min;
    auto center = min + dim/2.0f;
    
    
    //[/ ! \] MEMORY LEAK PROTECTION DO NOT REMOVE [/ ! \]
    //The way this works is it deletes a vector of double referenced shared pointers therefore remvoing any chance of
    //double reference hell, therefore freeing the memeory.
    if (Root.get())
    {
        Root->clear();
    };
    //[/ ! \][/ ! \]
    
    Root = std::make_shared<Space>(center,dim);;
    
    for (auto v: Shapes)
    {
        addToSpace(Root,v);
    }
    changed = false;
};
void Octtree::addSingle(std::shared_ptr<Space> space, std::shared_ptr<Shape> shape)
{
    int count = 0;
    shape->refresh();
    std::shared_ptr<Space> host;
    for (auto v: space->Children)
    {
        if (v->Box.collidesWith(shape->getBox()))
        {
            host = v;
            count++;
        }
    }
    if (count > 2)
    {
        space->add(shape);
    }else 
    {
        addToSpace(host,shape);
    }
}
void Octtree::addToSpace(std::shared_ptr<Space> space, std::shared_ptr<Shape> shape)
{
    if (space->Shapes.size() > 1
    and shape->getDimensions() < (space->Box.Dimensions/2.0f)
    and space->Box.Dimensions.x > precision)
    {
        if (space->Children.size() == 0)
        {
            space->subdivide();
        };
        std::vector<std::shared_ptr<Shape>> old;
        for (auto v: space->Shapes)
        {
            old.push_back(v);
        }
        space->Shapes.clear();
        for (auto shp: old)
        {
            addSingle(space,shp);
        }
        addSingle(space,shape);
    }
    else 
    {
        space->add(shape);
    }
};
void Octtree::update()
{
    if (changed)
    {
        form(); // MEMORY LEAK!!!
        changed = false;
        checkCollisions(Root);
    }
};
void Octtree::rayCast(Vertex a,Vertex b,std::function <void(Shape *B)> c,bool infinite_depth)
{
    auto ray = std::make_shared<Ray>();
    ray->OnCollide = c;
    if (infinite_depth)
    {
        auto box = Root->Box;
        a.z = (box.Center - box.Dimensions/2.0f).z;
        b.z = (box.Center + box.Dimensions/2.0f).z;
    }
    ray->make(nullptr,a,b);
    checkColisionsOnce(Root,ray);
}
std::pair<float,float> Octtree::getDepthLimits()
{
    if (Root.get())
    {
        auto box = Root->Box;
        return std::pair<float,float>((box.Center - box.Dimensions/2.0f).z,(box.Center + box.Dimensions/2.0f).z);
    }else
    {
        return std::pair<float,float>(0,1);
    }
    
    
}
void Octtree::checkColisionsOnce(std::shared_ptr<Space> space,std::shared_ptr<Shape> shape,bool check_parent)
{
    if (space.get() == nullptr){return;};
    if (shape.get() == nullptr){return;};
    auto shapes = space->Shapes;
    for (auto v: shapes)
    {
        if (shape != v)
        {
            v->refresh();
            if (shape->collidesWith(v))
            {
                v->push(shape);
                shape->push(v);
            }else 
            {
                v->pop(shape);
                shape->pop(v);
            }
        }
    };
    for (auto v: space->Children)
    {
        checkColisionsOnce(v,shape,false);
    }
    if (check_parent){checkColisionsOnce(space->Parent,shape);};
}
void Octtree::checkCollisions(std::shared_ptr<Space> space)
{
    auto shapes = space->Shapes;
    for (auto v: shapes)
    {
        v->refresh();
        checkColisionsOnce(space,v);
    }
    for (auto v: space->Children){checkCollisions(v);}
};