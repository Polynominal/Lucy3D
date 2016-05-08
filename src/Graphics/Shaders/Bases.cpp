#include "Bases.h"
typedef Graphics::Shaders::Bases::Canvas CanvasBase;

bool CanvasBase::generate(float w,float h)
{
    if (Canvas.get() == nullptr)
    {
        Canvas = std::make_shared<Graphics::Canvas>();
    }
    Canvas->generate(w,h);
    lw = Canvas->getWidth();
    lh = Canvas->getHeight();
    passive = Canvas->isDone();
    return passive;
}
void CanvasBase::clear()
{
    Canvas->clear();
}
void CanvasBase::remove()
{
    if (Canvas.get() != nullptr){Canvas->remove();};
    passive = true;
}
GLuint CanvasBase::getTextureID()
{
    if (!passive && Canvas->isDone())
    {
        return Canvas->getTextureID();
    }
    return 0;
}
void CanvasBase::attach()
{
    Canvas->attach(true);
}
void CanvasBase::detach()
{
    Canvas->detach();
}
