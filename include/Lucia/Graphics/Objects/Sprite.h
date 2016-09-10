#ifndef MIKUS_LUCIA_GRAPHICS_OBJECT_SPRITE_H
#define MIKUS_LUCIA_GRAPHICS_OBJECT_SPRITE_H

#include <Lucia/Graphics/Scene/Object.h>
#include <Lucia/Graphics/Buffers/Spritesheet.h>

namespace Lucia{
namespace Graphics{
namespace Object{
    

class Sprite: public Graphics::Scene::Object,public Lucia::Maths::Sprite
{
        public:
        Sprite(){};
        Sprite(std::shared_ptr<Buffer::Spritesheet> Parent,float x,float y,float w,float h)
        {
            create(parent,x,y,w,h);
            setTransparency(true);
        };
        void create(std::shared_ptr<Buffer::Spritesheet> Parent,float x,float y,float w,float h)
        {
            Vec2 dim = Parent->getDimensions();
            parent = Parent;
            setShaderVars(Graphics::_Shaders::VaryingUV->getVars());
            
            Maths::Sprite::create(x/dim.x,
                                    y/dim.y,
                                    w/dim.x,
                                    h/dim.y);   
        }
        void fromSprite(std::shared_ptr<Buffer::Spritesheet> p ,Lucia::Maths::Sprite s)
        {
            parent = p;
            setShaderVars(Graphics::_Shaders::VaryingUV->getVars());
            create(p,s.x,s.y,s.w,s.h);
        };
        //get
        std::shared_ptr<Buffer::Spritesheet> getParent(){return parent;};
        //
        virtual void useShader(Maths::Matrix<4>* view,Maths::Matrix<4>* projection){parent->useShader(view,projection);};
        virtual void preDraw(){parent->preDraw();};
        virtual void render(Maths::Matrix<4>* model,DRAW mode){
            ShaderVars->send("color",1.0f,1.0f,1.0f,1.0f);
            ShaderVars->send("data",x,y,w,h);
            ShaderVars->sendMatrix("model",4,4,true,model->unpack());

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            Graphics::Shared::Quad->draw(Graphics::_Shaders::VaryingUV->getVars());
        };

        virtual ~Sprite(){};
    private:
        std::shared_ptr<Buffer::Spritesheet> parent;
};
}}}
#endif