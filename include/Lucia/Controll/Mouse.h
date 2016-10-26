#ifndef MIKUS_LUCIA_CONTROLL_MOUSE_H
#define MIKUS_LUCIA_CONTROLL_MOUSE_H
#include <SDL.h>
#include <string>
namespace Lucia {
    namespace Controll
    {
        class Mouse
        {
            public:
                Mouse(){};
                bool isDown(std::string key)
                {
                    auto state = SDL_GetMouseState(NULL, NULL);
                    if (key == "l"){return state & SDL_BUTTON(SDL_BUTTON_LEFT);};
                    if (key == "m"){return state & SDL_BUTTON(SDL_BUTTON_LEFT);};
                    if (key == "r"){return state & SDL_BUTTON(SDL_BUTTON_LEFT);};
                    if (key == "x1"){return state & SDL_BUTTON(SDL_BUTTON_LEFT);};
                    if (key == "x2"){return state & SDL_BUTTON(SDL_BUTTON_LEFT);};
                };
                void getKeys(bool* buttons)
                {
                    auto state = SDL_GetMouseState(NULL, NULL);
                    buttons[0] =  (unsigned int)(state & SDL_BUTTON(SDL_BUTTON_LEFT)) == 1;
                    buttons[1] =  (unsigned int)(state & SDL_BUTTON(SDL_BUTTON_MIDDLE)) == 2;
                    buttons[2] =  (unsigned int)(state & SDL_BUTTON(SDL_BUTTON_RIGHT)) == 4;
                    buttons[3] =  (unsigned int)(state & SDL_BUTTON(SDL_BUTTON_X1)) == 8;
                    buttons[4] =  (unsigned int)(state & SDL_BUTTON(SDL_BUTTON_X2)) == 16;
                };
                virtual ~Mouse(){};
        };
    }
}

#endif // mouse
