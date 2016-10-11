#include "Lucia\Maigui\Addons\Titlebar.h"
#include <Lucia\Maigui\Item.h>
using namespace Lucia;
typedef Maigui::Addon::Titlebar Titlebar;
typedef Collider::Vertex Vertex;
Titlebar::Titlebar()
{
    Name = "Titlebar";
    //ctor
}
void Titlebar::close()
{

}
void Titlebar::generate(Vertex position,Vertex dimensions,shared_ptr<Skin> sk)
{
    Position = position;
    Dimensions = dimensions;
    setSkin(sk);
}
void Titlebar::setSkin(shared_ptr<Skin>s)
{
    Maigui::Item::setSkin(s);
    text.setSkin(s);
}
void Titlebar::onMorph()
{
    auto s = text.getText().size();
    Maigui::Item::onMorph();
    Vertex dimensions = Vertex(fontSize*s,size,Dimensions.z);
    text.rotateTo(Rotation);
    text.scaleTo(dimensions);
    Vertex pos = Position;
    pos.y += (s/2.0f);
    text.moveTo(pos);
}
void Titlebar::draw()
{
    Maigui::Item::draw();
    text.draw();
}
void Titlebar::setTitle(string t)
{
    text.moveTo(Position);
    text.scaleTo(Dimensions);
    text.setText( t );
    onMorph();
}
void Titlebar::refresh()
{
    Maigui::Item::refresh();
    text.refresh();
}
Titlebar::~Titlebar()
{
    //dtor
}
