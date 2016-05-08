#include <Settings.h>

#ifdef __WIN32
#include <winsock2.h>
#include <windows.h>
#endif // __WIN32
// STL includes [-
#include <stdio.h>
#include <iostream>
#include <string>
// -]

// GL stuff [-
#include <SDL.h>
#include <Settings.h>
#include "glm.hpp"
// -]

// Devill [-
#include <IL/il.h>
#include <IL/ilut.h>
// -]

// Debug and logging [-
#include <utilities/Utils.h>
#include <utilities/Log.h>
// -]

// Engine specific quirks. [-
#include <Graphics\Scene\Instance.h>
#include <State\GameManager.h>
#include <Network\win32.h>

//-]
#include <Collider/Collider.h>
#include <Window/Windower.h>
// [- Gamestates -
#include <MainGame.h>
// -]
#include <Controll/Safety/Lucia.h>
//
#include <Controll/Safety/GLWrapper.h>

#undef main
void runserver(int a,std::atomic<bool> &finished)
{
    Log newLog = Log("Server");
    newLog.overwrite = true;
    int id;
    std::string msg;
    Sockets Network = Sockets();
    if (Network.init())
    {
        Sockets::UDPServer Server = Sockets::UDPServer();
        Server.init();
        newLog << "Server Online" << newLog.endl;
        if (Server.bindListener()){
            newLog << "Server Bound" << newLog.endl;
            while(finished)
            {
                if (Server.receive(id,msg,10)){
                    newLog << "Server has msg from" << id << newLog.endl;
                    msg = std::string("Hello user: ") + std::to_string(id);
                    newLog << "Sending:" << msg << newLog.endl;
                    Server.dispatch(id,msg,1024);
                    newLog.write();
                };
            }
        }else{newLog << "Server failed to bind!" << newLog.endl;};
    }else{newLog << "NETWORK FAILED" << newLog.endl;}
}
void runclient(int a,std::atomic<bool> &finished)
{
    Log newLog = Log("Sally");
    Sockets Network = Sockets();
    std::string msg;
    if (Network.init())
    {
        Sockets::UDPClient Client = Sockets::UDPClient();
        if (Client.init("127.0.0.1"))
        {
            newLog << "Connected to localhost!" << newLog.endl;
            if (Client.dispatch("Yellow!",10))
            {
                newLog << "Packet sent!" << LOG.endl;
            };
            while(finished)
            {
                if (Client.receive(msg,1024)){
                    newLog << "Msg arrived in client: " << msg << newLog.endl;
                    newLog.write();
                };
            }
        }
    }
}
int main_loop(int argc, char* argv[],Windower::Window* w) {
	 //  ----- Initialise DevIL -----

	LOG.overwrite = true;
	LOG.addSeverity(1,"Info");
    LOG.addSeverity(2,"Warning",Log::YELLOW);
    LOG.addSeverity(3,"Debug",Log::CYAN);
    LOG.addSeverity(4,"Fatal",Log::BRED,[](){LOG.write();});

    Threads t = Threads();

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_VideoInit(NULL);

    LOG.send(1,"Program started");

    w->create();
    w->setSize(800,600);


    // Done testing the waters, now moving onto the bigger fish!
    //GLenum err = glewInit();

        //LOG.send("Fatal","GLEW ERROR:");
        //std::cout << glewGetErrorString(err) << std::endl;

    MainGame Test =  MainGame();
    GameManager Gadmin = GameManager(&Test,w);

    double last_time = SDL_GetTicks();

    while(w->isAlive())
    {
        double current = SDL_GetTicks();
        double elapsed = current - last_time;

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            Gadmin.event(&event);
        }

        w->update();

        Gadmin.update(elapsed);
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
    Lucia errors;

    Windower::Window* window = new Windower::Window();

    Lucia_GLWrapper(&errors,window);

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
