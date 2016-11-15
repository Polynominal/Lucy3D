#ifndef LUCIA_STATE_EXTENDEDSTATE
#define LUCIA_STATE_EXTENDEDSTATE

#include <Lucia\State\Gamestate.h>

#include <Lucia\Maigui\Manager.h>
#include <Lucia\Maigui\Wrappers\OpenGL.h>

#include <Lucia\Collider\Manager.h>

#include <Lucia\Graphics\Camera.h>

#include <Lucia\Controll\Mouse.h>

#if defined LUCIA_USE_GLES2 || defined LUCIA_USE_OPENGL3
    #include <Lucia\Maigui\Wrappers\OpenGL.h>
    #include <Lucia\Collider\OpenGL.h>
#endif 
#include <Lucia/Maigui/Editor/Parser.h>

namespace Lucia{
    namespace State{
        class Extended:
        public Gamestate
        {
            public:
                Extended(){};
                virtual ~Extended(){};
                
                virtual void useDefaultSkinUI(bool f=true){formDefaultUISkin = f;};
                //inherited
                virtual void update(double delta_time);
                virtual void mousemotion(int x,int y,int relx,int rely);
                virtual void mousepressed(std::string key,int x ,int y);
                virtual void mousereleased(std::string key,int x ,int y);
                //create
                virtual std::shared_ptr<Maigui::Manager> generateEmptyUI(std::shared_ptr<Maigui::Skin> skin=nullptr);
                
                virtual std::shared_ptr<Maigui::Manager> generateUserInterface(std::shared_ptr<Maigui::Skin> skin=nullptr);
                virtual std::shared_ptr<Maigui::Manager> generateUserInterface(float w,float h,std::shared_ptr<Maigui::Skin> skin=nullptr);
                virtual std::shared_ptr<Maigui::Manager> generateUserInterface(Graphics::Camera* camera,std::shared_ptr<Maigui::Skin> skin=nullptr,bool orthographic=false);
                
                virtual std::shared_ptr<Collider::Manager> generateEmptyCollider();
                virtual std::shared_ptr<Collider::Manager> generateCollider();
                virtual std::shared_ptr<Collider::Manager> generateCollider(float w,float h);
                virtual std::shared_ptr<Collider::Manager> generateCollider(Graphics::Camera* camera);
                //additional/misc
                virtual void applyColliderWraperToUI();
                //remove
                virtual void removeUserInterface();
                virtual void removeCollider();
                
                std::shared_ptr<Collider::Manager> Collider;
                std::shared_ptr<Maigui::Manager> UserInterface;
                
                Graphics::Camera* camera = nullptr;
                std::unique_ptr<Maths::Matrix<4>> UIProjection;
                std::unique_ptr<Maths::Matrix<4>> UIView;
                
                std::unique_ptr<Maths::Matrix<4>> ColliderProjection;
                std::unique_ptr<Maths::Matrix<4>> ColliderView;
            
            private:
                bool isUIOrthographic = true;
                bool formDefaultUISkin = false;


        };
    }
}
#endif