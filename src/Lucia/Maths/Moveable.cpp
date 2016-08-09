#include "Lucia\Maths\Moveable.h"
using namespace Lucia;
using namespace Maths;
typedef Maths::Moveable Moveable;
void Moveable::setDimensions(Vertex v)
{
    Dimensions = v;
    scaleTo(Vertex(1,1,1));
}
void Moveable::rotateTo(Quaternion q)
{
    if (can_rotate)
    {
        auto axis = Rotation.toAxis();
        Rotation = q;
        rotation_changed=true;
        needs_refresh = true;
        if (applyTranslationWithoutRequest)
        {
            applyTranslations();
        }
    }
}
void Moveable::moveTo(Vertex v)
{
    if (can_move)
    {
        Position = v;
        position_changed = true;
        needs_refresh = true;
        if (applyTranslationWithoutRequest)
        {
            applyTranslations();
        }
    }
}
void Moveable::scaleTo(Vertex v)
{
    if (can_scale)
    {
        Scale = v;
        scale_changed = true; 
        needs_refresh = true;
        if (applyTranslationWithoutRequest)
        {
            applyTranslations();
        }
    }
}
bool Moveable::applyTranslations(bool b)
{
    if (needs_refresh or b)
    {
        // scale * point rotation * rotation * translation
        Model_Matrix = Matrix<4>();
        
        Vertex v = Dimensions*Scale;
        auto rot = Rotation.toAxis();
        Model_Matrix.rotate(rot);
        Model_Matrix.scale(v);
        Model_Matrix.translate(Position);
        bool t = false;
        if (position_changed or b)
        {
            t = true;
            position_changed=false;
            OnMove();
            onMove();
        }
        if (scale_changed or b)
        {
            t = true;
            scale_changed=false;
            OnScale();
            onScale();
        }
        if (rotation_changed or b)  
        {
            t = true;
            rotation_changed=false;
            OnRotate();
            onRotate();
        }
        if (t){onMorph();};
        needs_refresh=false;
        return true;
    }
    return false;
}
