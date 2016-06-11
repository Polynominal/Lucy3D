#include "Graphics\Buffers\Spritesheet.h"
using namespace Graphics::Scene;
using namespace std;

namespace Graphics
{
    namespace _Shaders
    {
        std::shared_ptr<Graphics::Shader> VaryingUV=nullptr;
    }
}
void createShader()
{
    if (Graphics::_Shaders::VaryingUV.get() == nullptr)
    {
        Graphics::_Shaders::VaryingUV = std::make_shared<Graphics::Shader>();
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
            "   TexCord = vec2(data.x,-data.y) + (texCord * (data.z,data.w)) + vec2(0.0f,1.0f - data.w);"
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
void Spritesheet::useShader(glm::mat4* view,glm::mat4* projection)
{
    auto vars = getShaderVars();
    vars->use();
    vars->sendMatrix("view",4,4,false,glm::value_ptr(*view));
    vars->sendMatrix("projection",4,4,false,glm::value_ptr(*projection));
}
void Spritesheet::load(std::string file_path)
{
    image = std::make_shared<Image>(file_path);
    image->generate();
}
std::shared_ptr<Spritesheet::Block> Spritesheet::create(float ax,float ay,float w,float h)
{
    float width =Utils::Logic::lessThanZeroIsDefault(w,defaultWidth);
    float height =Utils::Logic::lessThanZeroIsDefault(h,defaultHeight);
    return std::make_shared<Block>(shared_from_this(),ax,ay,width,height);
}
void Spritesheet::preDraw()
{
    image->bind();
}
Spritesheet::Animation Spritesheet::newAnimation(unsigned int startFrame,unsigned int finalFrame,float speed,float ax,float ay,float w,float h)
{
    std::vector<Block> Frames;
    float width =Utils::Logic::lessThanZeroIsDefault(w,defaultWidth);
    float height =Utils::Logic::lessThanZeroIsDefault(h,defaultHeight);
    float imagew = image->getWidth();
    float imageh = image->getHeight();
    float rowsize = imagew/width;
    if (finalFrame == 0)
    {
        finalFrame = imagew/width * imageh/height;
    };
    for (unsigned int i=startFrame; i < finalFrame; i++)
    {
        unsigned int row = floor((1-i)/rowsize);
        unsigned int column = std::fmod((i-1),rowsize);
        Frames.push_back(Block(shared_from_this(),(column*width) + ax, (row*height) + ay,width,height));
    }
    return Animation(shared_from_this(),Frames,1.0f/speed);
};
Spritesheet::~Spritesheet()
{
    //dtor
}
// BLOCK
Spritesheet::Block::Block(std::shared_ptr<Spritesheet> p,float ax,float ay,float aw,float ah)
{
    parent = p;
    setData(ax,ay,aw,ah);
    setShaderVars(Graphics::_Shaders::VaryingUV->getVars());
}
void Spritesheet::Block::setData(float ax,float ay,float aw,float ah)
{
    float imgw = parent->image->getWidth();
    float imgh = parent->image->getHeight();
    x = ax/imgw;
    y = ay/imgh;
    w = aw/imgw;
    h = ah/imgh;
}
void Spritesheet::Block::render(glm::mat4 *model,DRAW mode)
{
    ShaderVars->send("color",1.0f,1.0f,1.0f,1.0f);
    ShaderVars->send("data",x,y,w,h);
    ShaderVars->sendMatrix("model",4,4,false,glm::value_ptr(*model));
    Graphics::Shared::Quad->draw(_Shaders::VaryingUV->getVars());
}
//
Spritesheet::Animation::Animation(std::shared_ptr<Spritesheet>p,std::vector<Block> fr,float fps)
{
    frames = fr;
    parent = p;
    FPS = fps;
};
void Spritesheet::Animation::setTween(std::shared_ptr<Utils::Tween<float>> tween)
{
    t = tween;
    t->setRate(1/FPS);
};
    //set
void Spritesheet::Animation::setSpeed(float fps)
{
    t->setRate(1/fps);
    FPS = fps;
};
void Spritesheet::Animation::play()
{
    paused = false;
    float i = frames.size()-1;
    t->addVar(&frame,i);
}
void Spritesheet::Animation::stop()
{
    t->stop();
}
void Spritesheet::Animation::update(double dt)
{
    t->update(dt);
}
void Spritesheet::Animation::render(glm::mat4 *model,DRAW mode)
{
    frames[(unsigned int)frame].render(model,mode);
}

