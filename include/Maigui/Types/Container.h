#ifndef MIKUS_MAIGUI_CONTAINER_H
#define MIKUS_MAIGUI_CONTAINER_H
#include <Maigui/Maigui.h>
#include <Maigui/Item.h>
namespace Maigui {
    class Container : public Maigui::Item
    {
        public:
            Container(){};
            virtual void generate(Vertex position,Vertex dimensions,shared_ptr<Skin> sk);
            virtual void minimize();
            virtual void maximize();
            virtual void close();
            virtual ~Container();
        protected:
            Vertex maximumSize = Vertex(100,100,100);
            Vertex minimumSize = Vertex(10,10,10);
    };
} // namespace Maigui

#endif // MAIGUI_CONTAINER_H
