#ifndef MIKUS_WINDOW_H
#define MIKUS_WINDOW_H
#include <Settings.h>
#include <Startup.h>
#include <Window/Windower.h>
#include <Window/Data.h>

class Windower::Window
{
    public:
        enum flag
        {
            hidden,
            borderless,

            maximixed,
            minimized,

            resizable,

            fullscreen,
            fullscreenDesktop,

            focus,

            highdpi,
            trapMouse,

        };
        // Sets
            void setAspect(uint x,uint y);
            void setTitle(std::string title);

            void setBorder(bool);
            void setMode(uint width,uint height,uint refreshRate);
            void lockMouse(bool free); // grabbed or free
            void lockMouseRelative(bool free);

            void setSize(uint x,uint y);
            void setPosition(uint x,uint y);

            void setMaxSize(uint x,uint y);
            void setMinSize(uint x,uint y);



            void setResizable(bool);

        // Gets
            vec2 getSize(){return size;}; // returns W,H
            vec2 getPosition(){return Position;};

            std::map<flag,bool> getFlags();

            bool getBorder();
            bool getMouseLock(){return flags[flag::trapMouse];};
            bool getResizable(){return flags[flag::resizable];};
            Mode getMode(){return mode;};


        //voids
            bool create();

            bool isAlive(){return alive;};

            void hide();
            void show();
            void restore();

            void grab(bool condition){lockMouse(condition);}; // short hand for setMouseState
            void lock(bool); // locks the window in its current position.

            // short hand for flagging fullscreen.
            void fullScreen(bool condition,bool desktop=false);
            //destroys the window, keeps the settings however to recreate
            void destroy();

            void update();

            string getTitle();
            uint getWidth();
            uint getHeight();

        Window();
        virtual ~Window();
    private:
        Mode mode;
        std::map<flag,bool> flags{
            {hidden,false},
            {borderless,false},

            {maximixed, false},
            {minimized,false},
            {resizable , false},

            {fullscreen , false},
            {fullscreenDesktop , false},

            {highdpi , false},
            {trapMouse , false}
        };
        // Dynamic data
        bool alive = false;
        std::string title;
        vec2 size;

        vec2 maxSize;
        vec2 minSize;

        vec2 Position;

        // Static data
        unsigned int id;
        SDL_GLContext glContext;
        SDL_DisplayMode Settings;
        SDL_Window* Core;

};

#endif // MIKUS_WINDOW_H
