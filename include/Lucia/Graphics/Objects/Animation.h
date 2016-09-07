#ifndef MIKUS_LUCIA_GRAPHICS_OBJECT_ANIMATION_H
#define MIKUS_LUCIA_GRAPHICS_OBJECT_ANIMATION_H
#include <vector>
#include <Lucia/Graphics/Scene/Object.h>
#include <Lucia/Graphics/Buffers/Spritesheet.h>
#include <Lucia/Graphics/Objects/Sprite.h>

namespace Lucia{
namespace Graphics{
namespace Object{

class Animation: public Graphics::Scene::Object
{
    public:
        enum Mode
        {
            Loop,
            Once,
            Bounce,
            Reverse
        };
        Animation(std::shared_ptr<Buffer::Spritesheet> Parent,std::vector<Graphics::Object::Sprite> Frames,float frames_per_second);
        virtual ~Animation(){};
        //set
        void setTween(std::shared_ptr<Utils::Tween<float>> t);
        void setSpeed(float fps);
        void setMode(Mode f);
        void setMode(std::string mode);
        //
        void play();
        void stop();
        void pause();
        std::shared_ptr<Animation> clone();
        void update(double dt);
        virtual void useShader(Maths::Matrix<4>* view,Maths::Matrix<4>* projection){parent->useShader(view,projection);};
        virtual void preDraw(){parent->preDraw();};
        virtual void render(Maths::Matrix<4>* model, DRAW mode);

    private:
        bool paused = false;
        float frame = 0;
        float FPS = 30;
        std::vector<Graphics::Object::Sprite> frames;
        std::function<void()> OnFinish = [](){};
        Mode mode=Mode::Loop;
        std::shared_ptr<Utils::Tween<float>> t;
        std::shared_ptr<Buffer::Spritesheet> parent;
};
}}};
#endif