#ifndef MIKUS_LUCIA_SCENE_OBJECT_H
#define MIKUS_LUCIA_SCENE_OBJECT_H

#include <Lucia\Maths\Matrix.h>
#include <Lucia\Maths\Moveable.h>

#include <Lucia\Graphics\Graphics.h>
#include <Lucia\Graphics\Bases\DrawMode.h>
#include <Lucia\Graphics\Bases\ShaderHolder.h>
#include <Lucia\Graphics\Scene\Instance.h>
#include <Lucia\Utils\OpenGL.h>
#include <algorithm>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <gtx/string_cast.hpp>

namespace Lucia {
namespace Graphics
{
    namespace Scene
    {
        class Object:
        public Graphics::Base::DrawMode,
        public Graphics::Base::ShaderHolder,
        public std::enable_shared_from_this<Object>
        {
            public:
                Object(){};
                virtual ~Object(){};
                //set
                virtual void setScene(Graphics::Scene::Instance* s){Core_Scene = s;};
                //these must be provided to you by the item object is attached to.
                virtual void useShader(Maths::Matrix<4>* view,Maths::Matrix<4>* projection)
                {
                    auto vars = getShaderVars();
                    vars->use();

                    vars->sendMatrix("view",4,4,true,view->unpack());
                    vars->sendMatrix("projection",4,4,false,projection->unpack());
                };
                virtual void preDraw(){}; // put your image binding here works like bind but I decided that bind is too common causing name clashes.
                virtual void render(Maths::Matrix<4>* model,DRAW mode)=0; // rendering assuming that everything is bound correctly.
                virtual void update(double dt){};
                //

                virtual void render(Maths::Matrix<4>* model){render(model,getDrawMode());};
                //get
                std::shared_ptr<Object> getObjectPtr(){return shared_from_this();};
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
        class Container: public Maths::Moveable, public Graphics::Base::DrawMode
        {
        public:
            Container(std::shared_ptr<Object> o){Core = o;};
            std::shared_ptr<Object> getCore(){return Core;};
            virtual ~Container(){};
        private:
            std::shared_ptr<Object> Core;
        };
    }
}}


#endif // SCENE_OBJECT_H
