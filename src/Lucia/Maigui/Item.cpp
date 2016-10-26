#include "Lucia/Maigui/Item.h"
using namespace Lucia;
using namespace Maigui;

void Item::create(Manager *m)
{
    
    resetDrawStates();
    setDrawState(Drawable::BASE,Name);
    std::cout << "Creating item: " << Name << std::endl;

    Visible = true;
    Inactive = false;

    Instance = m;
    // add shape
    auto collider = m->World;
    if(CoreShape.get() != nullptr)
    {
        CoreShape->remove();
    };
    CoreShape = collider->addBox(Vertex(0,0,0),Vertex(1,1,1));
    CoreShape->UserData = this;
    onCreate();
};
void Item::create(Item *m)
{
    Parent = m;
    create(m->Instance);
}; // creation within another item. Allows for recrusion.

void Item::remove()
{
    for(auto v: Children){v->remove();};
    Instance->remove(shared_from_this());
    if(Parent != nullptr){Parent->remove(this);Parent=nullptr;};
};
void Item::addItem(Item* item){auto p = item->getPosition(); addItem(item,p.x,p.y,p.z);};
// for example you can remove the shape from one manager and add it to the other.
void Item::addItem(Item* item,float x,float y,float z)
{
    auto w = item->getPtr();
    Vertex p = Position - (Dimensions/2 - w->Dimensions/2) + Vertex(x,y,z);
    bool found = false;
    for (auto v: Children)
    {
        if (v.get() == w.get())
        {
            found = true;
        };
    };
    if (not found){Children.push_back(w);};
    w->setSkin(this->skin);
    w->create(this);
    w->moveTo(p.x,p.y,p.z + 0.05f);
}
void Item::setSkin(shared_ptr<Skin> skin)
{
    Maigui::Base::Drawable::setSkin(skin);
    for (auto v: Children)
    {
        v->setSkin(skin);
    }
}
void Item::remove(Item* w)
{
    Utils::erase_if(&Children,[w](shared_ptr<Item>a)
    {
        if(a.get() == w)
        {
            a->remove();
            return true;
        }return false;
    });
}
void Item::draw()
{
    if (Visible)
    {
        OnDrawPre();
        Maigui::Base::Drawable::draw(Translation,hasFocus());
        OnDrawAfter();

        if (areItemsDrawn()){
            for (auto v: Children)
            {
                v->draw();
            }
        };
    }
};
void Item::update(double dt)
{
    OnUpdate(dt);
    refresh();
    for (auto v: Children)
    {
        v->update(dt);
    }
};
bool checkPointer(void *b)
{
    if (b != nullptr)
    {
        try{static_cast<Shape*>(b);}
        catch(...){return false;}
        return true;
    }
    return false;
}
void Item::refresh()
{
    if (Base::Moveable::needsRefresh)
    {
        Maigui::Base::Moveable::refresh();

        CoreShape->moveTo(getGlobalPosition());
        CoreShape->rotateTo(getRotation());
        CoreShape->scaleTo(getGlobalDimensions());

        CoreShape->OnCollide = [this](Shape *b){
            if (checkPointer(b->UserData))
            {onCollide(static_cast<Item*>(b->UserData));};
        };
        CoreShape->OnRelease = [this](Shape *b){
            if (checkPointer(b->UserData))
                {onRelease(static_cast<Item*>(b->UserData));};
            };
            
        Base::Moveable::needsRefresh =false;
    }
}
