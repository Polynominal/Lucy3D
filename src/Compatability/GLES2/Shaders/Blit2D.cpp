#include "Compatability/GLES2/Shaders/Blit2D.h"
namespace Graphics
{
    namespace _Shaders
    {
        std::shared_ptr<Graphics::Shader> Blit2D = std::shared_ptr<Graphics::Shader>(new Graphics::Shader);
    }
}
typedef Graphics::Shaders::Blit2D Blit2D;
Blit2D::Blit2D()
{
    if (!Graphics::_Shaders::Blit2D->isBuilt())
    {
        std::cout << "Building Blit2D Shader" << std::endl;
        std::string Vertex =
        "#version 100\n"
        "attribute vec2 vertex;\n"
        "attribute vec2 uv;\n"
        "uniform vec2 Position;\n"
        "varying vec2 TexCoords;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(Position.x + vertex.x,Position.y + vertex.y, 0.0f, 1.0f);\n"
        "   TexCoords = uv;\n"
        "}; \n";
        std::string Fragment =
        "#version 100 \n"
        "varying highp vec2 TexCoords;\n"
        "uniform sampler2D Texture;\n"
        "void main()"
        "{"
        "   gl_FragColor=texture2D(Texture,TexCoords);"
        "};";
        Graphics::_Shaders::Blit2D->build(Vertex,Fragment,"Blit2D");
    }
    //ctor
}
void Blit2D::sendData(float x,float y)
{
    GLint v  = glGetAttribLocation(Graphics::_Shaders::Blit2D->programID,"vertex");

    glEnableVertexAttribArray(v);
    glVertexAttribPointer(v, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    GLint uv = glGetAttribLocation(Graphics::_Shaders::Blit2D->programID,"uv");

    glEnableVertexAttribArray(uv);
    glVertexAttribPointer(uv, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));

    glUniform2i(glGetUniformLocation(Graphics::_Shaders::Blit2D->programID, "Position"),x,y);
    glUniform1i(glGetUniformLocation(Graphics::_Shaders::Blit2D->programID, "Texture"),0);
}
Blit2D::~Blit2D()
{
    //dtor
}

