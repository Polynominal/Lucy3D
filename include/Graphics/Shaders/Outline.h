#ifndef MIKUS_GRAPHICS_SHADER_OUTLINE_H
#define MIKUS_GRAPHICS_SHADER_OUTLINE_H
#include <Graphics/Graphics.h>
#include <Graphics/Shaders/Bases.h>

namespace Graphics
{
    namespace Shaders
    {
        class Outline: public Graphics::Shaders::Bases::Canvas
        {
            public:
                Outline();
                void draw(float thickness=2.0f,float r=0.0f,float g=0.0f,float b=0.0f,float a=1.0f,Graphics::Canvas* c=nullptr);
                virtual ~Outline();
        };
    }
    namespace _Shaders
    {
        extern std::unique_ptr<Shader> Outline;
    }
}
#endif // MIKUS_GRAPHICS_SHADER_OUTLINE_H
