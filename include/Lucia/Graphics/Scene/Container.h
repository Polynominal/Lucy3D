#ifndef MIKUS_LUCIA_SCENE_CONTAINER_H
#define MIKUS_LUCIA_SCENE_CONTAINER_H

#include <Lucia\Maths\Moveable.h>

#include <Lucia\Graphics\Graphics.h>
#include <Lucia\Graphics\Bases\DrawMode.h>
#include <Lucia\Graphics\Bases\ShaderHolder.h>
#include <Lucia\Graphics\Bases\Color.h>

namespace Lucia {
namespace Graphics
{
    namespace Scene
    {
        class Object;
        class Container: public Maths::Moveable, public Graphics::Base::DrawMode, public Graphics::Base::Color
        {
            public:
                Container(std::shared_ptr<Object> o){Core = o;};
                Container(){};
                //get
                virtual Object* getCore();
                virtual void removeFromScene();
                virtual void setScene(Graphics::Scene::Instance*s);
                virtual void onMorph();
                virtual void onMove();
                virtual void onScale();
                virtual void onRotate();
                virtual void updateInstance();
                
                virtual ~Container(){};
            private:
                Graphics::Scene::Instance* scene;
                std::shared_ptr<Object> Core;
        };
    }
}
}
#endif