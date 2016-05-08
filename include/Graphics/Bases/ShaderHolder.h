#ifndef MIKUS_GRAPHICS_BASES_SHADERHOLDER_H
#define MIKUS_GRAPHICS_BASES_SHADERHOLDER_H
#include <Graphics/Graphics.h>
#include <Graphics/Shaders.h>
#include <utilities/OpenGL.h>
namespace Graphics
{
    namespace Base
    {
        typedef Utils::OpenGL::Shader_Vars Shader_Vars;
        class ShaderHolder
        {
            public:
                //sets
                void setShaderVars(std::shared_ptr<Shader_Vars> v){Vars = v;};
                //gets
                std::shared_ptr<Shader_Vars> getShaderVars(){return Vars;};
                //
            protected:
                std::shared_ptr<Shader_Vars> Vars;
        };
    }
}

#endif // MIKUS_GRAPHICS_BASES_SHADERHOLDER_H