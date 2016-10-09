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
                int* getKeys()
                {
                    auto state = SDL_GetMouseState(NULL, NULL);
                    int buttons[5] = 
                    {
                        state & SDL_BUTTON(SDL_BUTTON_LEFT),
                        state & SDL_BUTTON(SDL_BUTTON_MIDDLE),
                        state & SDL_BUTTON(SDL_BUTTON_RIGHT),
                        state & SDL_BUTTON(SDL_BUTTON_X1),
                        state & SDL_BUTTON(SDL_BUTTON_X2)
                    };
                    return buttons;
                };
                virtual ~Mouse(){};
        };
    }
}

#endif // mouse
