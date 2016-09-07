#ifndef MIKUS_LUCIA_COLLIDER_ALGORITHM_OCTTREE_H
#define MIKUS_LUCIA_COLLIDER_ALGORITHM_OCTTREE_H
#include <Lucia/Collider/Collider.h>
#include <Lucia/Collider/Space.h>
#include <Lucia/Collider/BoundingBox.h>

namespace Lucia
{
    namespace Collider{
        namespace Algorithm
        {
            class Octtree
            {
                public:
                    friend class Shape;
                    Octtree();
                    virtual ~Octtree();
                    
                    void setChanged(bool c){changed = c;};
                    
                    void addSingle(std::shared_ptr<Space> space, std::shared_ptr<Shape> shape);
                    void addShape(std::shared_ptr<Shape> target);
                    void addToSpace(std::shared_ptr<Space> space, std::shared_ptr<Shape> shape);
                    
                    void removeShape(Shape* target);
                    void form();
                    void update();
                    
                    void checkColisionsOnce(std::shared_ptr<Space> space,std::shared_ptr<Shape> shape,bool check_parent=true);
                    void checkCollisions(std::shared_ptr<Space> space);
                    void rayCast(Vertex a,Vertex b,std::function <void(Shape *B)> collide,bool infinite_depth=false);
                    std::pair<float,float> getDepthLimits();
                    
                    float precision = 0.01f;
                    bool changed = true;
                    std::shared_ptr<Space> Root;
                    std::vector<std::shared_ptr<Shape>> Shapes;
            };
        }
    }
}
#endif