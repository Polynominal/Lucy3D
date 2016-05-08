#ifndef MIKUS_GRAPHICS_SHADERS_BLUR_H
#define MIKUS_GRAPHICS_SHADERS_BLUR_H
#include <Graphics/Graphics.h>
#include <Graphics/Shaders/Bases.h>

namespace Graphics
{
    namespace Shaders
    {
        class Blur: public Graphics::Shaders::Bases::Canvas
        {
            public:
                enum Mode
                {
                    Kawase,
                    Gaus5x5,
                    Gaus
                };
                Blur();
                void draw(Graphics::Canvas* c,int Passes,float degree);
                void setMode(Mode m){mode = m;};
                virtual ~Blur();
            private:
                Mode mode = Mode::Kawase;
        };
    }
    namespace _Shaders
    {
        extern std::unique_ptr<Shader> Kawase;
        extern std::unique_ptr<Shader> Gaus5x5;
        extern std::unique_ptr<Shader> Gaus;
    }
}

#endif // MIKUS_GRAPHICS_BLUR_H
