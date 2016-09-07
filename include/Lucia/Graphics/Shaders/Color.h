#ifndef MIKUS_LUCIA_GRAPHICS_SHADER_COLOR_H
#define MIKUS_LUCIA_GRAPHICS_SHADER_COLOR_H
#include <Lucia/Settings.h>
#include <Lucia/Graphics/Shaders.h>
#include <Lucia/Maths/Matrix.h>

namespace Lucia {
namespace Graphics
{
    namespace _Shaders
    {
        std::unique_ptr<Graphics::Shader> Color;
    }
    namespace Shaders
    {
        class Color
        {
            public:
                Color(){
                    if (Graphics::_Shaders::Color.get() == nullptr)
                    {
                        string Vertex =
                        "#version 100\n"
                        "attribute vec3 vertex;"
                        "varying vec4 color;"
                        "uniform vec4 Color;"
                        "uniform mat4 model;"
                        "uniform mat4 view;"
                        "uniform mat4 projection;"
                        "void main()";
                        "{"
                        "   color = Color;"
                        "   gl_Position = model * view * projection * vertex;"
                        "}";
                        string Fragment =
                        "#version 100\n"
                        "precision highp float;"
                        "varying vec4 color;"
                        "void main()"
                        "{"
                        "   gl_FragColor = color;"
                        "}";
                        Graphics::_Shaders::Color.reset(new Graphics::Shader());
                        Graphics::_Shaders::Color->build(Vertex,Fragment,"Color");
                        Graphics::_Shaders::Color->addVar("vertex",0,3);
                    }
                };
                void Use()
                {
                    Graphics::_Shaders::Color->use();
                };
                Utils::OpenGL::Shader_Vars* getVars()
                {
                    Graphics::_Shaders::Color->getVars();
                }
                virtual ~Color() {

                }
            protected:
            private:
        };
    }
}
}
#endif // MIKUS_GRAPHICS_SHADER_SCENE2D_H
