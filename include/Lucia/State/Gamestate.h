#ifndef MIKUS_LUCIA_GAMESTATE_H
#define MIKUS_LUCIA_GAMESTATE_H
#include <Lucia/Settings.h>
#include <Lucia/Controll/Keyboard.h>
#include <Lucia/Window/Window.h>
#include <iostream>
#include <vector>
namespace Lucia
{
    class Gamestate
    {
        public:
            Gamestate();
            void setWindow(Context::Window* w){Window = w;};
            virtual ~Gamestate();


            // virtual functions for different states used alongside SDL
            // a new Gamestate can redefine these for their own purposes, for convenience they are optional.

            // passes some intro functions as well as the last Gamestate
            virtual void enter(){};

            virtual void leave(){};
            virtual void quit(){};

            // 3D cords in the world. as such <screen> [x,y], <world> [x,y,z]
            virtual void mousepressed(std::string key,int x,int y){};
            virtual void mousereleased(std::string key, int x ,int y ){};
            virtual void internalMousemotion(int x,int y,int relx,int rely){};
            virtual void mousemotion(int x,int y,int relx,int rely){};

            virtual void keypressed(std::string key){};
            virtual void keyreleased(std::string key){};
            virtual void textinput(std::string t){};

            virtual void preUpdate(double delta_time){};
            virtual void internalUpdate(double delta_time){};
            virtual void update(double delta_time){};
            virtual void postUpdate(double delta_time){};
            
            virtual void preDraw();
            virtual void draw(){};
            virtual void postDraw(){};

            virtual void window_resized(int oldw, int oldh, int neww,int newh){};
            virtual void window_positionchanged(int x,int y){};
            virtual void window_focus(bool focus){};

            virtual void drop(std::string filedirectory){};

        protected:
            Controll::Keyboard keyboard = Controll::Keyboard();
            Context::Window *Window;
    };
}


#endif // GAMESTATE_H
