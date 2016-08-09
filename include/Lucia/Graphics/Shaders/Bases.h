#ifndef MIKUS_LUCIA_GRAPHICS_SHADERS_BASES_H
#define MIKUS_LUCIA_GRAPHICS_SHADERS_BASES_H
#include <Lucia/Graphics/Graphics.h>
#include <Lucia/Graphics/Shaders.h>
#include <Lucia/Graphics/Buffers/Canvas.h>
#include <Lucia/Graphics/Primitives/Quad.h>
namespace Lucia {
namespace Graphics
{
    namespace Shaders
    {
        namespace Bases
        {
            // fully virtual class
            class Canvas
            {
                public:
                    virtual bool generate(float w=0,float h=0);
                    virtual void clear(); // clears the buffer and recreates it
                    virtual void remove(); // removes the buffer completely and sets the class in passive mode
                    virtual GLuint getTextureID();
                    virtual bool done(){return !passive;};
                    virtual std::shared_ptr<Graphics::Canvas> getCanvas(){return Canvas;};
                    // [!]Your draw function should have a variable for passing a user canvas in[!]
                    // eg:  draw(x1,x2 ... , canvas* c);
                    virtual void attach();
                    virtual void detach();
                protected:
                    std::shared_ptr<Graphics::Canvas> Canvas;
                    float lw;
                    float lh;
                    bool passive = true;
            };
        }
    }
}
}
#endif // MIKUS_GRAPHICS_SHADERS_BASES_H
