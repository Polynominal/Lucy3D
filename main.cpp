

// STL includes [-
#include <stdio.h>
#include <iostream>
#include <string>
// -]

// GL stuff [-
#include <Lucia/Settings.h>
#include "glm.hpp"
// -]

// Devill [-
#include <IL/il.h>
#include <IL/ilut.h>
// -]

// Debug and logging [-
#include <Lucia/Utils/Utils.h>
#include <Lucia/Utils/Log.h>
// -]

// Engine specific quirks. [-
#include <Lucia\Graphics\Scene\Instance.h>
#include <Lucia\State\GameManager.h>
#include <Lucia\Network\win32.h>

//-]
#include <Lucia\Collider/Collider.h>
#include <Lucia\Window/Context.h>
// [- Gamestates -
#include <MainGame.h>
// -]
#include <Lucia/Controll/Safety/Error.h>
#include <Lucia/Controll/Safety/GLWrapper.h>
#include <Lucia/Controll/Safety/Handler.h>
//
using namespace Lucia;
using namespace Lucia::Threading;
#undef main
int main_loop(int argc, char* argv[],Context::Window* w) {
	 //  ----- Initialise DevIL -----

	LOG.overwrite = true;
	LOG.addSeverity(1,"Info");
    LOG.addSeverity(3,"Building",Log::GREEN);
    LOG.addSeverity(2,"Warning",Log::YELLOW);
    LOG.addSeverity(3,"Debug",Log::CYAN);
    LOG.addSeverity(4,"Fatal",Log::BRED,[](){LOG.write();});

    Threads t = Threads();

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_VideoInit(NULL);

    LOG.send(1,"Program started");

    w->create();
    //we need the window up for this due to needing a surface and a window handle.
    // compatability settings eg turn off desktop composition.
    // platform dependent.
    Lucia::Compat::patch();

    w->setSize(800,600);

    // Done testing the waters, now moving onto the bigger fish!
    //GLenum err = glewInit();

        //LOG.send("Fatal","GLEW ERROR:");
        //std::cout << glewGetErrorString(err) << std::endl;

    MainGame Test =  MainGame();
    GameManager Gadmin = GameManager(&Test,w);

    double last_time = 0;
    while(w->isAlive())
    {
        double current = SDL_GetTicks();
        double elapsed = current - last_time;
        last_time = current;

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            Gadmin.event(&event);
        }
        w->update();
        Gadmin.update(elapsed/1000.0d);

        Gadmin.preDraw();
        Gadmin.draw();
        Gadmin.postDraw();
        Gadmin.wrap();
    };

    LOG.write();
    // Close and destroy the window

    // Clean up

    SDL_VideoQuit();
    SDL_Quit();

    return 0;
}

int main(int argc, char* argv[])
{
    Error errors;

    Context::Window* window = new Context::Window();

    // this is for gl context / window related functions


    Error_GLWrapper(&errors,window);
    Controll::Safety::OnFatalCrash = [&errors](std::string data)
    {
        errors.reportError(data,true);
    };

    auto fn = errors.newFunction("main",[argc,argv,window]()
    {
        main_loop(argc,argv,window);
    });

    errors.handleException(fn);
    errors.presentErrors(fn);

    while (true){};
    window->destroy();
    delete window;
}
