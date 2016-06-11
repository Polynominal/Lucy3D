#ifndef MIKUS_GRAPHICS_TOOLS_SPRITESHEET_H
#define MIKUS_GRAPHICS_TOOLS_SPRITESHEET_H

#include <memory>

#include <Graphics/Image.h>
#include <Graphics/Scene/Object.h>
#include <Graphics/Shaders.h>
#include <Graphics/Bases/ShaderHolder.h>

#include <utilities/tween.h>
#include <utilities/logistics.h>
#include <utilities/OpenGL.h>
#include <Graphics/Primitives/Quad.h>

namespace Graphics
{
    namespace Scene
    {
        class Spritesheet: public std::enable_shared_from_this<Spritesheet>, public Graphics::Base::ShaderHolder
        {
            public:
                class Block: public Graphics::Scene::Object
                {
                    public:
                        Block(std::shared_ptr<Spritesheet> parent,float x,float y,float w,float h);
                        //set
                        void setData(float x,float y,float w,float h);
                        //get
                        float getWidth(){return w;};
                        float getHeight(){return h;};
                        std::pair<float,float> getSpritesheetPosition(){return std::pair<float,float>(x,y);};
                        std::shared_ptr<Spritesheet> getParent(){return parent;};
                        //
                        virtual void useShader(glm::mat4* view,glm::mat4* projection){parent->useShader(view,projection);};
                        virtual void preDraw(){parent->preDraw();};
                        virtual void render(glm::mat4* model,DRAW mode);

                        virtual ~Block(){};
                    private:
                        std::shared_ptr<Spritesheet> parent;
                        float topx=0;
                        float topy=0;
                        float bottomx=0;
                        float bottomy=0;

                        float x=0;
                        float y=0;
                        float w=1;
                        float h=1;
                };
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
                        Animation(std::shared_ptr<Spritesheet> Parent,std::vector<Block> Frames,float frames_per_second);
                        virtual ~Animation(){};
                        //set
                        void setTween(std::shared_ptr<Utils::Tween<float>> t);
                        void setSpeed(float fps);
                        void setMode(Mode f){mode =f;};
                        //
                        void play();
                        void stop();
                        void pause();
                        Animation clone();
                        void update(double dt);
                        virtual void useShader(glm::mat4* view,glm::mat4* projection){parent->useShader(view,projection);};
                        virtual void preDraw(){parent->preDraw();};
                        virtual void render(glm::mat4* model, DRAW mode);

                    private:
                        bool paused = false;
                        float frame = 0;
                        float FPS = 30;
                        std::vector<Block> frames;
                        std::function<void()> OnFinish = [](){};
                        Mode mode=Mode::Loop;
                        std::shared_ptr<Utils::Tween<float>> t;
                        std::shared_ptr<Spritesheet> parent;
                };

                Spritesheet();
                //set
                void setFrameDimensions(float width,float height){defaultWidth = width;defaultHeight = height;};
                //
                void load(std::string file_path);
                void useShader(glm::mat4* view,glm::mat4* projection);
                void preDraw();
                std::shared_ptr<Block> create(float x,float y,float width=-1,float height=-1);
                Animation newAnimation(unsigned int startFrame,unsigned int finalFrame=0,float speed=1,float ax=0,float ay=0,float w=-1,float h=-1);
                //get
                std::shared_ptr<Image> getImage(){return image;};
                //set

                //
                virtual ~Spritesheet();


            private:
                float defaultWidth = 0;
                float defaultHeight = 0;
                std::shared_ptr<Image> image;
        };
    }
}

#endif // MIKUS_GRAPHICS_TOOLS_SPRITESHEET_H
