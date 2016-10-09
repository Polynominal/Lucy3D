#ifndef MIKUS_LUCIA_MAIGUI_ADDONS_TITLEBAR_H
#define MIKUS_LUCIA_MAIGUI_ADDONS_TITLEBAR_H
#include <Lucia/Maigui\Item.h>
#include <Lucia/Maigui\Bases\Moveable.h>
#include <Lucia/Maigui\Addons\Text.h>
#include <Lucia/Graphics/Bases/Color.h>

namespace Lucia {
namespace Maigui
{
    namespace Addon
    {
        class Titlebar: public Maigui::Item
        {
            public:
                Titlebar();

                void generate(Vertex position,Vertex dimensions,shared_ptr<Skin> sk);
                void drag(int x,int y){if (Parent){Parent->drag(x,y);};};
                
                virtual void draw();
                virtual void close();

                virtual void onMorph();


                virtual void refresh();
                
                virtual Text* getText(){return &text;};
                virtual string getTitle(){return text.getText();};

                virtual void setTitle(string Title);
                virtual void setSkin(shared_ptr<Maigui::Skin>s);
                virtual void setFontSize(float s){fontSize = s;};
                virtual void setSize(float s){size = s;};


                virtual ~Titlebar();

            protected:
                float size = 20;
                float fontSize = 10;
                Text text = Text();
        };
    }
}
}
#endif // MIKUS_MAIGUI_ADDONS_TITLEBAR_H
