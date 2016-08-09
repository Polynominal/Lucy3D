#ifndef MIKUS_LUCIA_GRAPHICS_BASES_SHADERHOLDER_H
#define MIKUS_LUCIA_GRAPHICS_BASES_SHADERHOLDER_H
#include <Lucia/Graphics/Graphics.h>
#include <Lucia/Graphics/Shaders.h>
#include <Lucia/Utils/OpenGL.h>
namespace Lucia {
namespace Graphics
{
    namespace Base
    {
        typedef Utils::OpenGL::Shader_Vars Shader_Vars;
        class ShaderHolder
        {
            public:
                //sets
                virtual void setShaderVars(std::shared_ptr<Shader_Vars> v){ShaderVars = v;};
                //gets
                virtual std::shared_ptr<Shader_Vars> getShaderVars(){return ShaderVars;};
                //
            protected:
                std::shared_ptr<Shader_Vars> ShaderVars;
        };
    }
}}

#endif // MIKUS_GRAPHICS_BASES_SHADERHOLDER_H