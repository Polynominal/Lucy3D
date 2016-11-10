#ifndef MIKUS_LUCIA_MAIGUI_TYPES_BUTTON_H
#define MIKUS_LUCIA_MAIGUI_TYPES_BUTTON_H
#include <Lucia/Maigui/Addons/Text.h>
#include <Lucia/Maigui/Item.h>
namespace Lucia {
namespace Maigui
{
    class Button: public Maigui::Item
    {
        public:
            Button(){};
            virtual void draw();
            virtual void generate(Vertex position,Vertex dimensions,shared_ptr<Skin> sk);
            virtual void setText(string text){CText.setText(text);};
            virtual void onCreate();
            virtual void onMorph();
            virtual void resetText();
            virtual void refresh();

            virtual Maigui::Addon::Text* getTextObject(){return &CText;};

        protected:
            Maigui::Addon::Text CText = Maigui::Addon::Text();
    };
}
}

#endif // MIKUS_MAIGUI_TYPES_BUTTON_H
