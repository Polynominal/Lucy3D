#include "Lucia\Graphics\Buffers\Canvas.h"
using namespace Lucia;
using namespace Graphics;
using namespace Buffer;
bool Canvas::generate(float w,float h)
{
    if (!Graphics::Shared::Quad->isDone())
    {
        Graphics::Shared::Quad->generate();
    }
    if (w == 0 or h == 0)
    {
        GLint d[4];
        glGetIntegerv(GL_VIEWPORT,(GLint*)&d);
        w = d[2];
        h = d[3];
    }
    remove();
    glGenTextures(1,&textureID);
    glGenFramebuffers(1,&FBO);
    glGenRenderbuffers(1,&RBO);

    width = w;
    height = h;

    glBindFramebuffer(GL_FRAMEBUFFER,FBO);
    glBindTexture(GL_TEXTURE_2D,textureID);

    #ifndef LUCIA_USE_GLES2
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    #else
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    #endif
    //filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //wrapping
    glTexParameteri(GL_TEXTURE_2D,  GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,  GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D,0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureID, 0);

    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    #ifdef LUCIA_USE_GLES2
        glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT,width,height);
    #else
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    #endif // LUCIA_USE_GLES2

    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    #ifdef LUCIA_USE_GLES2
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RBO);
    #else
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);
    #endif
    glBindFramebuffer(GL_FRAMEBUFFER,0);


    passive = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
    if (passive != true)
    {
        passive = true;
        remove();
        return false;
    }
    return true;
}
void Canvas::clear()
{
    glBindFramebuffer(GL_FRAMEBUFFER,FBO);
    glClearColor(Color[0],Color[1],Color[2],Color[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER,0);
}
void Canvas::attach(bool clear,bool clearColor)
{
    // we are going to need to reset the shader to its default state because
    // at this point we are probably reseting the draw sequence.
    glBindFramebuffer(GL_FRAMEBUFFER,FBO);
    if (clear){
        if (clearColor)
        {
            glClearColor(Color[0],Color[1],Color[2],Color[3]);
        };
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }else
    {
        glClear(GL_DEPTH_BUFFER_BIT);
    };
}
void Canvas::remove()
{
    glDeleteFramebuffers(1, &FBO);
    glDeleteRenderbuffers(1,&RBO);
    Graphics::Base::Texture::remove();
    passive = true;
}
void Canvas::detach(int nbuffer)
{
    glBindFramebuffer(GL_FRAMEBUFFER,0);
}
void Canvas::resize(float width,float height,bool keepAspect)
{
    Canvas newImage = Canvas();
    newImage.generate(width,height);
    #ifndef LUCIA_USE_GLES2
        glBlitNamedFramebuffer(FBO,newImage.FBO,0.0f,0.0f,width,height,
          0.0f,0.0f,width,height,GL_COLOR_BUFFER_BIT, GL_LINEAR);
    #else
    GLint d[4];
    glGetIntegerv(GL_VIEWPORT,(GLint*)&d);

    float tx = (d[2]/width);
    float ty = (d[3]/height);

    Graphics::Shared::Quad->setImageRange(0.0f,0.0f,tx,ty);
    newImage.attach(true,true);
    renderQuad(true);
    newImage.detach();

    tx = (d[2]/width);
    ty = (d[3]/height);
    Graphics::Shared::Quad->setImageRange(0.0f,0.0f,tx,ty);

    attach(true,true);
    newImage.renderQuad(true);
    detach();
    newImage.remove();
    Graphics::Shared::Quad->setImageRange();
    #endif // LUCIA_USE_GLES2
}
std::unique_ptr<Canvas> Canvas::getBlitSize(float w,float h,bool keepAspect)
{
    auto output = std::unique_ptr<Graphics::Buffer::Canvas>(new Graphics::Buffer::Canvas());
    output->generate(w,h);
    #ifndef LUCIA_USE_GLES2
    glBlitNamedFramebuffer(FBO,output->FBO,0.0f,0.0f,width,height,
                           0.0f,0.0f,w,h,GL_COLOR_BUFFER_BIT, GL_LINEAR);
    #else
    GLint d[4];
    glGetIntegerv(GL_VIEWPORT,(GLint*)&d);

    float tx = (d[2]/w);
    float ty = (d[3]/h);

    Graphics::Shared::Quad->setImageRange(0.0f,0.0f,tx,ty);
    output->attach(true,true);
    renderQuad(true);
    output->detach();
    Graphics::Shared::Quad->setImageRange();

    generate(w,h);
    attach(true,true);
    output->renderQuad(true);
    detach();
    output->remove();
    #endif
    return output;
}
