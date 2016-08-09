#ifndef MIKUS_LUCIA_GRAPHICS_BASES_PRIMITIVE_H
#define MIKUS_LUCIA_GRAPHICS_BASES_PRIMITIVE_H

#include <Lucia/Maths/Moveable.h>
#include <Lucia/Graphics/Bases/Color.h>
#include <Lucia/Graphics/Bases/ShaderHolder.h>
#include <Lucia/Graphics/Bases/InstantDraw.h>
#include <Lucia/Graphics/Bases/DrawMode.h>

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
            typedef Utils::OpenGL::Buffer Buffer;
            typedef Utils::OpenGL::Shader_Vars Shader_Vars;
            
            Primitive();
            virtual ~Primitive();
            
            public:
                virtual void generate(unsigned int id)
                {
                        
                };
                virtual void draw(unsigned int id=-1){
                    if (Buf->get()){
                        if (id == -1){Buf->draw(getMechanicalTarget(drawMode));}else{generate(id);};
                    }
                };
                virtual void isDone(){return complete;};
            
            private: 
                std::unique_ptr<Buffer> Buf;
                bool complete = false;
            
            protected:
                virtual void makeBuffer(Shader_Vars v)
                {
                    Buf.reset(new Buffer(SVars));
                    complete = true;
                };
                virtual void instant_draw(Maths::Matrix<4> viewport,Maths::Matrix<4> model)
                {
                    auto s = getShaderVars();
                    s->sendMatrix(viewport.unpack(),4,4,false,"view");
                    s->sendMatrix(model.unpack(),4,4,false,"model");
                    draw(s->getID());
                };
        }
}}};
#endif