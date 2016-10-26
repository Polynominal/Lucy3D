#ifndef MIKUS_LUCIA_MAIGUI_ITEM_H
#define MIKUS_LUCIA_MAIGUI_ITEM_H

#include <Lucia/Maigui/Maigui.h>
#include <Lucia/Maigui/Skin.h>
#include <Lucia/Maigui/Manager.h>
#include <Lucia/Maigui/Bases/Drawable.h>
#include <Lucia/Maigui/Bases/Moveable.h>
#include <Lucia/Maigui/Bases/Focus.h>

namespace Lucia {
namespace Maigui
{
    class Item : 
    
    public std::enable_shared_from_this<Maigui::Item>, 
    public Maigui::Base::Drawable,
    public Maigui::Base::Moveable,
    public Maigui::Base::Focus
    
    {
        friend class Container;
        friend class Widget;
        friend class Abstract;

        public:
            // these do not necesarily remove the shape programatically, these are used alongside manager.
            virtual void create(Manager *m);
            virtual void create(Item *m); // creation within another item. Allows for recrusion
            
            // SET's
            virtual void setSkin(shared_ptr<Skin> skin);
            virtual void setSkinReference(string name){Name = name;};
<<<<<<< HEAD
            virtual void setReference(string name){personalReference = name;};
=======
>>>>>>> dd48432476c1400a2c8d402d5f0074a5083831c7
            virtual void setDrag(bool b){canDrag = b;};
            
            // GET's
            shared_ptr<Item> getPtr(){return shared_from_this();};
            virtual string getReference(){return personalReference;};
            //Constructors and destructors
            virtual void addItem(Item* item,float x,float y,float z=0);
            virtual void addItem(Item* item);
            virtual void remove(Item* w);
            virtual void remove();
            // VOIDS
            virtual void draw();
            virtual void update(double dt);
            virtual void refresh();

            virtual void drag(int dx,int dy){if(canDrag){move(dx,dy,0);onDrag(dx,dy);OnDrag(dx,dy);}};
            //IS
            virtual bool hasDrag(){return canDrag;}
            // ON's
            virtual void onCreate(){};
            virtual void onRelease(Item *b){};
            virtual void onCollide(Item *b){};
            virtual void onDrag(int x,int y){};
            
            std::function<void()>OnDrawPre = [](){};
            std::function<void()>OnDrawAfter = [](){};
            std::function<void(int,int)>OnDrag = [](int dx,int dy){};
            std::function<void(double)>OnUpdate = [](double dt){};


        protected:
            string Name="Plate"; // this is used to navigate the skin.
            string personalReference = "null";
            Manager *Instance;
            Item    *Parent;
            
            vector<shared_ptr<Item>> Children;
            bool canDrag=false;
    };
}
}
#endif // MIKUS_MAIGUI_ITEM_H
