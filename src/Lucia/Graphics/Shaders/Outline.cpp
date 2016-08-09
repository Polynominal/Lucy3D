#include "Lucia\Graphics\Shaders\Outline.h"
using namespace Lucia;
using namespace Graphics;
typedef Graphics::Shaders::Outline Outline;
namespace Lucia {
    namespace Graphics
    {
        std::unique_ptr<Shader> Graphics::_Shaders::Outline;
    }
}
Outline::Outline()
{
    if (Graphics::_Shaders::Outline.get() == nullptr)
    {
        string Vertex =
        #ifndef LUCIA_USE_GLES2
        "#version 330 core \n"
        "layout (location = 0) in vec2 vertex;\n"
        "layout (location = 1) in vec2 uv;\n"
        "out vec2 TexCoords;\n"
        #else
        "#version 100 \n"
        "attribute vec2 vertex;\n"
        "attribute vec2 uv;\n"
        "varying vec2 TexCoords;\n"
        #endif
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(vertex.x, vertex.y, 0.0f, 1.0f);\n"
        "   TexCoords = uv;\n"
        "}; \n";

        string Fragment =

        #ifndef LUCIA_USE_GLES2
        "#version 330 core \n"
        "in vec2 TexCoords;\n"
        "out vec4 color;\n"
        #else
        "#version 100 \n"
        "precision highp float;"
        "varying vec2 TexCoords;\n"
        "vec4 color\n;"
        "vec4 texture(sampler2D s,vec2 pos)"
        "{return texture2D(s,pos);};"
        #endif
        "uniform sampler2D Texture;\n"
        "uniform vec2 size;\n"
        "uniform vec4 OColor;\n"
        "void main()\n"
        "{\n"
            "vec2 offsets[9] = vec2[9]("
             "   vec2(-size.x, size.y),"  // top-left
             "   vec2(0.0f,    size.y),"  // top-center
             "   vec2(size.x,  size.y),"  // top-right
             "   vec2(-size.x, 0.0f), "   // center-left
             "   vec2(0.0f,    0.0f), "   // center-center
             "   vec2(size.x,  0.0f),"    // center-right
             "   vec2(-size.x, -size.y)," // bottom-left
             "   vec2(0.0f,    -size.y)," // bottom-center
             "   vec2(size.x,  -size.y)"  // bottom-right
            ");"
            "float r = 1.0f;"
           " float kernel[9] = float[9]("
             "   r, r, r,"
             "   r,  -r*8.0f, r,"
             "   r, r, r"
            ");"

            "vec4 sampleTex[9];"
            "float epsilon = 0.001f;"
            "for(int i = 0; i < 9; i++)"
            "{"
            "    vec2 pos = vec2(TexCoords.st + offsets[i]);"
            "    vec4 c = vec4(texture(Texture,pos));"
            "    if (TexCoords.y > 1.0f - size.y){c = vec4(0.0f,0.0f,0.0f,0.0f);};"
            "    sampleTex[i] = c;"
           " }"
            "vec4 col = vec4(0.0);"
            "for(int i = 0; i < 9; i++)"
            "   col += sampleTex[i] * kernel[i];"

            "color = col;"
            "color.a = 0.0f;"
            "if((color.r + color.g + color.b) > 0.1f){gl_FragColor = OColor;};"
        "};\n";
        Graphics::_Shaders::Outline.reset(new Shader());
        Graphics::_Shaders::Outline->build(Vertex,Fragment, "Outline");
    }
    //ctor
}

void Outline::draw(float thickness,float r,float g,float b,float a,Graphics::Canvas* c)
{

    Graphics::Canvas *canvas=nullptr;
    if (c == nullptr){canvas = Canvas.get();}else{canvas = c;};

    GLint last;
    glGetIntegerv(GL_CURRENT_PROGRAM,&last);

    GLuint id = Graphics::_Shaders::Outline->programID;
    glUseProgram(id);

    glUniform2f(glGetUniformLocation(Graphics::_Shaders::Outline->programID, "size"),(1.0f/canvas->getWidth()),(1.0f/canvas->getHeight()));
    glUniform4f(glGetUniformLocation(Graphics::_Shaders::Outline->programID, "OColor"),r,g,b,a);

    canvas->renderQuad();

    glUseProgram(last);
}
Outline::~Outline()
{
    remove();
    glDeleteProgram(Graphics::_Shaders::Outline->programID);
    //dtor
}
