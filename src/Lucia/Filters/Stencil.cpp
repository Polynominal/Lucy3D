#include "Lucia/Filters/Stencil.h"
using namespace Lucia;
using namespace Graphics;
using namespace Filters;
Stencil::Stencil()
{
    //ctor
}
GLenum enumToGL(Stencil::Result v)
{
    GLenum out = Stencil::Result::Keep;
    switch(v)
    {
        case Stencil::Result::Zero:
            out = GL_ZERO;
            break;
        case Stencil::Result::Replace:
            out = GL_REPLACE;
            break;
        case Stencil::Result::Increase:
            out = GL_INCR;
            break;
        case Stencil::Result::IncreaseWrap:
            out = GL_INCR_WRAP;
            break;
        case Stencil::Result::Decrease:
            out = GL_DECR;
            break;
        case Stencil::Result::DeacreaseWrap:
            out = GL_DECR_WRAP;
            break;
        case Stencil::Result::Invert:
            out = GL_INVERT;
            break;
    }
    return out;
}
GLenum enumToGL(Stencil::Value v)
{
    GLenum out =GL_NEVER;
    switch(v)
    {
        case Stencil::Value::Less:
            out = GL_LESS;
            break;
        case Stencil::Value::LessEqual:
            out = GL_LEQUAL;
            break;
        case Stencil::Value::Greater:
            out = GL_GREATER;
            break;
        case Stencil::Value::GreaterEqual:
            out = GL_GEQUAL;
            break;
        case Stencil::Value::Equal:
            out = GL_EQUAL;
            break;
        case Stencil::Value::NotEqual:
            out = GL_NOTEQUAL;
        case Stencil::Value::Always:
            out = GL_ALWAYS;
            break;
    }
    return out;
}
void Stencil::start()
{
    glEnable(GL_STENCIL_TEST);
    switch(mode)
    {
        case Inverted:
            glStencilMask(0xFF);
            break;
        case Regular:
            glStencilMask(0x00);
            break;
    }
    glStencilFunc(enumToGL(startValue),1,0xFF);
    glStencilOp(enumToGL(fail),enumToGL(depthFail),enumToGL(pass));
    glDisable(GL_DEPTH_TEST);
}
void Stencil::end()
{
    glStencilFunc(enumToGL(startValue),1,0xFF);
    switch(mode)
    {
        case Inverted:
            glStencilMask(0x00);
            break;
        case Regular:
            glStencilMask(0xFF);
            break;
    }
    glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT);
}
Stencil::~Stencil()
{
    //dtor
}
