#include "Lucia/Graphics/Objects/Billboard.h"
using namespace Lucia;
using namespace Graphics;

using namespace Object;
using namespace Scene;

std::unique_ptr<Buff> Core;
std::unique_ptr<Shader> BillboardShader;

Billboard::Billboard()
{
    if (BillboardShader.get() == nullptr)
    {
        string Vertex =
        "#version 100\n"
        "attribute vec3 vertex;"
        "attribute vec2 texCord;"
        "varying vec2 TexCord;"
        "varying vec4 color;"
        "varying float enable_texture;"
        "uniform float enableTexture;"
        "uniform bool x;"
        "uniform bool y;"
        "uniform bool z;"
        "uniform bool Static;"
        "uniform vec4 Color;"
        "uniform mat4 view;"
        "uniform mat4 projection;"
        "uniform vec3 Scale;"
        "uniform vec3 Position;"
        "void main()"
        "{"
        "   if (Static)"
        "   { "
        "       gl_Position = projection*view* vec4(Position, 1.0f);"
        "       gl_Position /= gl_Position.w; "
        "       gl_Position.xy += vertex.xy * Scale.xy; "
        "    }else{"
        "    vec3 Up = vec3(0.0f,0.0f,0.0f);"
        "    vec3 Right = vec3(0.0f,0.0f,0.0f);"
        "    if(x){"
        "        Up.x = view[0][1];"
        "           Right.x = view[0][0];"
        "       };"
        "       if(y){"
        "           Up.y =  view[1][1];"
        "           Right.y = view[1][0];"
        "       };"
        "       if(z){"
        "           Up.z =  view[2][1];"
        "        Right.z = view[2][0];"
        "       };"
        "    vec3 position = Position"
        "       + Right*vertex.x*Scale.x"
        "       + Up*vertex.y*Scale.y;"
        "       gl_Position = projection*view*vec4(position,1.0f);"
        "   };"
        "       enable_texture = enableTexture;"
        "       color = Color;"
        "       TexCord = texCord;"
        "}";
        string Fragment =
        "#version 100\n"
        "precision highp float;"
        "varying float enable_texture;"
        "varying vec2 TexCord;"
        "varying vec4 color;"
        "uniform sampler2D Texture;"
        "void main()"
        "{"
        "   if (enable_texture > 0.5f){gl_FragColor = color*texture2D(Texture,TexCord);}"
        "   else{gl_FragColor = color;};"
        "}";
        BillboardShader.reset(new Graphics::Shader());
        BillboardShader->build(Vertex,Fragment,"Billboard internal");
        BillboardShader->addVar("vertex",0,3);
        BillboardShader->addVar("texCord",3,5);
    }
    if (Core.get() == nullptr)
    {
        float data[] = {
            -0.5f, -0.5f, 0.0,  0.0f, 0.0f,
            0.5f, -0.5f, 0.0f,  1.0f, 0.0f,
            0.5f,  0.5f, 0.0f,  1.0f, 1.0f,
            0.5f,  0.5f, 0.0f,  1.0f, 1.0f,
            -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f
        };
        Core.reset( new Buff(BillboardShader->getVars()) );
        Core->setData(data,6);
    }
    setShaderVars(BillboardShader->getVars());
}
void Billboard::render(Graphics::Scene::Container* c,DRAW dmode)
{
    auto vars = getShaderVars();
    if (image.get() == nullptr)
    {
        vars->send("enableTexture",0.0f);
    }else{
        vars->send("enableTexture",1.0f);
        image->setName("Texture");
        image->bind();
    };
    
    if (std::get<0>(axis)){vars->send("x",true);};
    if (std::get<1>(axis)){vars->send("y",true);};
    if (std::get<2>(axis)){vars->send("z",true);};
    
    auto Scale = c->getScale();
    vars->send("Scale",Scale.x,Scale.y,Scale.z);
    
    Scale = c->getPosition();
    vars->send("Position",Scale.x,Scale.y,Scale.z);
    vars->send("Static",(mode == MODE::STATIC));
    c->applyColor(vars);
    
    auto target = getMechanicalTarget(dmode);
    Core->draw(target);
}
void Billboard::setStatic(bool t)
{
    if (t){mode = MODE::STATIC;}else{mode = MODE::DYNAMIC;};
}