#ifndef KEYBOARD_H
#define KEYBOARD_H
#include <SDL.h>
#include <string>

class Keyboard
{
    public:
        Keyboard();
        bool isDown(std::string key);

        virtual ~Keyboard();
    protected:
    private:
        int* number;
        const Uint8 *state;


};

#endif // KEYBOARD_H
