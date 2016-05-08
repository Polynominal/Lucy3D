#ifndef MIKUS_GRAPHICS_MOVEABLE_H
#define MIKUS_GRAPHICS_MOVEABLE_H
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <gtx/transform.hpp>
#include <gtx/quaternion.hpp>
#include <gtc/quaternion.hpp>
#include <functional>

namespace Graphics
{
    namespace Base
    {
        class Moveable
        {
            public:
                Moveable(){};
                virtual ~Moveable(){};
                // [*]you should ask neil armstrong why this is bad.[*]
                // The moon landing had a few issues because of this! [^]
                virtual glm::quat toQuatFromEular(glm::vec3 angle)
                {
                    return glm::quat(angle);
                };
                virtual glm::vec3 toEularFromQuat(glm::quat q)
                {
                    return glm::eulerAngles(q);
                };

                virtual void move(glm::vec3 p){moveTo(Position + p);};
                virtual void move(float x,float y,float z){move(glm::vec3(x,y,z));};

                virtual void moveTo(glm::vec3 p);
                virtual void moveTo(float x,float y,float z){moveTo(glm::vec3(x,y,z));};

                virtual void rotate(glm::vec3 p)
                {
                    rotateTo(toEularFromQuat(Rotation) + p);
                };
                virtual void rotate(float x,float y,float z){rotate(glm::vec3(x,y,z));};

                virtual void rotateTo(glm::vec3 p)
                {
                    Rotation = toQuatFromEular(p);
                }
                virtual void rotateTo(float x,float y,float z){rotateTo(glm::vec3(x,y,z));};

                virtual void rotate(glm::quat q){rotateTo(Rotation + q);};
                virtual void rotateTo(glm::quat q);

                virtual void scale(glm::vec3 p){scaleTo(Scale + p);};
                virtual void scale(float x,float y,float z){scale(glm::vec3(x,y,z));};

                virtual void scaleTo(glm::vec3 p);
                virtual void scaleTo(float x,float y,float z){scaleTo(glm::vec3(x,y,z));};

                virtual void refresh();
                // on functions
                virtual void onMove(){};
                virtual void onRotate(){};
                virtual void onScale(){};
                virtual void onMorph(){};
                // can
                void canMove(bool b){can_move =b;};
                void canScale(bool b){can_scale = b;};
                void canRotate(bool b){can_rotate = b;};
                void canMorph(bool b){can_move = b;can_scale =b;can_rotate=b;};

                bool canMove(){return can_move;};
                bool canScale(){return can_scale;};
                bool canRotate(){return can_rotate;};
                bool canMorph(){return (can_move and can_scale and can_rotate);};
                //misc
                bool needsRefresh(){return needs_refresh;};
                // Get
                glm::mat4* getModelMatrix(){return &Model_Matrix;};
                // user functions
                std::function<void()> OnMove   = [](){};
                std::function<void()> OnRotate = [](){};
                std::function<void()> OnScale  = [](){};
                std::function<void()> OnMoprh  = [](){};

            protected:

                glm::mat4 Model_Matrix;

                glm::quat Rotation = glm::quat(0,0,0,0);
                glm::vec3 Position  = glm::vec3(0.0f,0.0f,0.0f);
                glm::vec3 Scale     = glm::vec3(1.0f,1.0f,1.0f);

            private:
                bool can_rotate = true;
                bool can_scale  = true;
                bool can_move   = true;

                bool position_changed = false;
                bool scale_changed = false;
                bool rotation_changed = false;

                bool needs_refresh=false;
        };
    }
}
#endif // MIKUS_GRAPHICS_MOVEABLE_H
