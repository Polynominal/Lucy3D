#ifndef MIKUS_LUCIA_CONTEXT_H
#define MIKUS_LUCIA_CONTEXT_H
#include <vector>
#include <memory>
#include <SDL.h>

#include <string>
#include <vector>
#include <map>
namespace Lucia {
    namespace Context
    {
        class Window;
        class Clipboard;
        class Message;
        class Error;

        namespace Data{};

        typedef unsigned int uint;
        struct vec2
        {
            vec2(){};
            vec2(uint xt,uint yt): x(xt),y(yt){};
            uint x;
            uint y;
        };
        struct Mode
        {
            Mode(){};
            Mode(uint w,uint h,uint r):
            width(w),
            height(h),
            refreshRate(r){};
            uint width;
            uint height;
            uint refreshRate;
        };

        bool switchTo(Window *W);
        std::vector<Window> getWindows();
        bool close(); // closes down all windows;

    };
}
#include <Lucia/Window/Window.h>
#endif // MIKUS_WINDOW_H
