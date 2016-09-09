#include "Lucia/Graphics/Shaders/Scene3D.h"
using namespace Lucia;
namespace Lucia{
namespace Graphics{
    namespace _Shaders
    {
        std::unique_ptr<Graphics::Shader> Scene3D;
    }
}}

using namespace Graphics;
using namespace Shaders;
Scene3D::Scene3D()
{
    if (Graphics::_Shaders::Scene3D.get() == nullptr)
    {
        string Vertex =
        "#version 100\n"
        "attribute vec3 vertex;"
        "attribute vec2 texCord;"
        "varying vec2 TexCord;"
        "varying vec4 color;"
        "uniform vec4 Color;"
        "uniform mat4 model;"
        "uniform mat4 view;"
        "uniform mat4 projection;"
        "void main()"
        "{"
        "   color = Color;"
        "   TexCord = texCord;"
        "   gl_Position = model * view * projection * vertex;"
        "}";
        string Fragment =
        "#version 100\n"
        "precision highp float;"
        "varying vec2 TexCord;"
        "varying vec4 color;"
        "uniform sampler2D Texture;"
        "void main()"
        "{"
        "   gl_FragColor = color*texture2D(Texture,TexCord);"
        "}";
        Graphics::_Shaders::Scene3D.reset(new Graphics::Shader());
        Graphics::_Shaders::Scene3D->build(Vertex,Fragment,"Scene3D");
        Graphics::_Shaders::Scene3D->addVar("vertex",0,3);
        Graphics::_Shaders::Scene3D->addVar("texCord",3,5);
    }
};
void Scene3D::Use()
{
    Graphics::_Shaders::Scene3D->use();
};

std::shared_ptr<Utils::OpenGL::Shader_Vars> Scene3D::getVars()
{
    return Graphics::_Shaders::Scene3D->getVars();
};

void Scene3D::send(Maths::Matrix<4> model,Maths::Matrix<4> view,Maths::Matrix<4> projection)
{
    auto ID = Graphics::_Shaders::Scene3D->programID;
    glUniformMatrix4fv(glGetAttribLocation(ID,"model"),1,GL_TRUE,model.unpack());
    glUniformMatrix4fv(glGetAttribLocation(ID,"view"),1,GL_TRUE,view.unpack());
    glUniformMatrix4fv(glGetAttribLocation(ID,"projection"),1,GL_FALSE,projection.unpack());
};
