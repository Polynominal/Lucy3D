#ifndef MIKUS_LUCIA_GRAPHICS_SHADER_SCENE2D_H
#define MIKUS_LUCIA_GRAPHICS_SHADER_SCENE2D_H
#include <Lucia/Settings.h>
#include <Lucia/Graphics/Shaders.h>
#include <Lucia/Maths/Matrix.h>

namespace Lucia {
namespace Graphics
{
    namespace _Shaders
    {
        std::unique_ptr<Graphics::Shader> Scene2D;
    }
    namespace Shaders
    {
        class Scene2D
        {
            public:
                Scene2D(){
                    if (Graphics::_Shaders::Scene2D.get() == nullptr)
                    {
                        string Vertex =
                        "#version 100\n"
                        "attribute vec3 vertex;"
                        "attribute vec2 texCord;"
                        "varying vec2 TexCord;"
                        "uniform mat4 model;"
                        "uniform mat4 view;"
                        "uniform mat4 projection;"
                        "void main()";
                        "{"
                        "   TexCord = texCord;"
                        "   gl_Position = model * view * projection * vertex;"
                        "}";
                        string Fragment =
                        "#version 100\n"
                        "precision highp float;"
                        "varying vec2 TexCord;"
                        "uniform sampler2D Texture;"
                        "void main()"
                        "{"
                        "   gl_FragColor = texture2D(Texture,TexCord);"
                        "}";
                        Graphics::_Shaders::Scene2D.reset(new Graphics::Shader());
                        Graphics::_Shaders::Scene2D->build(Vertex,Fragment,"Scene2D");
                        Graphics::_Shaders::Scene2D->addVar("vertex",0,3);
                        Graphics::_Shaders::Scene2D->addVar("texCord",0,2);
                    }
                };
                void Use()
                {
                    Graphics::_Shaders::Scene2D->use();
                };
                Utils::OpenGL::Shader_Vars* getVars()
                {
                    Graphics::_Shaders::Scene2D->getVars();
                }
                void send(Maths::Matrix<4> model,Maths::Matrix<4> view,Maths::Matrix<4> projection)
                {
                    auto ID = Graphics::_Shaders::Scene2D->programID;
                    glUniformMatrix4fv(glGetAttribLocation(ID,"model"),1,GL_FALSE,model.unpack());
                    glUniformMatrix4fv(glGetAttribLocation(ID,"view"),1,GL_FALSE,view.unpack());
                    glUniformMatrix4fv(glGetAttribLocation(ID,"projection"),1,GL_FALSE,projection.unpack());
                };
                virtual ~Scene2D() {

                }
            protected:
            private:
        };
    }
}
}
#endif // MIKUS_GRAPHICS_SHADER_SCENE2D_H
