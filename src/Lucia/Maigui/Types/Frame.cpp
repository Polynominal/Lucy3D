#include "Lucia/Maigui/Types/Frame.h"
namespace Lucia {
namespace Maigui
{
    namespace Containers
    {
        Frame::Frame()
        {
            Name = "Frame";
        }
        void Frame::rePositionTitle()
        {
            Vertex dimensions = Vertex(Dimensions.x,Maigui::Sizes::FrameBorder,Dimensions.z);
            Vertex position =
            Vertex(Position.x,
                  (Position.y + Dimensions.y/2.0f) + dimensions.y/2.0f,
                    Position.z + 0.00000001f);
            titleBar->moveTo(position);
            titleBar->scaleTo(dimensions);
            titleBar->rotateTo(Rotation);
        }
        void Frame::onCreate()
        {
            titleBar = std::make_shared<Addon::Titlebar>();
            auto p = titleBar.get();
            this->addItem(dynamic_cast<Maigui::Item*>(p),0,0,0);
            refresh();
            setTitle("Test");
            titleBar->refresh();
        }
        void Frame::onMorph()
        {
            Container::onMorph();
            rePositionTitle();
        }
        void Frame::setTitle(string title)
        {
            rePositionTitle();
            titleBar->setTitle(title);
        }
    }
}
}
