#ifndef MIKUS_LUCIA_GRAPHICS_PRIMITIVES_PLATE_H
#define MIKUS_LUCIA_GRAPHICS_PRIMITIVES_PLATE_H
#include <Lucia/Settings.h>
#include <Lucia/Graphics/Bases/Primitive.h>

namespace Lucia{
namespace Graphics {
namespace Primitive{
    class Plate:
    public Graphics::Base::Primitive
    {
        public:
            Plate(){};
            virtual ~Plate(){};
            void generate(unsigned int id){
                //create a buffer and load shader vars
                auto SVars = std::make_shared<Utils::OpenGL::Shader_Vars>();
                SVars->setProgram(id);
                SVars->add("vertex",0,3);
                SVars->add("uv",3,5);
                makeBuffer(SVars);
                
                float data[30] = {
                    -0.5f, -0.5f, 0.0,  0.0f, 0.0f,
                    0.5f, -0.5f, 0.0f,  1.0f, 0.0f,
                    0.5f,  0.5f, 0.0f,  1.0f, 1.0f,
                    0.5f,  0.5f, 0.0f,  1.0f, 1.0f,
                    -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
                    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f
                };
                Buf->setData(data,6);
                complete = true;
            };
            
        private:
            bool complete = false;
    };

}
}}
#ifdef GENERATE_SHARED_RECOURCES
namespace Lucia{
namespace Graphics{
    namespace Shared
    {
        extern std::unique_ptr<Primitive::Plate> Plate;
    }
}}
#endif
#endif