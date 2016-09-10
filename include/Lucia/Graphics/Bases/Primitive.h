#ifndef MIKUS_LUCIA_GRAPHICS_BASES_PRIMITIVE_H
#define MIKUS_LUCIA_GRAPHICS_BASES_PRIMITIVE_H

#include <Lucia/Maths/Moveable.h>
#include <Lucia/Graphics/Bases/Color.h>
#include <Lucia/Graphics/Bases/ShaderHolder.h>
#include <Lucia/Graphics/Bases/InstantDraw.h>
#include <Lucia/Graphics/Bases/DrawMode.h>
#include <Lucia/Graphics/Bases/Filter.h>

namespace Lucia{
namespace Graphics{
namespace Base{
        class Primitive:
        
        public Maths::Moveable,
        public Graphics::Base::Color,
        public Graphics::Base::Filter,
        public Graphics::Base::ShaderHolder,
        public Graphics::Base::DrawMode,
        public Graphics::Base::InstantDraw
        
        {
            public:
                Primitive(){};
                virtual ~Primitive(){};
                
                virtual void generate(unsigned int id){};
                
                virtual void draw()
                {Buf->draw(getMechanicalTarget(getDrawMode()));};
                
                virtual void draw(std::shared_ptr<Utils::OpenGL::Shader_Vars> vars)
                {Buf->draw(GL_TRIANGLES,vars.get());};
                
                virtual bool isDone(){return complete;};
                
            protected:
                std::unique_ptr<Utils::OpenGL::Buffer> Buf;
                bool complete = false;
                
                virtual void makeBuffer(std::shared_ptr<Utils::OpenGL::Shader_Vars> v)
                {
                    Buf.reset(new Utils::OpenGL::Buffer(v));
                    complete = true;
                };
                virtual void instant_draw(Maths::Matrix<4> viewport,Maths::Matrix<4> model)
                {
                    auto s = getShaderVars();
                    s->sendMatrix("view",4,4,false,viewport.unpack());
                    s->sendMatrix("model",4,4,false,model.unpack());
                    draw(s);
                };
        };
}}};
#endif