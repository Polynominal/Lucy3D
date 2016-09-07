#include "Lucia\Graphics\Buffers\Spritesheet.h"
#include "Lucia\Graphics\Objects\Sprite.h"
#include "Lucia\Graphics\Objects\Animation.h"

using namespace Lucia;
using namespace Graphics::Scene;
using namespace Graphics::Buffer;
using namespace std;
namespace Lucia {
namespace Graphics
{
    namespace _Shaders
    {
        std::unique_ptr<Graphics::Shader> VaryingUV;
    }
}
}
void createShader()
{
    if (Graphics::_Shaders::VaryingUV.get() == nullptr)
    {
        Graphics::_Shaders::VaryingUV.reset( new Graphics::Shader());
            string Vertex =
            "#version 100 \n"
            "attribute vec3 vertex;"
            "attribute vec2 texCord;"
            "varying vec2 TexCord;"
            "uniform mat4 model;"
            "uniform mat4 projection;"
            "uniform mat4 view;"
            "uniform vec4 data;"
            "void main()"
            "{"
            "   TexCord = data.xy + (texCord * data.zw);"
            "   gl_Position = projection * view * model * vec4(vertex, 1.0f);"
            "}";
            string Fragment =
            "#version 100 \n"
            "precision highp float;"
            "varying vec2 TexCord;"
            "uniform sampler2D Texture;"
            "uniform vec4 color;"
            "void main()"
            "{"
            "   gl_FragColor = color*texture2D(Texture,TexCord);"
            "}";
        Graphics::_Shaders::VaryingUV->build(Vertex,Fragment,"SpriteSheet[VaryingUV]");
        Graphics::_Shaders::VaryingUV->addVar("vertex",0,2);
        Graphics::_Shaders::VaryingUV->addVar("texCord",2,4);
        if (!Graphics::Shared::Quad->isDone())
        {
            Graphics::Shared::Quad->generate();
        }
    };
}
Spritesheet::Spritesheet()
{
    createShader();
    setShaderVars(Graphics::_Shaders::VaryingUV->getVars());
    //ctor
}
void Spritesheet::useShader(Maths::Matrix<4>* view,Maths::Matrix<4>* projection)
{
    auto vars = getShaderVars();
    vars->use();
    vars->sendMatrix("view",4,4,true,view->unpack());
    vars->sendMatrix("projection",4,4,false,projection->unpack());
}
void Spritesheet::load(std::string file_path)
{
    image = std::make_shared<Image>(file_path);
    image->generate();
}
std::shared_ptr<Graphics::Object::Sprite> Spritesheet::create(float ax,float ay,float w,float h)
{
    float width =Utils::Logic::lessThanZeroIsDefault(w,defaultWidth);
    float height =Utils::Logic::lessThanZeroIsDefault(h,defaultHeight);
    auto ptr = shared_from_this();
    auto sprite = std::make_shared<Graphics::Object::Sprite>();
    sprite->create(ptr,ax,ay,width,height);
    return sprite;
}
void Spritesheet::preDraw()
{
    image->bind();
}
std::shared_ptr<Graphics::Object::Animation> Spritesheet::newAnimation(unsigned int startFrame,unsigned int finalFrame,float speed,float ax,float ay,float w,float h)
{
    auto frames = Maths::createSprites(startFrame,finalFrame,ax,ay,w,h,defaultWidth,defaultHeight);
    std::vector<Graphics::Object::Sprite> Frames;
    for (auto v: frames)
    {
        Frames.push_back(Graphics::Object::Sprite(shared_from_this(),v.x,v.y,v.w,v.h));
    };
    std::shared_ptr<Utils::Tween<float>> t = std::make_shared<Utils::Tween<float>>();
    auto a = std::make_shared<Graphics::Object::Animation>(shared_from_this(),Frames,speed);
    a->setTween(t);
    a->setShaderVars(getShaderVars());
    return a;
};
Spritesheet::~Spritesheet()
{
    //dtor
}

