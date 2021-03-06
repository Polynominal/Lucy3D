#include "Lucia/Maigui/Types/Widget.h"
namespace Lucia {
namespace Maigui {

    Widget::Widget(Vertex position,Vertex dimensions,shared_ptr<Skin> sk)
    {
        Position = position;
        Dimensions = dimensions;
        skin = sk;
        Name = "Widget";
        //ctor
    };
    Widget::~Widget()
    {
        remove();
        //dtor
    }
} // namespace Maigui
}
