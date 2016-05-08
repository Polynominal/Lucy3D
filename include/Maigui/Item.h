#ifndef MIKUS_MAIGUI_ITEM_H
#define MIKUS_MAIGUI_ITEM_H

#include <Maigui/Maigui.h>
#include <Maigui/Skin.h>
#include <Maigui/Manager.h>
#include <Maigui/Bases/Drawable.h>
#include <Maigui/Bases/Moveable.h>

namespace Maigui
{
    class Item : public std::enable_shared_from_this<Maigui::Item>, public Maigui::Base::Drawable,public Maigui::Base::Moveable
    {
        friend class Container;
        friend class Widget;
        friend class Abstract;

        public:
            virtual void mousepressed(string key); // x and y relative to the top corner of the item
            virtual void mousereleased(string key);
            virtual void textinput(char);
            virtual void keypressed(string);

            // these do not necesarily remove the shape programatically, these are used alongside manager.
            virtual void create(Manager *m);
            virtual void create(Item *m); // creation within another item. Allows for recrusion.


            // for example you can remove the shape from one manager and add it to the other.


            // SET's
            virtual void setSkin(shared_ptr<Skin> skin);
            virtual void setSkinReference(string name){Name = name;};
            virtual void setInactive(bool condition);
            virtual void setFocus(bool condition);
            // GET's
            virtual bool getFocus(){return hasFocus;};
            shared_ptr<Item> getPtr(){return shared_from_this();};
            shared_ptr<Shape> getShape();
            //Constructors and destructors
            virtual void addItem(Item* item,float x,float y,float z=0);
            virtual void remove(Item* w);
            virtual void remove();
            // VOIDS
            virtual void draw();
            virtual void update(double dt);
            virtual void refresh();


            // ON's
            virtual void onCreate(){};
            virtual void onFocus(bool f){};
            virtual void onRelease(Item *b){};
            virtual void onCollide(Item *b){};

            std::function<void()>OnDrawPre = [](){};
            std::function<void()>OnDrawAfter = [](){};
            std::function<void(double)>OnUpdate = [](double dt){};
            std::function<void(bool)>OnFocus = [](bool hasFocus){};

            std::function<void(bool)>OnClick = [](bool b){};
            std::function<void(bool)>OnRightClick = [](bool b){};
            std::function<void(bool)>OnMiddleClick = [](bool b){};


        protected:
            string Name="Plate"; // this is used to navigate the skin.

            Manager *Instance;
            Item    *Parent;

            shared_ptr<Shape> Core;

            vector<shared_ptr<Item>> Children;

            bool Inactive= true; // inactive cannot not process input but is drawn
            bool Visible = false; // invisible but can still process input
            bool needsRefresh=true;

            bool hasFocus=false;
            bool lastFocus=false;
    };
}

#endif // MIKUS_MAIGUI_ITEM_H
