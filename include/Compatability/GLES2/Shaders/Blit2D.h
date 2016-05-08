#ifndef MIKUS_GRAPHICS_SHADERS_BLIT2D_H
#define MIKUS_GRAPHICS_SHADERS_BLIT2D_H
#include <Graphics/Graphics.h>
#include <Graphics/Shaders.h>

namespace Graphics
{
    class Shader;
    namespace Shaders
    {
        class Blit2D
        {
            public:
                Blit2D();
                void sendData(float x,float y);
                virtual ~Blit2D();
        };
    }
    namespace _Shaders
    {
        extern std::shared_ptr<Graphics::Shader> Blit2D;
    }
}

#endif // MIKUS_GRAPHICS_SHADERS_BLIT2D_H
