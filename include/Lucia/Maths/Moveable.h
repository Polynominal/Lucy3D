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
                Moveable(){Rotation = Quaternion(Vertex(0,0,0));};
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
                virtual void moveLocal(float x=0,float y=0,float z=0){moveLocal(Vertex(x,y,z));};
                virtual void moveLocal(Vertex v){moveToLocal(Position + v);};
                virtual void moveToLocal(float x=0,float y=0,float z=0){moveToLocal(Vertex(x,y,z));};
                virtual void moveToLocal(Vertex v);
                
                virtual void moveOffset(Vertex p){moveOffsetTo(Offset + p);};
                virtual void moveOffset(float x,float y,float z=0.0f){moveOffset(Vertex(x,y,z));};

                virtual void moveOffsetTo(Vertex p);
                virtual void moveOffsetTo(float x,float y,float z=0.0f){moveOffsetTo(Vertex(x,y,z));};
                
                virtual void move(Vertex p){moveTo(Position + p);};
                virtual void move(float x,float y,float z=0.0f){move(Vertex(x,y,z));};

                virtual void moveTo(Vertex p);
                virtual void moveTo(float x,float y,float z=0.0f){moveTo(Vertex(x,y,z));};

                virtual void pitch(float angle,Vertex local=Vertex(1.0f,0.0f,0.0f));
                virtual void yaw(float angle,Vertex up=Vertex(0.0f,1.0f,0.0f));
                virtual void roll(float angle,Vertex forward=Vertex(0.0f,0.0f,1.0f));
                virtual void rotate(Vertex p);
                virtual void rotate(float x,float y,float z=0.0f){rotate(Vertex(x,y,z));};
                
                virtual void rotateTo(Vertex p)
                {
                    rotateTo(toQuatFromEular(p));
                };
                virtual void rotateTo(float x,float y,float z=0.0f){rotateTo(Vertex(x,y,z));};

                virtual void rotate(Quaternion q);
                virtual void rotateTo(Quaternion q);

                virtual void scale(Vertex p){scaleTo(Scale + p);};
                virtual void scale(float x,float y,float z=0.0f){scale(Vertex(x,y,z));};

                virtual void scaleTo(Vertex p);
                virtual void scaleTo(float x,float y,float z=1.0f){scaleTo(Vertex(x,y,z));};
                //original fraction is the size of the model relative to the screen so if your model ranges from 0.5 to -0.5 then set the original fraction to 0.5/
                virtual void scaleToPixels(float w,float h,float screen_w,float screen_h,float original_fraction=0.5f){scaleTo((w/screen_w)*(1/original_fraction),(h/screen_h)*(1/original_fraction),Scale.z);};
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
                
                //has
                bool hasMoved(){return position_changed;};
                bool hasScaled(){return scale_changed;};
                bool hasRotated(){return rotation_changed;};
                //misc
                bool needsRefresh(){return needs_refresh;};
                void enableImplicitTranslation(bool b){applyTranslationWithoutRequest = b;};
                //Set
                void setModelMatrix(Matrix<4> mat){Model_Matrix = mat;};
                // Get
                Matrix<4> getModelMatrixCopy(){return Model_Matrix;};
                Matrix<4>* getModelMatrix(){return &Model_Matrix;};
                Matrix<4>* getMatrix(){return &Model_Matrix;};
                Vertex getDimensions(){return Dimensions*Scale;};
                Vertex getPosition(){return Position;};
                Vertex getScale(){return Scale;};
                Vertex getRotation(){return toEularFromQuat(Rotation);};
                Quaternion getQuaternion(){return Rotation;};
                // user functions
                std::function<void()> OnMove   = [](){};
                std::function<void()> OnRotate = [](){};
                std::function<void()> OnScale  = [](){};
                std::function<void()> OnMorph  = [](){};

            protected:

                Matrix<4> Model_Matrix = Matrix<4>(4);

                Quaternion Rotation = Quaternion(0,0,0,0);
                //locals.
                Vertex Local      = Vertex(1.0f,0.0f,0.0f);
                Vertex Up         = Vertex(0.0f,1.0f,0.0f);
                Vertex Forward    = Vertex(0.0f,0.0f,1.0f);
                //global cords.
                Vertex Offset     = Vertex(0.0f,0.0f,0.0f);
                Vertex Position   = Vertex(0.0f,0.0f,0.0f);
                Vertex Scale      = Vertex(1.0f,1.0f,1.0f);
                Vertex Dimensions = Vertex(1.0f,1.0f,1.0f);
                
            private:
                bool applyTranslationWithoutRequest = false;
                bool can_rotate = true;
                bool can_scale  = true;
                bool can_move   = true;

                bool position_changed = true;
                bool scale_changed = true;
                bool rotation_changed = true;

                bool needs_refresh=true;
        };
}}

#endif