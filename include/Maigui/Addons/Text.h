#ifndef MIKUS_MAIGUI_TEXT_H
#define MIKUS_MAIGUI_TEXT_H
#include <Maigui/Maigui.h>
#include <Maigui/Skin.h>
#include <Maigui/Bases/Color.h>
#include <Maigui/Bases/Moveable.h>

namespace Maigui
{
    namespace Addon
    {
        class Text : public Maigui::Base::Moveable, Maigui::Base::Color
        {
            public:
                Text();
                virtual void remove();
                virtual void generate(Vertex position,Vertex dimensions,shared_ptr<Skin> sk);
                virtual void setText(string T);
                virtual void setSkin(std::shared_ptr<Maigui::Skin> s);
                virtual void setSize(int s);

                virtual int getSize(){return size;};
                virtual int getLines();
                virtual int getMaxLineLenght();
                virtual string getText(){return text;};
                virtual float getWidth();
                virtual float getHeight();

                virtual void draw();
                virtual ~Text();
            protected:
            private:
                string text="Text";
                shared_ptr<Skin> skin;
                float width=0.0f;
                float height=0.0f;
                int size = 24;
                int id=-1;
        };
    }
}
#endif // MIKUS_MAIGUI_TEXT_H
