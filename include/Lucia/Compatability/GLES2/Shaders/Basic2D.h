#ifndef MIKUS_LUCIA_GLES2_BASIC2D_H
#define MIKUS_LUCIA_GLES2_BASIC2D_H
#include <Lucia/Settings.h>
#include <memory>
#include <Lucia/Graphics/Shaders.h>
namespace Lucia {
namespace Graphics
{
    namespace Shaders
    {
        class Basic2D
        {
            public:
                Basic2D();
                void sendData(bool useColor,float r,float g,float b);
                void use();
                virtual ~Basic2D();
            protected:
            private:
        };
    }
}
}
#endif // BASIC2D_H
