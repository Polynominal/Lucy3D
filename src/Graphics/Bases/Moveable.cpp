#include "Graphics\Bases\Moveable.h"
typedef Graphics::Base::Moveable Moveable;
glm::quat addDegree(glm::quat Q,float size,int dir)
{
    switch(dir)
    {
    case 0:
        {
            return glm::angleAxis(1.0f,glm::vec3(size,0.0f,0.0f));
        }
    case 1:
        {
            return glm::angleAxis(1.0f,glm::vec3(0.0f,size,0.0f));
        }
    case 3:
        {
            return glm::angleAxis(1.0f,glm::vec3(0.0f,0.0f,size));
        }
    }
}
void Moveable::rotateTo(glm::quat q)
{
    if (can_rotate)
    {
        Rotation = q;
        rotation_changed=true;
        needs_refresh = true;
    }

}
void Moveable::moveTo(glm::vec3 v)
{
    if (can_move)
    {
        Position = v;
        position_changed = true;
        needs_refresh = true;
    }
}
void Moveable::scaleTo(glm::vec3 v)
{
    if (can_scale)
    {
        Scale = v;
        scale_changed = true;
        needs_refresh = true;
    }
}
void Moveable::applyTranslations()
{
    if (needs_refresh)
    {
        Model_Matrix = glm::mat4_cast(Rotation);
        Model_Matrix = glm::scale(Model_Matrix,Scale);
        Model_Matrix = glm::translate(Model_Matrix,Position);

        if (position_changed)
        {
            position_changed=false;
            OnMove();
            onMove();
        }
        if (scale_changed)
        {
            scale_changed=false;
            OnScale();
            onScale();
        }
        if (rotation_changed)
        {
            rotation_changed=false;
            OnRotate();
            onRotate();
        }

        needs_refresh=false;
    }
}
