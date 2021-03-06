#include "Lucia/Maigui/Manager.h"
#include <Lucia/Maigui/Types/Container.h>
#include <Lucia/Maigui/Types/Widget.h>
#include <Lucia/Maigui/Types/Frame.h>
#include <Lucia/Maigui/Types/Button.h>

using namespace Lucia;
using namespace Maths;
namespace Lucia {
namespace Maigui
{
    Manager::Manager()
    {
        skin = Maigui::Default::Skin;
        World = std::make_shared<Collider::Manager>();
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

                activeItem = shape->getPtr();
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
            ActiveCollisions.clear();
            Mouse = World->addRay(Eye,Forward);
            Mouse->OnCollide = [this](Shape *a)
            {
                void* UserData = a->UserData;
                Item* realData = static_cast<Item*>(UserData);
                ActiveCollisions.push_back(realData->getPtr());
                compare(realData->getPtr());
            };
            Mouse->OnRelease = [this](Shape *a)
            {
                void* UserData = a->UserData;
                Item* realData = static_cast<Item*>(UserData);
                eraseFromCollisions(realData);
            };
        }else
        {
            Mouse->setPoints(Eye,Forward);
        };
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
        boundSkin = nullptr;
        
        for (auto v: Children)
        {
            auto mySkin = v->getSkin();
            bool sameSkin = (mySkin == boundSkin);
            if (not sameSkin){boundSkin = mySkin; boundSkin->PreDraw();};
            v->draw();
            
            if (not sameSkin){boundSkin->PostDraw();};
        }
        
        boundSkin = nullptr;
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
    
    
    Maigui::Item* Manager::findItem(string name)
    {
        auto it = std::find_if(Children.begin(), Children.end(), [name](std::shared_ptr<Item> i){return i->getReference() == name;});
        if (it != Children.end())
        {
            return it->get();
            
        }else
        {
            return nullptr;
        }
    }
    // ADDS
    void Manager::addItem(Maigui::Item* i)
    {
        i->setSkin(skin);
        i->create(this);
        i->setGlobalScale(globalScale);
        
        Children.push_back(i->getPtr());
        std::sort(Children.begin(),Children.end(),[](std::shared_ptr<Item> a,std::shared_ptr<Item> b)
        {
            return a->getPosition().z < b->getPosition().z;
        });
    }
    void Manager::addItem(Maigui::Item* i,Vertex v,Vertex b)
    {
        addItem(i);
        i->moveTo(v);
        i->scaleTo(b);
    }
    void Manager::mousemoved(bool* key,int x,int y,int dx,int dy)
    {
        if (key[0])//l key is down
        {
            if (activeItem.get() and activeItem->hasFocus()){
                activeItem->drag(dx,-dy);
            }
        }
    }
    shared_ptr<Container> Manager::addContainer(float x,float y,float z,float w,float h, float d,bool add)
    {
        auto c = std::make_shared<Container>();
        if (add){addItem(dynamic_cast<Maigui::Item*>(c.get()),Vertex(x,y,z),Vertex(w,h,d));};
        return c;
    };
    shared_ptr<Containers::Frame> Manager::addFrame(float x,float y,float z,float w,float h,float d,bool add)
    {
        auto c = std::make_shared<Maigui::Containers::Frame>();
        if (add){addItem(dynamic_cast<Maigui::Item*>(c.get()),Vertex(x,y,z),Vertex(w,h,d));};
        return c;
    };
    shared_ptr<Button> Manager::addButton(float x,float y,float z,float w,float h,float d,bool add)
    {
        auto c = std::make_shared<Button>();
        c->generate(Vertex(x,y,z),Vertex(w,h,d),Maigui::Default::Skin);
        if (add){addItem(dynamic_cast<Maigui::Item*>(c.get()),Vertex(x,y,z),Vertex(w,h,d));};
        return c;
    };
    shared_ptr<Widget> Manager::addWidget(float x,float y,float z,float w,float h, float d,bool add)
    {

    };

    Manager::~Manager()
    {
    }
}
}
