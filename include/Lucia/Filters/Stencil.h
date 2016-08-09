#ifndef MIKUS_GRAPHICS_STENCIL_H
#define MIKUS_GRAPHICS_STENCIL_H
#include <Lucia/Settings.h>
namespace Lucia
{
    namespace Graphics
    {
        namespace Filters
        {
            class Stencil
            {
                public:
                    Stencil();
                    enum Mode
                    {
                        Inverted,
                        Regular,
                    };
                    enum Face
                    {
                        Back,
                        Front,
                    };
                    enum Result
                    {
                        Keep,
                        Zero,
                        Replace,
                        Increase,
                        IncreaseWrap,
                        Decrease,
                        DeacreaseWrap,
                        Invert
                    };
                    enum Value
                    {
                        Never,
                        Less,
                        LessEqual,
                        Greater,
                        GreaterEqual,
                        Equal,
                        NotEqual,
                        Always,
                    };
                    virtual ~Stencil();

                    void start();
                    void end();

                    void setTest(Result f,Result depth_f,Result p)
                    {fail =f;depthFail = depth_f,pass = p;};
                    void setFace(Face f){face =f;};
                    void setValueStart(Value v){startValue = v;};
                    void setValueEnd(Value v){endValue = v;};


                    void render();

                protected:
                private:
                    Mode mode = Mode::Inverted;
                    Face face = Face::Front;

                    Result fail = Result::Keep;
                    Result depthFail = Result::Keep;
                    Result pass = Result::Keep;

                    Value startValue = Value::Equal;
                    Value endValue = Value::NotEqual;
            };
        }
    }
}

#endif // MIKUS_GRAPHICS_STENCIL_H
