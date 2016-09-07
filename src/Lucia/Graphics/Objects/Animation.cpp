#include "Lucia/Graphics/Objects/Animation.h"
using namespace Lucia;
using namespace Graphics;
using namespace Object;
using namespace Buffer;

Animation::Animation(std::shared_ptr<Spritesheet>p,std::vector<Graphics::Object::Sprite> fr,float fps)
{
    frames = fr;
    parent = p;
    FPS = fps;
};
std::shared_ptr<Animation> Animation::clone()
{
    auto nAnim = std::make_shared<Animation>(parent,frames,FPS);
    auto tween =  std::make_shared<Utils::Tween<float>>();
    nAnim->setTween(tween);
    return nAnim;
}
void Animation::setTween(std::shared_ptr<Utils::Tween<float>> tween)
{
    t = tween;
    //setSpeed(FPS);
    setMode(mode);
};
void Animation::setMode(Mode m)
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
void Animation::setMode(std::string m)
{
    if (m == "Loop"){mode = Animation::Loop;}
    else if (m == "Bounce"){mode = Animation::Bounce;}
    else if (m == "Reverse"){mode = Animation::Reverse;}
    else{mode = Animation::Once;};
    setMode(mode);
}
    //set
void Animation::setSpeed(float fps)
{
    float speed = (frames.size()-1)/(fps);
    t->setRate(speed);
    FPS = fps;
};
void Animation::play()
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
void Animation::stop()
{
    t->stop();
}
void Animation::update(double dt)
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
void Animation::render(Maths::Matrix<4> *model,DRAW mode)
{
    auto f = floor(frame);
    if (frame > (frames.size()-2) + 0.5f){f = frames.size()-1;}
    frames[f].render(model,mode);
};