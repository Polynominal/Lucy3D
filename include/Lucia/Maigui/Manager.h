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
            
            
            void remove();
            void remove(shared_ptr<Maigui::Item>i);
        
            void draw();
            void drawSkeleton();
            void update(double dt);
            
            //GET
            Vertex getViewVertex();
            shared_ptr<Skin> getSkin(){return skin;};
            
            Maigui::Item* findItem(string name);
            //SET
            void setRay(Vertex Eye,Vertex Forward);
            void setGlobalScale(float x=1.0f,float y = 1.0f);
            
<<<<<<< HEAD
            void mousemoved(bool* keys_down,int x,int y,int dx,int dy);
=======
            void mousemoved(int* keys_down,int x,int y,int dx,int dy);
>>>>>>> dd48432476c1400a2c8d402d5f0074a5083831c7
            void mousepressed(string key);
            void mousereleased(string key);
            void keypressed(string key);
            void textinput(char c);
            
            void eraseFromCollisions(Item* itemData);
            virtual ~Manager();
            // ADD's
            void addItem(Maigui::Item*);
            void addItem(Maigui::Item*,Vertex p,Vertex d);
            shared_ptr<Container> addContainer(float x,float y,float z,float w,float h, float d=1,bool add=true);
            shared_ptr<Containers::Frame> addFrame(float x,float y,float z,float w,float h,float d=1,bool add=true);
            shared_ptr<Button> addButton(float x,float y,float z,float w,float h,float d=1,bool add=true);
            shared_ptr<Widget> addWidget(float x,float y,float z,float w,float h, float d=1,bool add=true);
            // PUBLIC MEMBERS//
            std::shared_ptr<Collider::Manager> World;
            std::shared_ptr<Collider::Manager> getWorld(){return World;};
            shared_ptr<Collider::Ray> Mouse;

            Item* blankCompare(Item* shape1,Item* shape2);
            bool compare(std::shared_ptr<Item> shape);

        private:
            Matrix<4>* Projection = nullptr;
            Matrix<4>* View = nullptr;
            
            Vertex globalScale = Vertex(1,1,1);
            shared_ptr<Item> activeItem;
            shared_ptr<Skin> skin;
            std::vector<shared_ptr<Item>> ActiveCollisions;
            std::vector<shared_ptr<Item>> Children;
    };
}
}
#endif // MIKUS_MAIGUI_MANAGER_H
