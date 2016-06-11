#include "Graphics\Buffers\Canvas.h"

typedef Graphics::Canvas Canvas;
Canvas::Canvas()
{
    //ctor
}
bool Canvas::generate(float width,float height)
{
    if (!Graphics::Shared::Quad->isDone())
    {
        Graphics::Shared::Quad->generate();
    }
    if (width == 0 or height == 0)
    {
        GLint d[4];
        glGetIntegerv(GL_VIEWPORT,(GLint*)&d);
        width = d[2];
        height = d[3];
    }
    remove();
    glGenTextures(1,&texture);
    glGenFramebuffers(1,&FBO);
    glGenRenderbuffers(1,&RBO);

    lw = width;
    lh = height;

    glBindFramebuffer(GL_FRAMEBUFFER,FBO);
    glBindTexture(GL_TEXTURE_2D,texture);


    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, lw, lh, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    //filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //wrapping
    glTexParameteri(GL_TEXTURE_2D,  GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,  GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D,0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    #ifdef USE_GLES2
        glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT,lw,lh);
    #else
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, lw, lh);
    #endif // USE_GLES2

    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    #ifdef USE_GLES2
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
    lastShader = -1.0f;
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
    glDeleteTextures(1,&texture);
    passive = true;
}
void Canvas::renderQuad(bool ownShader,bool resetShader)
{
    GLboolean hasDepth = glIsEnabled(GL_DEPTH_TEST);
    if (ownShader){
        if (resetShader or lastShader == -1.0f){glGetIntegerv(GL_CURRENT_PROGRAM,&lastShader);};

        glUseProgram(Graphics::_Shaders::Basic2D->programID);
        // send necessary parameters such as that we want to use a texture.
        glUniform1i(glGetUniformLocation(Graphics::_Shaders::Basic2D->programID, "useColor"),false);
    }
    GLint lastTexture;
    glGetIntegerv(GL_TEXTURE_BINDING_2D,&lastTexture);

    glDisable(GL_DEPTH_TEST);
    glActiveTexture(GL_TEXTURE0);

    glBindTexture(GL_TEXTURE_2D,texture);
    if (Quad == nullptr)
    {
        Graphics::Shared::Quad->draw();
    }else
    {
        Quad->draw();
    }
    glBindTexture(GL_TEXTURE_2D,lastTexture);

    if (ownShader and resetShader){glUseProgram(lastShader);}
    if (hasDepth){glEnable(GL_DEPTH_TEST);};
}
void Canvas::detach(int nbuffer)
{
    glBindFramebuffer(GL_FRAMEBUFFER,0);
}
void Canvas::resize(float width,float height,bool keepAspect)
{
    Canvas newImage = Canvas();
    newImage.generate(width,height);
    #ifndef USE_GLES2
        glBlitNamedFramebuffer(FBO,newImage.FBO,0.0f,0.0f,lw,lh,
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

    tx = (d[2]/lw);
    ty = (d[3]/lh);
    Graphics::Shared::Quad->setImageRange(0.0f,0.0f,tx,ty);

    attach(true,true);
    newImage.renderQuad(true);
    detach();
    newImage.remove();
    Graphics::Shared::Quad->setImageRange();
    #endif // USE_GLES2
}
std::unique_ptr<Canvas> Canvas::getSize(float w,float h,bool keepAspect)
{
    auto output = std::unique_ptr<Graphics::Canvas>(new Graphics::Canvas());
    output->generate(w,h);
    #ifndef USE_GLES2
    glBlitNamedFramebuffer(FBO,output->FBO,0.0f,0.0f,lw,lh,
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
float Canvas::getWidth()
{
    return lw;
}
float Canvas::getHeight()
{
    return lh;
}
void Canvas::bindTexture()
{
    glBindTexture(GL_TEXTURE_2D,texture);
}
Canvas::~Canvas()
{
    remove();
    //dtor
}
