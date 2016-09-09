#ifndef MIKUS_LUCIA_SCENE_OBJECT_H
#define MIKUS_LUCIA_SCENE_OBJECT_H

#include <Lucia\Maths\Matrix.h>
#include <Lucia\Maths\Moveable.h>

#include <Lucia\Graphics\Graphics.h>
#include <Lucia\Graphics\Bases\DrawMode.h>
#include <Lucia\Graphics\Bases\ShaderHolder.h>
#include <Lucia\Graphics\Bases\Color.h>
#include <Lucia\Graphics\Scene\Instance.h>
#include <Lucia\Graphics\Scene\Container.h>
#include <Lucia\Utils\OpenGL.h>
#include <memory>

#include <algorithm>


namespace Lucia {
namespace Graphics
{
    namespace Scene
    {
        class Object:
        public Graphics::Base::DrawMode,
        public Graphics::Base::ShaderHolder
        {
            public:
                Object(){};
                virtual ~Object(){};
                //set
                virtual void setScene(Graphics::Scene::Instance* s){Core_Scene = s;};
                virtual Graphics::Scene::Instance* getScene(){return Core_Scene;};
                //these must be provided to you by the item object is attached to.
                virtual void useShader(Maths::Matrix<4>* view,Maths::Matrix<4>* projection)
                {
                    auto vars = getShaderVars();
                    vars->use();
                    vars->sendMatrix("view",4,4,true,view->unpack());
                    vars->sendMatrix("projection",4,4,false,projection->unpack());
                };
                virtual void preDraw(){}; // put your image binding here works like bind but I decided that bind is too common causing name clashes.
                //VTABLE FUNC
                
                virtual void render(Container* c,DRAW mode){render(c->getModelMatrix(),c->getDrawMode());};
                //legacy. support becomes discontinued when u use the render method above this.
                virtual void render(Maths::Matrix<4>* model, DRAW mode){};
                
                virtual void update(double dt){};
                virtual void update(double dt,Container *c){update(dt);};
                //
                virtual void onMorph(Container*){};
                virtual void onMove(Container*){};
                virtual void onScale(Container*){};
                virtual void onRotate(Container*){};
                virtual void updateInstance(Container*){};
                
                virtual void render(Container* c){render(c,getDrawMode());};
                //
                //has
                bool hasTransparency(){return transparency;};
                //
            private:
                bool transparency = false;
                std::function<void()> OnPreDraw=[](){};
                std::function<void()> OnDraw=[](){};
                Graphics::Scene::Instance* Core_Scene=nullptr;
        };
        class Item: public Object, public Container, public std::enable_shared_from_this<Item>
        {
            public: 
                Item(){};
                virtual ~Item(){};
                std::shared_ptr<Item> getSceneNode(){return std::enable_shared_from_this<Item>::shared_from_this();};
                Object* getCore(){return this;};
        };
    }
}}


#endif // SCENE_OBJECT_H
