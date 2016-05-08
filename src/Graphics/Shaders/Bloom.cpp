#include "Bloom.h"
typedef Graphics::Shaders::Bloom Bloom;
Bloom::Bloom()
{
    Blur.reset(new Graphics::Shaders::Blur());
    Blur->setMode(Blur::Gaus5x5);
    //ctor
}
void Bloom::parse(unsigned int degree,float fsize,Graphics::Canvas* canvas,bool reset)
{
    // get: last shader, if blend is on, blend function
    generate();
    GLint lastShader;

    GLint src;
    GLint dst;
    GLboolean blend=false;
    glGetIntegerv(GL_CURRENT_PROGRAM,&lastShader);
    if (reset)
    {
        blend = glIsEnabled(GL_BLEND);
        glGetIntegerv(GL_BLEND_SRC_RGB,&src);
        glGetIntegerv(GL_BLEND_DST_RGB,&dst);
    }

    // the degree is a base of 2
    // so for 2 degrees the following is done:
    // 16x16 then 32x32 and that's it.
    // so for 4 degrees the following is done
    // 16x16 then 32x32 then 64x64 then 128x128
    // Remember that you are limited to the size of the texture
    // hence if it exceeds the size its imposible to increase the threshold so it defaults to max




        Canvas->attach(true,true);

        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE,GL_ONE);
        glUseProgram(Graphics::_Shaders::Gaus5x5->programID);

            for (unsigned int i=0;i < degree;i++)
            {
                unsigned int size = fsize*(i+1);
                if (lw < size or lh < size){break;};

                auto Buffer = canvas->getSize(size,size);
                Graphics::Canvas  Buff = Graphics::Canvas();
                Buff.generate(size,size);

                auto Quad = new Graphics::Primitive::Quad();
                Quad->setMode(false);
                Quad->generate(size,size);
                Buffer->setQuad(Quad);
                Buff.setQuad(Quad);

                glUniform2f(glGetUniformLocation(Graphics::_Shaders::Gaus5x5->programID, "size"),(1.0f/size),(1.0f/size));

                Buff.attach();
                Buffer->renderQuad();
                auto nBuffer = Buff.getSize(lw,lh);

                Canvas->attach();
                nBuffer->renderQuad(true);

                delete Quad;
            }
        Canvas->detach();

        glUseProgram(lastShader);
        if (reset)
        {
            if (!blend)
            {
                glDisable(GL_BLEND);
            };
            glBlendFunc(src,dst);
        }
}
void Bloom::draw()
{
    Canvas->renderQuad(true);
}
Bloom::~Bloom()
{
    //dtor
}
