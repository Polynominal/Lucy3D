#ifndef MIKUS_LUCIA_GRAPHICS_BASES_INSTANTDRAW_H
#define MIKUS_LUCIA_GRAPHICS_BASES_INSTANTDRAW_H
#include <Lucia/Maths/Matrix.h>
#include <Lucia/Maths/Vertex.h>
#include <Lucia/Graphics/Shaders.h>

namespace Lucia{
namespace Graphics{
using namespace Maths;
    namespace Base{
        class InstantDraw
        {
            private:
            protected:
                virtual void instant_draw(Maths::Matrix<4> viewport,Maths::Matrix<4> model){};
            public:
                InstantDraw(){};
                virtual ~InstantDraw(){};
                virtual void instantDraw(
                    Vertex position,
                    Vertex rotation = Vertex(0,0,0),
                    Vertex scale = Vertex(1,1,1),
                    Vertex offset = Vertex(0,0,0),
                    Maths::Matrix<4> viewport = Maths::Matrix<4>()
                ){};
        };
    }
    namespace _Shaders
    {
        extern std::unique_ptr<Graphics::Shader> Instant;
    }
}};

#endif