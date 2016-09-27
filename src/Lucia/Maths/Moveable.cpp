#include "Lucia\Maths\Moveable.h"
using namespace Lucia;
using namespace Maths;
typedef Maths::Moveable Moveable;
void Moveable::setDimensions(Vertex v)
{
    Dimensions = v;
    scaleTo(Vertex(1,1,1));
}
void Moveable::rotate(Vertex p)
{
    //apply rotation one by one.
    auto pitch = Quaternion(Vertex(p.x,0.0f,0.0f));
    auto yaw = Quaternion(Vertex(0.0f,p.y,0.0f));
    auto roll = Quaternion(Vertex(0.0f,0.0f,p.z));
    if (Rotation == Quaternion(0,0,0,0))
    {
        rotateTo(pitch*yaw*roll);
        
    }else
    {
        rotateTo(pitch*Rotation*yaw*roll);
    }
    
};
void Moveable::rotate(Quaternion q){rotateTo(q*Rotation);}

void Moveable::rotateTo(Quaternion q)
{
    if (can_rotate)
    {
        Rotation = q;
        rotation_changed=true;
        needs_refresh = true;
        if (applyTranslationWithoutRequest)
        {
            applyTranslations();
        }
    }
}

void Moveable::moveOffsetTo(Vertex v)
{
    if (can_move)
    {
        Offset = v;
        position_changed = true;
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
void Moveable::moveToLocal(Vertex v)
{
    
    auto Local       = (Rotation*Vertex(1.0f,0.0f,0.0f)).normalize();
    auto Up          = (Rotation*Vertex(0.0f,1.0f,0.0f)).normalize();
    auto Forward     = (Rotation*Vertex(0.0f,0.0f,1.0f)).normalize();
    
    Position += Up*v.y;
    Position += Local*v.x;
    Position += Forward*v.z;
    moveTo(Position);
}
bool Moveable::applyTranslations(bool b)
{
    if (needs_refresh or b)
    {
        // scale * point rotation * rotation * translation
        Model_Matrix = Matrix<4>();
        
        Vertex v = Dimensions*Scale;
        auto rot = Rotation.toAxis();
        Model_Matrix.translate(Offset);
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
        if (t){onMorph();OnMorph();};
        needs_refresh=false;
        return true;
    }
    return false;
}
