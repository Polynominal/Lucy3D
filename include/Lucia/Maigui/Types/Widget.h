#ifndef MIKUS_LUCIA_MAIGUI_WIDGET_H
#define MIKUS_LUCIA_MAIGUI_WIDGET_H
#include <Lucia/Maigui/Maigui.h>
#include <Lucia/Maigui/Item.h>
namespace Lucia {
class Maigui::Widget : public Maigui::Item
{
    public:
        Widget(Vertex,Vertex,shared_ptr<Skin> skin);
        Widget(){};
        virtual ~Widget();

    protected:
    private:
};
}
#endif // MIKUS_MAIGUI_WIDGET_H
