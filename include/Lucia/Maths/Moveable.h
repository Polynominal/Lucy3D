#ifndef MIKUS_LUCIA_MATHS_MOVEABLE_H
#define MIKUS_LUCIA_MATHS_MOVEABLE_H
#include <Lucia\Maths\Vertex.h>
#include <Lucia\Maths\Matrix.h>
#include <Lucia\Maths\Quaternion.h>
namespace Lucia{
namespace Maths{
       class Moveable
        {
            public:
                Moveable(){};
                virtual ~Moveable(){};
                // [*]you should ask neil armstrong why this is bad.[*]
                // The moon landing had a few issues because of this! [^]
                virtual Quaternion toQuatFromEular(Vertex angle)
                {
                    return Quaternion(angle);
                };
                virtual Vertex toEularFromQuat(Quaternion q)
                {
                    return q.toAxis();
                };

                virtual void move(Vertex p){moveTo(Position + p);};
                virtual void move(float x,float y,float z=0.0f){move(Vertex(x,y,z));};

                virtual void moveTo(Vertex p);
                virtual void moveTo(float x,float y,float z=0.0f){moveTo(Vertex(x,y,z));};

                virtual void rotate(Vertex p)
                {
                    rotateTo(toEularFromQuat(Rotation) + p);
                };
                virtual void rotate(float x,float y,float z=0.0f){rotate(Vertex(x,y,z));};
                
                virtual void rotateTo(Vertex p)
                {
                    rotateTo(toQuatFromEular(p));
                };
                virtual void rotateTo(float x,float y,float z=0.0f){rotateTo(Vertex(x,y,z));};

                virtual void rotate(Quaternion q){rotateTo(Rotation + q);};
                virtual void rotateTo(Quaternion q);

                virtual void scale(Vertex p){scaleTo(Scale + p);};
                virtual void scale(float x,float y,float z=0.0f){scale(Vertex(x,y,z));};

                virtual void scaleTo(Vertex p);
                virtual void scaleTo(float x,float y,float z=1.0f){scaleTo(Vertex(x,y,z));};

                virtual void setDimensions(Vertex p);
                virtual void setDimensions(float x,float y,float z){setDimensions(Vertex(x,y,z));};
                
                virtual bool applyTranslations(bool force = false); // force = apply Vertex'es to Matrix even if we havent moved.
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
                void enableImplicitTranslation(bool b){applyTranslationWithoutRequest = b;};
                // Get
                Matrix<4>* getModelMatrix(){return &Model_Matrix;};
                Matrix<4>* getMatrix(){return &Model_Matrix;};
                Vertex getDimensions(){return Dimensions*Scale;};
                Vertex getPosition(){return Position;};
                Vertex getScale(){return Scale;};
                Vertex getRotation(){return toEularFromQuat(Rotation);};
                Quaternion getQuaternion(){return Rotation;};
                // user functions
                std::function<void()> OnMove   = [](){};
                std::function<void()> OnRotate = [](){} ;
                std::function<void()> OnScale  = [](){};
                std::function<void()> OnMoprh  = [](){};

            protected:

                Matrix<4> Model_Matrix = Matrix<4>(4);

                Quaternion Rotation = Quaternion(0,0,0,0);
                Vertex Position  = Vertex(0.0f,0.0f,0.0f);
                Vertex Scale     = Vertex(1.0f,1.0f,1.0f);
                Vertex Dimensions = Vertex(1.0f,1.0f,1.0f);

            private:
                bool applyTranslationWithoutRequest = false;
                bool can_rotate = true;
                bool can_scale  = true;
                bool can_move   = true;

                bool position_changed = false;
                bool scale_changed = false;
                bool rotation_changed = false;

                bool needs_refresh=false;
        };
}}

#endif