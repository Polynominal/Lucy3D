#include "Lucia/Merger/GraphicsToCollider.h"
using namespace Lucia;
using namespace Collider;
using namespace Graphics;
std::shared_ptr<Collider::Polygon> Graphics::CreateShape(Graphics::Object::Model* model)
{
    auto ret = std::make_shared<Collider::Polygon>();
    auto points = model->getPoints();
    ret->create(points);
    return ret;
}