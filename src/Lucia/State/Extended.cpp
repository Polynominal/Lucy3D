#include "Lucia\State\Extended.h"
using namespace Lucia;
using namespace Maths;
using namespace State;
std::shared_ptr<Maigui::Manager> Extended::generateEmptyUI(std::shared_ptr<Maigui::Skin> skin)
{
    if (UserInterface.get() == nullptr)
    {
        auto m = std::make_shared<Maigui::Manager>();
        if (skin.get()==nullptr){m->formDefaultSkin();};
        return m;
        
    }else{removeUserInterface(); return UserInterface;};
};

std::shared_ptr<Maigui::Manager> Extended::generateUserInterface(std::shared_ptr<Maigui::Skin> skin)
{
    return generateUserInterface(Window->getWidth(),Window->getHeight(),skin);
}
std::shared_ptr<Maigui::Manager> Extended::generateUserInterface(float w,float h,std::shared_ptr<Maigui::Skin> skin)
{
    camera = nullptr;
    isUIOrthographic = true;
    //we need a large amount of size for otrhographic to add a Z-limit or UI element limit, assuming each has z+1
    UserInterface = generateEmptyUI(skin);
    if (skin.get() ==nullptr){skin = Maigui::Default::Skin;};
    
    UIProjection.reset(new Ortho(0.0f,w,0.0f,h,0.0f,100000000));
    UIView.reset(new Matrix<4>());
    Lucia::Maigui::OpenGL::attachWrappers(skin.get(),UIView.get(),UIProjection.get());
    UserInterface->formDefaultSkin();
    
    return UserInterface;
}
std::shared_ptr<Maigui::Manager> Extended::generateUserInterface(Graphics::Camera* cam,std::shared_ptr<Maigui::Skin> skin,bool ortho)
{
    camera = cam;
    isUIOrthographic = ortho;
    UserInterface = generateEmptyUI(skin);
    if (skin.get() ==nullptr){skin = Maigui::Default::Skin;};
    
    Lucia::Maigui::OpenGL::attachWrappers(skin.get(),&cam->projection,&cam->view);
    UserInterface->formDefaultSkin();
    
    return UserInterface;
};
void Extended::removeUserInterface()
{
    if (UserInterface.get() != nullptr)
    {
        camera = nullptr;
        UserInterface = nullptr;
        Lucia::Maigui::OpenGL::cleanUp();
        UIProjection=nullptr;
        UIView=nullptr;
    };
}


std::shared_ptr<Collider::Manager> Extended::generateEmptyCollider()
{
    if (Collider.get() == nullptr)
    {
        auto m = std::make_shared<Lucia::Collider::Manager>();
        return m;
        
    }else{removeCollider(); return Collider;};
};
std::shared_ptr<Collider::Manager> Extended::generateCollider()
{
    return generateCollider(Window->getWidth(),Window->getHeight());
};
std::shared_ptr<Collider::Manager> Extended::generateCollider(float w,float h)
{
    //we need a large amount of size for otrhographic to add a Z-limit or UI element limit, assuming each has z+
    Collider = generateEmptyCollider();
    ColliderProjection.reset(new Perspective(45.0f,float(w/h),0.1f,10000.0f));
    ColliderView.reset(new Matrix<4>());
    
    Lucia::Collider_OpenGL::createWrappers(Collider.get(),ColliderView.get(),ColliderProjection.get());
    
    return Collider;
};
std::shared_ptr<Collider::Manager> Extended::generateCollider(Graphics::Camera* cam)
{
    Collider = generateEmptyCollider();
    Lucia::Collider_OpenGL::createWrappers(Collider.get(),&cam->view,&cam->projection);
    return Collider;
};
void Extended::applyColliderWraperToUI()
{
    if (UserInterface.get() != nullptr)
    {
        if (camera != nullptr)
        {
            Lucia::Collider_OpenGL::createWrappers(UserInterface->getWorld().get(),&camera->view,&camera->projection);
            
        }else 
        {
            Lucia::Collider_OpenGL::createWrappers(UserInterface->getWorld().get(),UIView.get(),UIProjection.get());
        }
    }
};
void Extended::removeCollider()
{
    if (Collider.get() != nullptr)
    {
        Collider = nullptr;
        Lucia::Collider_OpenGL::cleanup();
        ColliderView = nullptr;
        ColliderProjection = nullptr;
    };
}
void Extended::internalMousemotion(int x,int y,int relx,int rely)
{
    if (UserInterface.get() != nullptr)
    {
        Vertex a;
        Vertex b;
        if (isUIOrthographic)
        {
            a = Vertex(x,600-y,0);
            b = Vertex(x,600-y,10000);
            UserInterface->setRay(a,b);
            
        }else 
        {
            auto ray = camera->getRay(x,y);
            a = ray.first;
            b = ray.second;
        }
        
    }
    
};
void Extended::internalUpdate(double dt)
{
    if(Collider.get() != nullptr){Collider->update();};
    if(UserInterface.get() != nullptr){UserInterface->update(dt);};
}