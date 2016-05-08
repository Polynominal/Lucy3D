#include "Graphics/Shaders/Blur.h"
typedef Graphics::Shaders::Blur Blur;
namespace Graphics
{
    namespace _Shaders
    {
        std::unique_ptr<Shader> Kawase;
        std::unique_ptr<Shader> Gaus5x5;
        std::unique_ptr<Shader> Gaus;
        string BlurStandardVertex =
        #if defined USE_GLES3 || defined USE_GLES2
        "#version 100 \n"
        "attribute vec2 vertex;\n"
        "attribute vec2 uv;\n"
        "varying vec2 TexCoords;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(vertex.x, vertex.y, 0.0f, 1.0f);\n"
        "   TexCoords = uv;\n"
        "}; \n";
        #else
        "#version 330 core \n"
        "layout (location = 0) in vec2 vertex;\n"
        "layout (location = 1) in vec2 uv;\n"
        "out vec2 TexCoords;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(vertex.x, vertex.y, 0.0f, 1.0f);\n"
        "   TexCoords = uv;\n"
        "}; \n";
        #endif // USE_GLES2
    }
}
Blur::Blur()
{
    if (Graphics::_Shaders::Kawase.get() == nullptr)
    {
        string Fragment =

        #if !(defined USE_GLES3 || defined USE_GLES2)
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
        "uniform int Passes;\n"
        "uniform float degree;"
        "vec4 KawaseFilter(sampler2D t, vec2 texCord, vec2 pixelSize, int i)\n"
        "{\n"
        "    vec2 tSample;\n"
        "    vec2 halfp = pixelSize/2.0f;\n"
        "    vec2 uv = (pixelSize.xy * float(i) ) + halfp.xy;\n"
        "    vec4 cout;\n"
            // [1][2]
            // -[0]-
            // [3][4]
            // 1;
        "    tSample.x = clamp(texCord.x - uv.x,0.0f,0.998f);\n"
        "    tSample.y = clamp(texCord.y + uv.y,0.0f,0.998f);\n"
        "    cout = texture(t,tSample);\n"
            // 2
        "    tSample.x = clamp(texCord.x + uv.x,0.0f,0.998f);\n"
        "    tSample.y = clamp(texCord.y + uv.y,0.0f,0.998f);\n"
        "    cout += texture(t,tSample);\n"
            // 4
        "    tSample.x = clamp(texCord.x + uv.x,0.0f,0.998f);\n"
        "    tSample.y = clamp(texCord.y - uv.y,0.0f,0.998f);\n"
        "    cout += texture(t,tSample);\n"
            // 3
        "   tSample.x = clamp(texCord.x - uv.x,0.0f,0.998f);\n"
        "   tSample.y = clamp(texCord.y + uv.y,0.0f,0.998f);\n"
        "   cout += texture(t,tSample);\n"
        "   cout *= degree;"
        "   return cout;\n"
        "};\n"
        "void main()\n"
        "{\n"
        "for (int i =0;i < Passes; i++)"
        "{"
        "   color += KawaseFilter(Texture,TexCoords,size,i);"
        "}"
        "color = color * (1.0f/float(Passes));"
        #if defined USE_GLES3 || defined USE_GLES2
        "gl_FragColor = color;"
        #endif // USE_GLES2
        "};\n";
        std::cout << "Building Kawase Shader" << std::endl;
        Graphics::_Shaders::Kawase.reset(new Shader());
        Graphics::_Shaders::Kawase->build(Graphics::_Shaders::BlurStandardVertex,Fragment,"Kawase");
    }
    if (Graphics::_Shaders::Gaus5x5.get() == nullptr)
    {
        string Fragment =
        #ifndef USE_GLES2
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
        "uniform vec2 size;"
        "void main()\n"
        "{\n"
            "vec2 offsets[25] = vec2[25]("
             "   vec2(-size.x*2.0f, size.y*2.0f),"  // top-left
             "   vec2(-size.x, size.y*2.0f),"  // top-left
             "   vec2(0.0f,    size.y*2.0f),"  // top-center
             "   vec2(size.x,  size.y*2.0f),"  // top-right
             "   vec2(size.x*2.0f,  size.y*2.0f),"  // top-right

             "   vec2(-size.x*2.0f, size.y),"  // top-left
             "   vec2(-size.x, size.y),"  // top-left
             "   vec2(0.0f,    size.y),"  // top-center
             "   vec2(size.x,  size.y),"  // top-right
             "   vec2(size.x*2.0f,  size.y),"  // top-right

             "   vec2(-size.x*2.0f, 0.0f),"  // top-left
             "   vec2(-size.x, 0.0f),"  // top-left
             "   vec2(0.0f,    0.0f),"  // top-center
             "   vec2(size.x,  0.0f),"  // top-right
             "   vec2(size.x*2.0f,  0.0f),"  // top-right

             "   vec2(-size.x*2.0f, -size.y),"  // top-left
             "   vec2(-size.x, -size.y),"  // top-left
             "   vec2(0.0f,    -size.y),"  // top-center
             "   vec2(size.x,  -size.y),"  // top-right
             "   vec2(size.x*2.0f,  -size.y),"  // top-right

             "   vec2(-size.x*2.0f, -size.y*2.0f),"  // top-left
             "   vec2(-size.x, -size.y*2.0f),"  // top-left
             "   vec2(0.0f,    -size.y*2.0f),"  // top-center
             "   vec2(size.x,  -size.y*2.0f),"  // top-right
             "   vec2(size.x*2.0f,  -size.y*2.0f)"  // top-right

            ");"
            "float r = 1.0f;"
            "float kernel[25] = float[25]("
             "   0.00390625f, 0.015625f, 0.0234375f, 0.015625f, 0.00390625f,"
             "   0.00390625f, 0.0625f, 0.09375f,0.0625f,0.00390625f,"
             "   0.0234375f, 0.09375f, 0.140625f,0.09375f,0.0234375f,"
             "   0.015625f, 0.0625f, 0.09375f,0.0625f,0.015625f,"
             "   0.00390625f, 0.015625f, 0.0234375f, 0.015625f, 0.00390625f"
            ");"

            "vec4 sampleTex[25];"
            "float epsilon = 0.001f;"
            "for(int i = 0; i < 25; i++)"
            "{"
            "    vec2 pos = vec2(TexCoords.st + offsets[i]);"
            "    vec4 c = vec4(texture(Texture,pos));"
            "    sampleTex[i] = c;"
           " }"
            "vec4 col = vec4(0.0);"
            "for(int i = 0; i < 25; i++)"
            "   col += sampleTex[i] * kernel[i];"

            "color = col;"
            #ifdef USE_GLES2
            "gl_FragColor = color;"
            #endif // USE_GLES2
        "};\n";
        std::cout << "Building Gaus5x5 Shader" << std::endl;
        Graphics::_Shaders::Gaus5x5.reset(new Shader());
        Graphics::_Shaders::Gaus5x5->build(Graphics::_Shaders::BlurStandardVertex,Fragment, "Gaus5x5");
    }
    if (Graphics::_Shaders::Gaus.get() == nullptr)
    {
        string Fragment =
        #if !(defined USE_GLES3 || defined USE_GLES2)
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
        "uniform vec2 size;"
        "void main()\n"
        "{\n"
            "vec2 offsets[9] = vec2[9]("
             "   vec2(-size.x,      size.y),"
             "   vec2(0.0f,         size.y),"  // top-center
             "   vec2(size.x,       size.y),"

             "   vec2(-size.x,      0.0f),"
             "   vec2(0.0f,         0.0f),"  // top-center
             "   vec2(size.x,       0.0f),"

             "   vec2(-size.x,      -size.y),"
             "   vec2(0.0f,         -size.y),"  // top-center
             "   vec2(size.x,       -size.y)"
            ");"
            "float r = 16.0f;"
            "float kernel[9] = float[9]("
            "   1.0f/r,2.0f/r,1.0f/r,"
            "   2.0f/r,4.0f/r,2.0f/r,"
            "   1.0f/r,2.0f/r,1.0f/r"
            ");"

            "vec4 sampleTex[9];"
            "float epsilon = 0.001f;"
            "for(int i = 0; i < 9; i++)"
            "{"
            "    vec2 pos = vec2(TexCoords.st + offsets[i]);"
            "    vec4 c = vec4(texture(Texture,pos));"
            "    sampleTex[i] = c;"
           " }"
            "vec4 col = vec4(0.0);"
            "for(int i = 0; i < 9; i++)"
            "   col += sampleTex[i] * kernel[i];"

            "color = col;"
            #if defined USE_GLES3 || defined USE_GLES2
            "gl_FragColor = color;"
            #endif // USE_GLES2
        "};\n";
        std::cout << "Building Gaus Shader" << std::endl;
        Graphics::_Shaders::Gaus.reset(new Shader());
        Graphics::_Shaders::Gaus->build(Graphics::_Shaders::BlurStandardVertex,Fragment,"Gaus");
    }
    //ctor
}
void Blur::draw(Graphics::Canvas* c,int Passes,float degree)
{
    Graphics::Canvas *canvas=nullptr;
    if (c == nullptr){canvas = Canvas.get();}else{canvas = c;};

    GLint last;
    glGetIntegerv(GL_CURRENT_PROGRAM,&last);
    //mechanical value of a single pixel within 0 to 1 space.
    float p[2] = {1.0f/canvas->getWidth(),1.0f/canvas->getHeight()};

    switch(mode)
    {
    case Kawase:
        glUseProgram(Graphics::_Shaders::Kawase->programID);
        glUniform1i(glGetUniformLocation(Graphics::_Shaders::Kawase->programID, "Passes"),Passes);
        glUniform1f(glGetUniformLocation(Graphics::_Shaders::Kawase->programID, "degree"),degree);
        glUniform2f(glGetUniformLocation(Graphics::_Shaders::Kawase->programID, "size"),p[0],p[1]);        break;
    case Gaus5x5:
        glUseProgram(Graphics::_Shaders::Gaus5x5->programID);
        glUniform2f(glGetUniformLocation(Graphics::_Shaders::Gaus5x5->programID, "size"),p[0],p[1]);
        break;
    case Gaus:
        glUseProgram(Graphics::_Shaders::Gaus->programID);
        glUniform2f(glGetUniformLocation(Graphics::_Shaders::Gaus->programID, "size"),p[0],p[1]);
        break;
    }

    canvas->renderQuad();
    glUseProgram(last);
}
Blur::~Blur()
{
    remove();
    //dtor
}
