#include "Controll/Keyboard.h"

Keyboard::Keyboard()
{
    state = SDL_GetKeyboardState(NULL);

    //ctor
}
bool Keyboard::isDown(std::string name)
{
    SDL_Scancode scancode;
    scancode = SDL_GetScancodeFromKey(SDL_GetKeyFromName(name.c_str()));
    if (state[scancode])
    {
        return true;
    };
    return false;
}
Keyboard::~Keyboard()
{
    //dtor
}
