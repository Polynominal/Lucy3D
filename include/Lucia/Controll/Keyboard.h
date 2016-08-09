#ifndef MIKUS_LUCIA_CONTROLL_KEYBOARD_H
#define MIKUS_LUCIA_CONTROLL_KEYBOARD_H
#include <SDL.h>
#include <string>
namespace Lucia {
    namespace Controll
    {
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
    }
}

#endif // KEYBOARD_H
