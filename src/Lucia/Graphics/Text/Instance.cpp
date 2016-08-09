#include "Lucia/Graphics/Text/Instance.h"
#include <Lucia/Graphics/Text/Task.h>
#include <Lucia/Maths/Moveable.h>
using namespace Lucia;
using namespace Graphics;

typedef Graphics::Text::Font        Font;
typedef Graphics::Text::Render      Render;
typedef Graphics::Text::Instance    Instance;
//INSTANCE
    Instance::Instance(Graphics::Text* p)
    {
        Fonts = p->Fonts;
        activeFont = p->activeFont;
        activeType = p->activeType;
        projection = p->projection;
        size = p->size;
        parent = p;
        ft = p->ft;
    }
    Instance::~Instance()
    {

    }
// RENDER
Render::Render(Instance* i)
{
    // time to parse the canvas:
    // we need to selectively produce canvases here so add a new function that rebuilds the canvases with smaller sizes.
    // based on their individual features.
    Buffer.reset(new Utils::OpenGL::Buffer(Graphics::textShader->getVars()));
    Buffer->allocateData(6);
    //
    parent = i;
    activeFont = parent->activeFont;
    Matrix = parent->Matrix;

    Position = parent->Position;
    Rotation = parent->Rotation;
    Scale = parent->Scale;
}
Render::~Render()
{
    Buffer->destroy();
}
void Render::generate()
{
    Buffer->allocateData(6);
}
Render& Graphics::Text::Render::operator << (string c)
{
        print(c);
        return *this;
};
void Render::print(string str)
{
    if (activeSub == nullptr)
    {
        activeSub = parent->get(activeFont,parent->activeType);
    }
    if (applyTranslations())
    {
        glUniformMatrix4fv(glGetUniformLocation(Graphics::textShader->programID, "model"),
        1,GL_FALSE,Model_Matrix.unpack());
    }
    auto of = parent->renderString(str,offset_x,0.0f,gapSize,fixedGap,activeSub);
    offset_x = of.first;
    width = std::max(width,offset_x);
    height = std::max(height,of.second);
}
Render& Render::operator << (const float a)
{
    return *this << std::to_string(a).c_str();
};
Render& Render::operator << (const bool a)
{
    if (a)
    {
        return *this << "True";

    }else
    {
        return *this << "False";
    }
}
Render& Render::operator << (Text::Type a)
{
    if (parent->has(a))
    {
        std::shared_ptr<Font> target=activeFont;
        if (target.get() == nullptr)
        {
            target = parent->activeFont;
        }
        activeSub = parent->get(target,a);
    }
    return *this;
};
Render& Render::operator << (std::shared_ptr<Font> a)
{
    activeFont = a;
    return *this;
}
Render& Render::operator << (Font::subfont* a)
{
    activeSub = a;
    return *this;
}
Graphics::Text::Task* Render::operator << (Text::Present a)
{
    auto t = std::make_shared<Graphics::Text::Task>(this,a);
    Tasks.push_back(t);
    return t.get();
}
void Render::pre()
{
    auto programID = Graphics::textShader->programID;
    Graphics::textShader->use();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glActiveTexture(GL_TEXTURE0);


    // lets start with white!
    glUniform4f(glGetUniformLocation(programID, "textColor"), 1.0f,1.0f,1.0f,1.0f);
    glUniformMatrix4fv(glGetUniformLocation(programID, "projection"),
    1,GL_FALSE,parent->projection->unpack());

    applyTranslations();
    glUniformMatrix4fv(glGetUniformLocation(programID, "model"),
    1,GL_FALSE,Model_Matrix.unpack());
}
void Render::post()
{
    glDisable(GL_BLEND);
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);
    used_size = 0;
    offset_x = 0;
}
void Render::setColor(float r,float g,float b,float a)
{
    glUniform4f(glGetUniformLocation(Graphics::textShader->programID, "textColor"), r,g,b,a);
};
