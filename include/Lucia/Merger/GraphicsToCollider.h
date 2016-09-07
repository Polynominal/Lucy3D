#ifndef LUCIA_MERGER_GRAPHICSTOCOLLIDER
#define LUCIA_MERGER_GRAPHICSTOCOLLIDER

#include <Lucia/Collider/Collider.h>
#include <Lucia/Collider/Shape/Polygon.h>

#include <Lucia/Graphics/Objects/Model.h>


namespace Lucia
{
    namespace Graphics 
    {
        std::shared_ptr<Collider::Polygon> CreateShape(Graphics::Object::Model* m);
    };
    namespace Collider
    {
        
    };
}
#endif 