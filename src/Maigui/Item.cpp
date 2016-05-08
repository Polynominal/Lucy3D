#include "Maigui/Item.h"
using namespace Maigui;
void Item::mousepressed(string key){
    if (key == "l")
    {
        OnClick(true);

    }else if(key == "r")
    {
        OnRightClick(true);

    }else if (key == "m")
    {
        OnMiddleClick(true);
    }
};
void Item::mousereleased(string key)
{
    if (key == "l")
    {
        OnClick(false);

    }else if(key == "r")
    {
        OnRightClick(false);

    }else if (key == "m")
    {
        OnMiddleClick(false);
    }
};
void Item::textinput(char c){};
void Item::keypressed(string key){};
void Item::create(Manager *m)
{
    resetDrawStates();
    setDrawState(Drawable::BASE,Name);

    Visible = true;
    Inactive = false;

    Instance = m;
    // add shape
    auto collider = m->World;
    if(Core.get() != nullptr)
    {
        Core->remove();
    };
    Core = collider->addBox(Vertex(0,0,0),Vertex(1,1,1));
    Core->UserData = this;
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
// for example you can remove the shape from one manager and add it to the other.
void Item::setInactive(bool condition){Inactive = condition;};
void Item::setFocus(bool condition){
    hasFocus = condition;
    if (lastFocus != condition){
        OnFocus(condition);
        lastFocus = condition;
    };
};
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
        Maigui::Base::Drawable::draw(Translation,hasFocus);
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
    if (needsRefresh){refresh();};
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
    if (needsRefresh)
    {
        Maigui::Base::Moveable::refresh();

        Core->moveTo(rPosition);
        Core->rotateTo(Rotation);
        Core->scaleTo(rDimensions);

        Core->OnCollide = [this](Shape *b){
            if (checkPointer(b->UserData))
            {onCollide(static_cast<Item*>(b->UserData));};
        };
        Core->OnRelease = [this](Shape *b){
            if (checkPointer(b->UserData))
                {onRelease(static_cast<Item*>(b->UserData));};
            };
    }
}
shared_ptr<Shape> Item::getShape(){return Core;};