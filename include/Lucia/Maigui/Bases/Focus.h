#ifndef MIKUS_LUCIA_MAIGUI_FOCUS_H
#define MIKUS_LUCIA_MAIGUI_FOCUS_H

#include <iostream>
#include <algorithm>
#include <functional>
#include <memory>
#include <Lucia/Collider/Shape.h>

namespace Lucia
{
    namespace Maigui
    {
        namespace Base
        {
                class Focus
                {
                    public:
                        Focus(){}
                        ~Focus(){}
                        //set
                        virtual void setMouse(){};
                        virtual void setInactive(bool condition){Inactive = condition;};
                        virtual void setFocus(bool condition);
                        void setShape(std::shared_ptr<Collider::Shape> s){CoreShape = s;};
                        //get 
                        std::shared_ptr<Collider::Shape> getShape(){return CoreShape;};
                        //has
                        virtual bool hasMouse(){return false;};
                        virtual bool hasFocus(){return currentFocus;};
                        //misc 
                        virtual bool checkFocus(){return hasFocus();};
                        //events
                        virtual void mousepressed(std::string key);
                        virtual void mousereleased(std::string key);
                        virtual void textinput(char);
                        virtual void keypressed(std::string);
                        //ON's
                        virtual void onFocus(bool f){};
                        virtual void onRelease(Focus *b){};
                        virtual void onCollide(Focus *b){};
                        //lambdas
                        std::function<void(bool)>OnFocus = [](bool hasFocus){};
                        std::function<void(std::string)>OnClick = [](std::string b){};
                        std::function<void(std::string)>OnReleaseClick = [](std::string b){};
                    
                    protected:
                        std::shared_ptr<Collider::Shape> CoreShape;
                        bool Inactive= true; // inactive cannot not process input but is drawn
                        bool Visible = false; // invisible but can still process input
                        bool currentFocus =false;
                        bool lastFocus=false;
                };
        }
    }
}
#endif