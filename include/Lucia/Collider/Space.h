#ifndef MIKUS_LUCIA_COLLIDER_SPACE_H
#define MIKUS_LUCIA_COLLIDER_SPACE_H
#include <Lucia/Collider/Collider.h>
#include <Lucia/Collider/BoundingBox.h>
#include <Lucia/Utils/Utils.h>
namespace Lucia{
    namespace Collider{
        class Space: public std::enable_shared_from_this<Space>
        {
            public:

                Space(Vertex Center,Vertex Dimensions); // the degree to which you wish to divide
                void remove(Shape* shape);
                void clear();
                void draw(std::function<void(Vertex,Vertex)>);
                void add(std::shared_ptr<Shape> shape);
                bool isEmpty();
                std::vector<std::shared_ptr<Space>> subdivide();
                ~Space();

                Lucia::Collider::BoundingBox Box;
                std::vector<std::shared_ptr<Shape>> Shapes; // the shapes within the space.
                std::vector<std::shared_ptr<Space>> Children; // the children of the space.
                std::shared_ptr<Space> Parent=nullptr;
                int degree =0;
                bool changed = true;
                bool ghost = false;
        };
    }
}
#endif 