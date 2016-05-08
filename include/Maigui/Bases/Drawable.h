#ifndef MIKUS_MAIGUI_BASES_DRAWABLE_H
#define MIKUS_MAIGUI_BASES_DRAWABLE_H
#include <Maigui/Skin.h>
#include <Maigui/Bases/Color.h>
#include <memory>
#include <vector>

namespace Maigui
{
    namespace Base
    {
        class Drawable
        {
            public:
                class State: public Color
                {
                    public:
                        State(int Enumref){enumref = Enumref;};
                        virtual ~State(){};
                        void setActive(bool cond){mode = (int)cond;};
                        void useName(bool cond){mode =2;};
                        void draw(shared_ptr<Skin> skin);
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
                virtual void drawFocus(){parseDrawState(FOCUS);};
                virtual void drawInactive(){parseDrawState(INACTIVE);};

                virtual void drawBase(){parseDrawState(BASE);};
                virtual void drawBackground(){parseDrawState(BACKGROUND);};
                virtual void drawForeground(){parseDrawState(FOREGROUND);};

                virtual void parseDrawState(DrawState enu)
                {
                    DrawStates[enu]->draw(skin);
                }
                virtual void draw(Collider::Tools::Matrix<4> Matrix,bool hasFocus=false,bool inactive=false);
                virtual bool areItemsDrawn(){return DrawItems;};

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
                }
                virtual shared_ptr<Skin> getSkin()
                {
                    return skin;
                }
                virtual void setName(string name)
                {
                    DrawStates[BASE]->name = name;
                }
                bool DrawItems=true;
                bool drawStatesInitalized =false;
                shared_ptr<Skin> skin;
                std::vector<shared_ptr<State>> DrawStates;
        };
    }
}

#endif // MIKUS_MAIGUI_BASES_DRAWABLE_H
