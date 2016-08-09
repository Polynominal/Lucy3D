#include "Lucia\Graphics\Buffers\Spritesheet.h"
using namespace Lucia;
using namespace Graphics::Scene;
using namespace Graphics::Buffer;
using namespace std;
namespace Lucia {
namespace Graphics
{
    namespace _Shaders
    {
        std::shared_ptr<Graphics::Shader> VaryingUV=nullptr;
    }
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
    vars->sendMatrix("view",4,4,false,view->unpack());
    vars->sendMatrix("projection",4,4,false,projection->unpack());
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
std::shared_ptr<Spritesheet::Animation> Spritesheet::newAnimation(unsigned int startFrame,unsigned int finalFrame,float speed,float ax,float ay,float w,float h)
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
    for (unsigned int i=startFrame; i < finalFrame+1; i++)
    {
        unsigned int row = floor(i/rowsize);
        unsigned int column = std::fmod(i,rowsize);
        Frames.push_back(Block(shared_from_this(),(column*width) + ax, (row*height) + ay,width,height));
    }
    std::shared_ptr<Utils::Tween<float>> t = std::make_shared<Utils::Tween<float>>();
    auto a = std::make_shared<Animation>(shared_from_this(),Frames,speed);
    a->setTween(t);
    a->setShaderVars(getShaderVars());
    return a;
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
void Spritesheet::Block::render(Maths::Matrix<4> *model,DRAW mode)
{
    ShaderVars->send("color",1.0f,1.0f,1.0f,1.0f);
    ShaderVars->send("data",x,y,w,h);
    ShaderVars->sendMatrix("model",4,4,false,model->unpack());

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    Graphics::Shared::Quad->draw(_Shaders::VaryingUV->getVars());
}
//
Spritesheet::Animation::Animation(std::shared_ptr<Spritesheet>p,std::vector<Block> fr,float fps)
{
    frames = fr;
    parent = p;
    FPS = fps;
};
std::shared_ptr<Spritesheet::Animation> Spritesheet::Animation::clone()
{
    auto nAnim = std::make_shared<Spritesheet::Animation>(parent,frames,FPS);
    auto tween =  std::make_shared<Utils::Tween<float>>();
    nAnim->setTween(tween);
    return nAnim;
}
void Spritesheet::Animation::setTween(std::shared_ptr<Utils::Tween<float>> tween)
{
    t = tween;
    //setSpeed(FPS);
    setMode(mode);
};
void Spritesheet::Animation::setMode(Mode m)
{
    mode = m;
    switch (mode)
    {
    case Loop:
        t->setMode(Utils::TweenMode::Loop);
        break;
    case Once:
        t->setMode(Utils::TweenMode::Once);
        break;
    case Bounce:
        t->setMode(Utils::TweenMode::Bounce);
        break;
    case Reverse:
        t->stop();
        frame = frames.size()-1;
        t->addVar(&frame,0,(1.0f/FPS));

        break;
    }
}
void Spritesheet::Animation::setMode(std::string m)
{
    if (m == "Loop"){mode = Spritesheet::Animation::Loop;}
    else if (m == "Bounce"){mode = Spritesheet::Animation::Bounce;}
    else if (m == "Reverse"){mode = Spritesheet::Animation::Reverse;}
    else{mode = Spritesheet::Animation::Once;};
    setMode(mode);
}
    //set
void Spritesheet::Animation::setSpeed(float fps)
{
    float speed = (frames.size()-1)/(fps);
    t->setRate(speed);
    FPS = fps;
};
void Spritesheet::Animation::play()
{
    if (paused)
    {
        t->unpause();
    }else
    {
        float i = frames.size()-1;
        t->addVar(&frame,i,1.0f/FPS);
        setMode(mode);
    }
    paused = false;

}
void Spritesheet::Animation::stop()
{
    t->stop();
}
void Spritesheet::Animation::update(double dt)
{
    t->update(dt);
    if (mode == Mode::Reverse)
    {
        if (floor(frame) == 0){OnFinish();};
    }else
    {
        if (floor(frame) == frames.size()-1){OnFinish();};
    }
};
void Spritesheet::Animation::render(Maths::Matrix<4> *model,DRAW mode)
{
    auto f = floor(frame);
    if (frame > (frames.size()-2) + 0.5f){f = frames.size()-1;}
    frames[f].render(model,mode);
};

