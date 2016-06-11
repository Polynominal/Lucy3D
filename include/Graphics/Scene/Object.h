#ifndef MIKUS_SCENE_OBJECT_H
#define MIKUS_SCENE_OBJECT_H

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include <Graphics\Graphics.h>
#include <Graphics\Bases\Moveable.h>
#include <Graphics\Bases\DrawMode.h>
#include <Graphics\Bases\ShaderHolder.h>
#include <Graphics\Scene\Instance.h>
#include <utilities\OpenGL.h>
#include <algorithm>

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
                virtual void useShader(glm::mat4* view,glm::mat4* projection)
                {
                        auto vars = getShaderVars();
                        vars->use();
                        vars->sendMatrix("view",4,4,false,glm::value_ptr(*view));
                        vars->sendMatrix("projection",4,4,false,glm::value_ptr(*projection));
                };
                virtual void preDraw(){}; // put your image binding here works like bind but I decided that bind is too common causing name clashes.
                virtual void render(glm::mat4* model,DRAW mode)=0; // rendering assuming that everything is bound correctly.
                //
                virtual void render(glm::mat4* model){render(model,getDrawMode());};
                std::shared_ptr<Object> getObjectPtr(){return shared_from_this();};
                bool hasTransparency(){return transparency;};
            private:
                bool transparency = false;
                std::function<void()> OnPreDraw=[](){};
                std::function<void()> OnDraw=[](){};
                Graphics::Scene::Instance* Core_Scene=nullptr;
        };
        class Container: public Graphics::Base::Moveable, public Graphics::Base::DrawMode
        {
        public:
            Container(std::shared_ptr<Object> o){Core = o;};
            std::shared_ptr<Object> getCore(){return Core;};
            virtual ~Container(){};
        private:
            std::shared_ptr<Object> Core;
        };
    }
}


#endif // SCENE_OBJECT_H
