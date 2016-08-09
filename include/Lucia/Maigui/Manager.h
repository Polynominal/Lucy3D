#ifndef MIKUS_LUCIA_MAIGUI_MANAGER_H
#define MIKUS_LUCIA_MAIGUI_MANAGER_H

#include <Lucia/Maigui/Maigui.h>
#include <Lucia/Maigui/Item.h>
namespace Lucia {
namespace Maigui
{
    class Manager
    {
        public:
            Manager();
            void formDefaultSkin();
            void setRay(Vertex Eye,Vertex Forward);

            void remove();
            void remove(shared_ptr<Maigui::Item>i);

            void draw();
            void drawSkeleton();
            void update(double dt);

            Vertex getViewVertex();
            void   setGlobalScale(float x=1.0f,float y = 1.0f);

            void mousepressed(string key);
            void mousereleased(string key);
            void keypressed(string key);
            void textinput(char c);
            shared_ptr<Skin> getSkin(){return skin;};
            void eraseFromCollisions(Item* itemData);
            virtual ~Manager();
            // ADD's
            void addItem(Maigui::Item*);
            void addItem(Maigui::Item*,Vertex p,Vertex d);
            shared_ptr<Container> addContainer(float x,float y,float z,float w,float h, float d=1);
            shared_ptr<Containers::Frame> addFrame(float x,float y,float z,float w,float h,float d=1);
            shared_ptr<Button> addButton(float x,float y,float z,float w,float h,float d=1);
            shared_ptr<Widget> addWidget(float x,float y,float z,float w,float h, float d=1);
            // PUBLIC MEMBERS//
            Collider::Manager* World;
            Collider::Manager* getWorld(){return World;};
            shared_ptr<Collider::Ray> Mouse;

            Item* blankCompare(Item* shape1,Item* shape2);
            bool compare(std::shared_ptr<Item> shape);

        private:
            Vertex globalScale = Vertex(1,1,1);
            shared_ptr<Item> activeItem;
            shared_ptr<Skin> skin;
            std::vector<shared_ptr<Item>> ActiveCollisions;
            std::vector<shared_ptr<Item>> Children;
    };
}
}
#endif // MIKUS_MAIGUI_MANAGER_H
