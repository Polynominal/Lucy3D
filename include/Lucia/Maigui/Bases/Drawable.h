#ifndef MIKUS_LUCIA_MAIGUI_BASES_DRAWABLE_H
#define MIKUS_LUCIA_MAIGUI_BASES_DRAWABLE_H
#include <Lucia/Maigui/Skin.h>
#include <Lucia/Graphics/Bases/Color.h>
#include <Lucia/Maths/SpriteSheet.h>
#include <Lucia/Graphics/Image.h>
#include <memory>
#include <vector>

namespace Lucia {
namespace Maigui
{
    namespace Base
    {
        class Drawable
        {
            protected:
                std::shared_ptr<Graphics::Image> image;
                std::shared_ptr<Maths::Sprite> sprite;
            public:
                class State: public Graphics::Base::Color
                {
                    public:
                        State(int Enumref){enumref = Enumref;};
                        virtual ~State(){};
                        void setActive(bool cond){mode = (int)cond;};
                        void useName(bool cond){mode =2;};
                        void draw(shared_ptr<Skin> skin,Graphics::Image* img,Maths::Sprite* sprite);
                        int mode=0;
                        string name="";

                        std::function<void(string)> PreDraw = [](string s){};
                        std::function<void(string)> PostDraw = [](string s){};

                        void setDrawFunction(std::function<void(string)> fn){Draw = fn;drawOverride=true;};
                        void useInbuiltDraw(){drawOverride = false;};

                    private:
                        int enumref = 0;
                        bool drawOverride = false;
                        std::function<void(string)> Draw = [](string s){};

                };
                enum DrawState
                {
                    BACKGROUND=0,
                    FOREGROUND=1,
                    BASE=2,
                    FOCUS=3,
                    INACTIVE=4
                };
                void resetDrawStates(){
                    DrawStates.clear();
                    for (auto i=0; i < 5; i++)
                    {
                        DrawStates.push_back(std::make_shared<State>(i));
                    }
                };
                virtual void drawFocus(Graphics::Image* i,Maths::Sprite* s){parseDrawState(FOCUS,i,s);};
                virtual void drawInactive(Graphics::Image* i,Maths::Sprite* s){parseDrawState(INACTIVE,i,s);};

                virtual void drawBase(Graphics::Image* i,Maths::Sprite* s){parseDrawState(BASE,i,s);};
                virtual void drawBackground(Graphics::Image* i,Maths::Sprite* s){parseDrawState(BACKGROUND,i,s);};
                virtual void drawForeground(Graphics::Image* i,Maths::Sprite* s){parseDrawState(FOREGROUND,i,s);};

                virtual void parseDrawState(DrawState enu,Graphics::Image* img,Maths::Sprite* s)
                {
                    DrawStates[enu]->draw(skin,img,s);
                }
                virtual void draw(Matrix<4> Matrix,bool hasFocus=false,bool inactive=false);
                virtual bool areItemsDrawn(){return DrawItems;};
                
                //SET
                virtual void setSprite(float x,float y,float w,float h)
                {
                    if (image.get() == nullptr)
                    {
                        auto img = skin->getData();
                        Drawable::setSprite(x,y,w,h,img->getWidth(),img->getHeight());
                        
                    }else
                    {
                        Drawable::setSprite(x,y,w,h,image->getWidth(),image->getHeight());
                    };
                };
                virtual void setSprite(std::shared_ptr<Graphics::Image>i,float x,float y,float w,float h)
                {
                    setImage(i);
                    setSprite(x,y,w,h);
                };
                virtual void setSprite(float x,float y,float w,float h,float nw,float nh){setSprite(std::make_shared<Maths::Sprite>(x,y,w,h,nw,nh));};
                virtual void setSprite(std::shared_ptr<Maths::Sprite> s){sprite = s;};
                virtual void setImage(std::shared_ptr<Graphics::Image>i){image = i;};
                virtual void setImage(){image = nullptr;};
                
                virtual void setDrawItems(bool drawThem=true){DrawItems = drawThem;};
                virtual void setDrawState(DrawState state,float r=255,float g=255,float b=255,float a=255);
                virtual void setDrawState(DrawState state,string Name,float r=255,float g=255,float b=255,float a=255);
                virtual void setDrawState(DrawState state,bool cond)
                {
                    auto s = DrawStates[state];
                    s->mode = (int)cond;
                };
                virtual void setSkin(shared_ptr<Skin> s){skin = s;}
                //GET
                virtual shared_ptr<State> getDrawState(DrawState state)
                {
                    return DrawStates[state];
                };
                virtual std::shared_ptr<Graphics::Image> getImage(){if (image.get() ==nullptr){return skin->getData();}else{return image;}};
                virtual shared_ptr<Skin> getSkin()
                {
                    return skin;
                };
                virtual void setStateName(string name,DrawState state)
                {
                    DrawStates[state]->name = name;
                };
                virtual void setName(string name)
                {
                   setStateName(name,BASE);
                };
                bool DrawItems=true;
                bool drawStatesInitalized =false;
                shared_ptr<Skin> skin;
                std::vector<shared_ptr<State>> DrawStates;
        };
    }
}}

#endif // MIKUS_MAIGUI_BASES_DRAWABLE_H
