#include "Lucia/Maigui/Manager.h"
#include <Lucia/Maigui/Types/Container.h>
#include <Lucia/Maigui/Types/Widget.h>
#include <Lucia/Maigui/Types/Frame.h>
#include <Lucia/Maigui/Types/Button.h>
using namespace Lucia;
namespace Lucia {
namespace Maigui
{
    Manager::Manager()
    {
        skin = Maigui::Default::Skin;
        World = new Collider::Manager;
    //ctor
    }
    void Manager::formDefaultSkin()
    {
        Maigui::Default::form();
        skin = Maigui::Default::Skin;
    }
    template<typename T>
    bool checkPointer(std::shared_ptr<T> pointer)
    {
        if (pointer.get() != nullptr){return true;};
        return false;
    }
    Item* Manager::blankCompare(Item* shape1,Item* shape2)
    {
        float z = shape1->getPosition().z;
        float z1 = shape2->getPosition().z;
        if (z > z1 or z == z1)
        {
            return shape1;
        }else
        {
            return shape2;
        }
    }
    bool Manager::compare(std::shared_ptr<Item> shape)
    {
        if (activeItem.get() == nullptr)
        {
            activeItem = shape;
            activeItem->setFocus(true);
        }
        else if (shape.get() != activeItem.get())
        {
            float z = shape->getPosition().z;
            float z1 = activeItem->getPosition().z;
            if (z > z1 or z == z1)
            {
                activeItem->setFocus(false);

                activeItem = shape->shared_from_this();
                shape->setFocus(true);
                return true;
            }
        }
        return false;
    }
    void Manager::eraseFromCollisions(Item* itemData)
    {
            std::shared_ptr<Item> nextInLine;
            Utils::erase_if_once(&ActiveCollisions,[this,itemData,&nextInLine](std::shared_ptr<Item> b)
             {
                 if (b.get() == itemData)
                 {
                     if (b->hasFocus()){b->setFocus(false);activeItem=nullptr;};
                     return true;
                 }
                 return false;
             });
             if (activeItem.get()==nullptr && ActiveCollisions.size() > 0)
             {
                 nextInLine = ActiveCollisions[0];
                 for (unsigned int i=1; i < ActiveCollisions.size();i++)
                 {
                    nextInLine = blankCompare(nextInLine.get(),ActiveCollisions[i].get())->getPtr();
                 }
                 activeItem = nextInLine;
                 activeItem->setFocus(true);

             }
    }
    void Manager::setRay(Vertex Eye,Vertex Forward)
    {
        if (Mouse.get() == nullptr)
        {
            Mouse = World->addRay(Eye,Forward);
            Mouse->OnCollide = [this](Shape *a)
            {
                void* UserData = a->UserData;
                Item* realData = static_cast<Item*>(UserData);
                ActiveCollisions.push_back(realData->shared_from_this());
                compare(realData->getPtr());
            };
            Mouse->OnRelease = [this](Shape *a)
            {
                void* UserData = a->UserData;
                Item* realData = static_cast<Item*>(UserData);
                eraseFromCollisions(realData);
            };
        }else
        {Mouse->setPoints(Eye,Forward);};
    }
    void Manager::remove()
    {
        for (auto v: Children)
        {
            v->remove();
        };
        ActiveCollisions.clear();
        Children.clear();
    }
    void Manager::remove(shared_ptr<Item>i)
    {
        Utils::erase_if(&Children,[i](shared_ptr<Item> b){
        if (b.get() == i.get())
            {
                b->remove();
                return true;

            }return false;
        });
        eraseFromCollisions(i.get());
    }
    void Manager::draw()
    {
        skin->PreDraw();
        for (auto v: Children)
        {
            v->draw();
        }
        skin->PostDraw();

    }
    void Manager::drawSkeleton()
    {
        World->drawChunks(true);
    }
    void Manager::update(double dt)
    {
        World->update();
        for (auto v: Children)
        {
            v->update(dt);
        }
    }
    Vertex Manager::getViewVertex()
    {
        return Vertex(-(400/2.0f)*globalScale.x,-(600/2.0f)*globalScale.y,-540*((globalScale.x + globalScale.y)/2));
    }
    void Manager::setGlobalScale(float x,float y)
    {
        globalScale = Vertex(x,y);
        for (auto v: Children)
        {
            v->setGlobalScale(globalScale);
        }
    }
    void Manager::mousepressed(string key)
    {
        if (checkPointer(activeItem)){activeItem->mousepressed(key);};
    }
    void Manager::mousereleased(string key)
    {
        if (checkPointer(activeItem)){activeItem->mousereleased(key);};
    }
    void Manager::keypressed(string key)
    {
        if (checkPointer(activeItem)){activeItem->keypressed(key);};
    }

    // ADDS
    void Manager::addItem(Maigui::Item* i)
    {
        i->setSkin(skin);
        i->create(this);
        i->setGlobalScale(globalScale);
        Children.push_back(i->getPtr());
    }
    void Manager::addItem(Maigui::Item* i,Vertex v,Vertex b)
    {
        addItem(i);
        i->moveTo(v);
        i->scaleTo(b);
    }

    shared_ptr<Container> Manager::addContainer(float x,float y,float z,float w,float h, float d)
    {
        auto c = std::make_shared<Container>();
        addItem(dynamic_cast<Maigui::Item*>(c.get()),Vertex(x,y,z),Vertex(w,h,d));
        return c;
    };
    shared_ptr<Containers::Frame> Manager::addFrame(float x,float y,float z,float w,float h,float d)
    {
        auto c = std::make_shared<Maigui::Containers::Frame>();
        addItem(dynamic_cast<Maigui::Item*>(c.get()),Vertex(x,y,z),Vertex(w,h,d));
        return c;
    };
    shared_ptr<Button> Manager::addButton(float x,float y,float z,float w,float h,float d)
    {
        auto c = std::make_shared<Button>();
        c->setSkin(skin);
        c->create(this);
        c->moveTo(x,y,z);
        c->scaleTo(w,h,d);
        c->setGlobalScale(globalScale);
        Children.push_back(c);

        return c;
    };
    shared_ptr<Widget> Manager::addWidget(float x,float y,float z,float w,float h, float d)
    {

    };

    Manager::~Manager()
    {
        delete World;
        //dtor
    }
}
}
