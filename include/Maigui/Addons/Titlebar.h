#ifndef MIKUS_MAIGUI_ADDONS_TITLEBAR_H
#define MIKUS_MAIGUI_ADDONS_TITLEBAR_H
#include <Maigui\Item.h>
#include <Maigui\Bases\Moveable.h>
#include <Maigui\Addons\Text.h>


namespace Maigui
{
    namespace Addon
    {
        class Titlebar: public Maigui::Item
        {
            public:
                Titlebar();

                void generate(Vertex position,Vertex dimensions,shared_ptr<Skin> sk);

                virtual void draw();
                virtual void close();

                virtual void onMorph();


                virtual void refresh();

                virtual Text* getTextItem(){return &text;};
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

#endif // MIKUS_MAIGUI_ADDONS_TITLEBAR_H
