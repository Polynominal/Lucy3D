#ifndef MIKUS_LUCIA_GAMEMANAGER_H
#define MIKUS_LUCIA_GAMEMANAGER_H
#include <Lucia/Settings.h>
#include <Lucia/Maths/General.h>
#include <Lucia/Window/Window.h>
#include <Lucia/State/Gamestate.h>
#include <Lucia/Threads/Threads.h>

#include <SDL.h>
#include <string>
#include <algorithm>

namespace Lucia {
class GameManager
{
    public:
        GameManager(Gamestate *state,Context::Window *W);
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
        Context::Window *Window =nullptr;
};
}
#endif // GAMEMANAGER_H
