#ifndef MIKUS_LUCIA_GRAPHICS_SHADER_SCENE3D_H
#define MIKUS_LUCIA_GRAPHICS_SHADER_SCENE3D_H
#include <Lucia/Settings.h>
#include <Lucia/Graphics/Shaders.h>
#include <Lucia/Maths/Matrix.h>

namespace Lucia {
namespace Graphics
{
    namespace _Shaders
    {
        extern std::unique_ptr<Graphics::Shader> Scene3D;
    }
    namespace Shaders
    {
        class Scene3D
        {
            public:
                Scene3D();
                void Use();
                std::shared_ptr<Utils::OpenGL::Shader_Vars> getVars();
                void send(Maths::Matrix<4> model,Maths::Matrix<4> view,Maths::Matrix<4> projection);
                virtual ~Scene3D(){};
        };
    }
}
}
#endif // MIKUS_GRAPHICS_SHADER_SCENE3D_H
