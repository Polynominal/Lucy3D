#ifndef MIKUS_GAMEMANAGER_H
#define MIKUS_GAMEMANAGER_H
#include <Settings.h>

#include <Window/Window.h>
#include <State\Gamestate.h>
#include <SDL.h>
#include <string>
#include <algorithm>
#include <Threads/Threads.h>

class GameManager
{
    public:
        GameManager(Gamestate *state,Windower::Window *W);
        GameManager();

        void push(Gamestate *state);
        void pop();
        void shift(Gamestate *state);

        void event(SDL_Event *event);
        void update(double dt);
        void wrap(); // used for state control to make sure that you don't switch a state while updating or drawing.

        void preDraw();
        void draw();
        void postDraw();


        virtual ~GameManager();
    protected:
    private:
        bool post_pone = false;
        int last_case;
        Gamestate* Active_Gamestate=nullptr;
        Gamestate* target=nullptr;
        Windower::Window *Window =nullptr;
};

#endif // GAMEMANAGER_H
