#ifndef MIKUS_MAIGUI_WIDGET_H
#define MIKUS_MAIGUI_WIDGET_H
#include <Maigui/Maigui.h>
#include <Maigui/Item.h>

class Maigui::Widget : public Maigui::Item
{
    public:
        Widget(Vertex,Vertex,shared_ptr<Skin> skin);
        virtual ~Widget();

    protected:
    private:
};

#endif // MIKUS_MAIGUI_WIDGET_H
