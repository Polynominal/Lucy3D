#ifndef MIKUS_LUCIA_GRAPHICS_SHADERS_BLOOM_H
#define MIKUS_LUCIA_GRAPHICS_SHADERS_BLOOM_H

#include <Lucia/Graphics/Shaders.h>
#include <Lucia/Graphics/Shaders/Blur.h>
namespace Lucia {
namespace Graphics
{
    namespace Shaders
    {
        class Bloom: public Graphics::Shaders::Bases::Canvas
        {
            public:
                Bloom();
                // the degree to which one should bloom
                // it is limited to the initial size of the texture
                // what it does is it resizes the texture like this:
                // for each degree
                void parse(unsigned int degree,float firstSize=16.0f, Graphics::Canvas* c=nullptr,bool reset_blend=true);
                void draw();

                virtual ~Bloom();
            protected:
            private:
                std::unique_ptr<Graphics::Shaders::Blur> Blur;
        };
    }

}
}
#endif // MIKUS_GRAPHICS_SHADERS_BLOOM_H
