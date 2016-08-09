#ifndef MIKUS_LUCIA_GRAPHICS_PRIMITIVES_QUAD_H
#define MIKUS_LUCIA_GRAPHICS_PRIMITIVES_QUAD_H
#include <Lucia/Graphics/Bases/Primitive.h>

namespace Lucia{
namespace Graphics {
namespace Base{
class Plate:
    public Graphics::Base::Primitive
{
    public:
        Plate(){};
        virtual ~Plate(){};
        void generate(unsigned int id){
            //create a buffer and load shader vars
            auto SVars = std::make_shared<Shader_Vars>();
            SVars->setProgram(id);
            SVars->add("vertex",0,3);
            SVars->add("uv",3,5);
            makeBuffer(Svars);
            
            float data[30] = {
                -0.5f, -0.5f, 0.0,  0.0f, 0.0f,
                0.5f, -0.5f, 0.0f,  1.0f, 0.0f,
                0.5f,  0.5f, 0.0f,  1.0f, 1.0f,
                0.5f,  0.5f, 0.0f,  1.0f, 1.0f,
                -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
                -0.5f, -0.5f, 0.0f, 0.0f, 0.0f
            };
            Buf->setData(&data,6);
            complete = true;
        };
        
    private:
        bool complete = false;
}
}}}
#endif