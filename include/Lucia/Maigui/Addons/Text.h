#ifndef MIKUS_LUCIA_MAIGUI_TEXT_H
#define MIKUS_LUCIA_MAIGUI_TEXT_H
#include <Lucia/Maigui/Maigui.h>
#include <Lucia/Maigui/Skin.h>
#include <Lucia/Graphics/Bases/Color.h>
#include <Lucia/Maigui/Bases/Moveable.h>
namespace Lucia {
namespace Maigui
{
    namespace Addon
    {
        class Text : public Maigui::Base::Moveable, Graphics::Base::Color
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
}
#endif // MIKUS_MAIGUI_TEXT_H
