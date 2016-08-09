#include "Lucia\Maigui\Types\Button.h"
using namespace Lucia;
typedef Maigui::Button Button;

void Button::generate(Vertex position,Vertex dimensions,shared_ptr<Skin> sk)
{
    Name = "Button";
    Position = position;
    Dimensions = dimensions;
    skin = sk;
    CText.generate(position,dimensions,sk);
}
void Button::onCreate()
{
    refresh();
}
float scaleToAspect(float w,float h,float nw)
{
    float ratio = w/h;
    return nw/ratio;
}
void Button::resetText()
{
    CText.moveTo(Position);
    CText.scaleTo(Dimensions);
    CText.rotateTo(Rotation);
    CText.setSize(CText.getSize());
}
void Button::onMorph()
{
    resetText();
}
void Button::refresh()
{
    Maigui::Item::refresh();
    CText.setGlobalScale(globalScale);
    CText.refresh();
    resetText();
}
void Button::draw()
{
    Maigui::Item::draw();
    CText.draw();
}
