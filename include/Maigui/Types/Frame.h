#ifndef MIKUS_MAIGUI_FRAME_H
#define MIKUS_MAIGUI_FRAME_H
#include <Maigui/Maigui.h>
#include <Maigui/Item.h>
#include <Maigui/Types/Container.h>
#include <Maigui/Addons/Titlebar.h>

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
                virtual void onMorph();
                virtual void onCreate();
                virtual ~Frame(){remove();};

            protected:
                std::shared_ptr<Addon::Titlebar> titleBar;
        };
    }
}

#endif // MIKUS_MAIGUI_FRAME_H
