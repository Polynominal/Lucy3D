#ifndef LUCIA_MERGER_GRAPHICSTOCOLLIDER
#define LUCIA_MERGER_GRAPHICSTOCOLLIDER
#include <Lucia/Graphics/Collider.h>
#include <Lucia/Graphics/Scene/Model.h>
#include <Lucia/Collider/Shape/Polygon.h>
namespace Lucia
{
    namespace Graphics 
    {
        std::shared_ptr<Collider::Shape::Polygon> CreateShape(std::shared_ptr<Collider::Graphics::Model> m);
    };
    namespace Collider
    {
        
    };
}
#endif 