#ifndef MIKUS_GLES2_BASIC2D_H
#define MIKUS_GLES2_BASIC2D_H
#include <Settings.h>
#include <memory>
#include <Graphics/Shaders.h>

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
#endif // BASIC2D_H
