#include "Lucia\Maths\Moveable.h"

using namespace Lucia;
using namespace Maths;
typedef Maths::Moveable Moveable;
void Moveable::setDimensions(Vertex v)
{
    Dimensions = v;
    scaleTo(Vertex(1,1,1));
}
void Moveable::pitch(float angle,Vertex local)
{
    angle = Maths::getAngle(angle);
    if (angle > 0)
    {
        Rotation = Rotation.pitch(angle,local,Forward,Up);
    }
    rotateTo(Rotation);
}
void Moveable::yaw(float angle,Vertex upper)
{
    angle = Maths::getAngle(angle);
    if (angle > 0)
    {
        Rotation = Rotation.yaw(angle,Local,Forward,upper);
    }
    rotateTo(Rotation);
}
void Moveable::roll(float angle,Vertex forward)
{
    angle = Maths::getAngle(angle);
    if (angle > 0)
    {
        Rotation = Rotation.roll(angle,Local,forward,Up);
    }
    rotateTo(Rotation);
}
void Moveable::rotate(Vertex p)
{
    auto angle = p.loop(0.0f,360.0f);
    if (Rotation == Quaternion(0,0,0,0))
    {
        Rotation = Quaternion(Vertex(0,0,0));
    }
    if (angle.x > 0)
    {
        Rotation = Rotation.pitch(angle.x,Local,Forward,Up);
    }
    if (angle.y > 0)
    {
        Rotation = Rotation.yaw(angle.y,Local,Forward,Up);
    }
    if (angle.z > 0)
    {
        Rotation = Rotation.roll(angle.z,Local,Forward,Up);
    };
    rotateTo(Rotation);
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
    Position += Local*v.y;
    Position += Up*v.x;
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
        Model_Matrix.translate(Offset);
        Model_Matrix = Model_Matrix*Rotation.toMatrix();
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
