#include "Window.h"
#include "glm.hpp"
#include <iostream>
using namespace std;
using namespace Windower;
using namespace Data;
Window::Window()
{
    // basic profile
    title = string("Untitled");
    auto s = Data::Limits::size(0);
    size = vec2(s.x/2,s.y/2);
    // Grabs from the window
    Position = vec2(size.x/2,size.y/2);

    //ctor
}
bool Window::create()
{
    uint w = size.x;
    uint h = size.y;
    uint x = Position.x;
    uint y = Position.y;
    int major =3;
    int minor =1;
    #if defined USE_GLES2 || defined USE_GLES3
    major = 2;
    minor = 0;
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK,SDL_GL_CONTEXT_PROFILE_ES );
    #elif defined USE_OPENGL3
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK,SDL_GL_CONTEXT_PROFILE_CORE );
    #endif // define

    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, major );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, minor );
    Core = SDL_CreateWindow(
        title.c_str(),
        x,
        y,
        w,
        h,
        SDL_WINDOW_OPENGL);

    glContext = SDL_GL_CreateContext(Core);
    if (glContext == NULL)
    {
        std::cout << "MAJOR ERROR: " << SDL_GetError() << std::endl;
    }else
    {
        std::cout << "Context is up!" << std::endl;
    }
    std::cout << "Major glVersion: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    if (Core == NULL)
    {
        return false;
    }else
    {
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
        alive = true;
    }
    #ifdef LUCY_PLEASE_USE_STARTUP
    startup_tasks();
    #endif // uncoditional_init
    return true;
}
void Window::setTitle(std::string title){SDL_SetWindowTitle(Core,title.c_str());};
void Window::setBorder(bool a)
{
    SDL_SetWindowBordered(Core,(SDL_bool)a);
    flags[flag::borderless] = !a;
};
void Window::setMode(uint width,uint height,uint refreshRate){
    SDL_DisplayMode dmode;
    dmode.w = width;
    dmode.h = height;
    dmode.refresh_rate = refreshRate;
    SDL_SetWindowDisplayMode(Core,&dmode);
};
void Window::lockMouse(bool free)
{
    flags[flag::trapMouse] = free;
    SDL_SetWindowGrab(Core,(SDL_bool)free);
} // grabbed or free

void Window::setSize(uint x,uint y)
{
    SDL_SetWindowSize(Core,x,y);
    size = vec2(x,y);
    glViewport(0.0f,0.0f,x,y);
    auto s = Data::Limits::size(0);
    auto sn = vec2(s.x/2,s.y/2);
    // Grabs from the window
    setPosition(sn.x -size.x/2,sn.y - size.y/2);

};
void Window::setPosition(uint x,uint y)
{
    SDL_SetWindowPosition(Core,x,y);
    Position = vec2(x,y);
};

void Window::setMaxSize(uint x,uint y){SDL_SetWindowMaximumSize(Core,x,y);};
void Window::setMinSize(uint x,uint y){SDL_SetWindowMinimumSize(Core,x,y);};

void Window::lock(bool condition){setResizable(false);};
// short hand for flagging fullscreen.
void Window::fullScreen(bool condition,bool desktop){
    if (condition)
    {
        if (desktop)
        {
            flags[flag::fullscreen] = false;
            flags[flag::fullscreenDesktop] = true;
            SDL_SetWindowFullscreen(Core,SDL_WINDOW_FULLSCREEN_DESKTOP);
        }else
        {
            flags[flag::fullscreenDesktop] = false;
            flags[flag::fullscreen] = true;
            SDL_SetWindowFullscreen(Core,SDL_WINDOW_FULLSCREEN);
        }
    }else
    {
        flags[flag::fullscreen] = false;
        flags[flag::fullscreenDesktop] = false;
        SDL_SetWindowFullscreen(Core,0);
    }
}

void Window::setResizable(bool a)
{
    flags[flag::resizable] = a;
    if (!a){setMaxSize(size.x,size.y);setMinSize(size.x,size.y);}
    else{setMaxSize(maxSize.x,maxSize.y);setMinSize(minSize.x,minSize.y);};
};
void Window::hide()
{
    flags[flag::hidden] = true;
    SDL_HideWindow(Core);
};
void Window::show()
{
    flags[flag::hidden] = false;
    SDL_RaiseWindow(Core);
};
void Window::restore()
{
    flags[flag::hidden] = false;
    SDL_RestoreWindow(Core);
};
void setIcon(string data){

};
string Window::getTitle()
{
    return string(SDL_GetWindowTitle(Core));
};
void Window::update(){SDL_GL_SwapWindow(Core);};
void Window::destroy()
{
    alive = false;
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(Core);
}
void Window::lockMouseRelative(bool a)
{
    SDL_SetRelativeMouseMode((SDL_bool)a);
    if (a)
    {
        SDL_WarpMouseInWindow(Core,size.x/2,size.y/2);
    }

}
uint Window::getWidth(){return size.x;};
uint Window::getHeight(){return size.y;};
Window::~Window()
{
    //dtor
}
