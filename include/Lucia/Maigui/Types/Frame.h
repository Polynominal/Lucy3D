#ifndef MIKUS_LUCIA_MAIGUI_FRAME_H
#define MIKUS_LUCIA_MAIGUI_FRAME_H
#include <Lucia/Maigui/Maigui.h>
#include <Lucia/Maigui/Item.h>
#include <Lucia/Maigui/Types/Container.h>
#include <Lucia/Maigui/Addons/Titlebar.h>
#include <Lucia/Graphics/Bases/Color.h>
namespace Lucia {
namespace Maigui
{
    namespace Containers
    {
        class Frame : public Maigui::Container
        {
            public:

                Frame();
                //misc
                virtual void rePositionTitle();
                //gets
                virtual Addon::Titlebar* getTitlebar(){return titleBar.get();};

                virtual string getTitle(){return titleBar->getTitle();};
                virtual void setTitle(string text);
                virtual void setTitleTextColor(float r,float g,float b,float a=255){setTitleTextColor(Graphics::Base::Color(r,g,b,a));};
                virtual void setTitleTextColor(Graphics::Base::Color c){titleBar->getText()->setColor(c);};
                virtual void onMorph();
                virtual void onCreate();

            protected:
                std::shared_ptr<Addon::Titlebar> titleBar;
        };
    }
}
}
#endif // MIKUS_MAIGUI_FRAME_H
