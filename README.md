# Lucia
##What is Lucia?

Lucia is a Work in progress 3D engine designed to be a versitile platform for building complex user interfaces including 3D graphics and systems including video games and other mediums.


Lucia focues on user interaction to insure that development with Lucia is as intuitive as it can be, meanwhile providing reliable preformance.

##How can I contribute?

To contribute to the project you need to pick a specific missing or incomplete field within this diagram:

![Alt text](https://github.com/Polynominal/Lucy3D/raw/master/progress.png?raw=true "Current progress")

Or add a completley new feature, its up to you. All new features must be tested.

###General guide lines

Use virtual (non-pure) base classes to reduce workload. 

Namespaces should reflect file structure.

Everything you can set in the class you should get from the class.

Seperate the following functions in groups:

-------------------------------------------


HAS


SET


GET


IS


--------------------------------------------

All base (non-pure and pure virtual) classes must be placed in a namespace Base which is global to the Module for example:

Graphics/Base/my_base.h  and you can then use it in other classes.


There are some exceptions regarding highly specialized base classes that are only designed to work alongside one or two classes so use definition outside of /Base/ directory is applicable in such cases.


# Compiling


There are no CMAKE lists yet, I would be glad if anyone could help me with that meanwhile follow these instructions.

Fetch all the libraries from /Dependencies/ from their repsective sources and compile them so you have
/lib/ and /src/ in each of them (or whatever you preffer for .o and .h files) and link them.

Make sure to link OpenGL/GLES to do so see Settings.h where you can pick your OpenGL version.

Please note that GLES is only compatable with AMD (as far as I know) so if you are using Nvidia or Intel you may wish to find out if your graphics card supports GLES emulation and if not use casual OpenGL.

Non-GLES render is not as widely tested but should work regardless, to set the renderer please use Settings.h

The programm will automaticaly apply compatability settings in windows (linux/mac work in progress) if required.
You can disable this in main.cpp.

#Documentation

Lucia was designed with you in mind. I designed lucia to make sure that its the easiest to use. So I decided to abandon the main.cpp style of programming and focus on gamestates.

To get lucia running you will need two main methods.
One for launching Lucia.
Another for catching errors. 

Here is how a typical Lucia main.cpp looks like:

```C++
// STL includes [-
#include <stdio.h>
#include <iostream>
#include <string>
// -]

// Engine specific quirks. [-
#include <Lucia\Settings.h>
#include <Lucia\State\GameManager.h>
#include <Lucia\Window\Context.h>

#include <Lucia\Utils\Log.h>
#include <Lucia\Controll\Safety\Error.h>
#include <Lucia\Controll\Safety\GLWrapper.h>
#include <Lucia\Controll\Safety\Handler.h>
//-]

// [- Gamestates -
#include <Gamestate\MainGame.h>
// -]
using namespace Lucia;
using namespace Lucia::Threading;

//here we set up the inner loop which will be isolated from the main thread.
// its puprose is to shield you from segments faults and other evil creatures and allows you to see the reason why your stuff //crashed

int main_loop(int argc, char* argv[],Context::Window* w) {
    //we set this to over write previous file with the same name.
	  LOG.overwrite = true;
	  // here we add all the severities to be used alongside LOG << severity_name << message
	  LOG.addSeverity(1,"Info");
    LOG.addSeverity(3,"Building",Log::GREEN);
    LOG.addSeverity(2,"Warning",Log::YELLOW);
    LOG.addSeverity(3,"Debug",Log::CYAN);
    LOG.addSeverity(4,"Fatal",Log::BRED,[](){LOG.write();}); // fatal has a lambda attached to it which will force it to write
    //the log in case we crash after wards.
    
    //open a new threading instance will be used for encapsulation mentioned before.
    Threads t = Threads();
    
    //create SDL stuff on the current thread.
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_VideoInit(NULL);

    LOG.send(1,"Program started");

    w->create();
    //we need the window up for this due to needing a surface and a window handle.
  
    // we now need to apply compatability settings eg turn off desktop composition.
    // this is platform dependent and is made automaticaly from macros defined in settings.h.
    
    Lucia::Compat::patch();

    w->setSize(800,600);

    // Done testing the waters, now moving onto the bigger fish!
    //GLenum err = glewInit();

        //LOG.send("Fatal","GLEW ERROR:");
        //std::cout << glewGetErrorString(err) << std::endl;

    //Create a new state in this case MainGame
    auto state =  new MainGame();
    //Create a Game state manager for managing our state. we will use this for state switches.
    GameManager Gadmin = GameManager(state,w);

    //we create our update loop.
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
    
        //parse our gamestates
        Gadmin.preDraw();
        Gadmin.draw();
        Gadmin.postDraw();
        Gadmin.wrap();
    };
    delete state;
    
    LOG.write();
    // Close and destroy the window

    // Clean up

    SDL_VideoQuit();
    SDL_Quit();

    return 0;
}
//we need to make sure main is not defined elsewhere like SDL.
#undef main
int main(int argc, char* argv[])
{
  //create a new error instance to record our errors.
    Error errors;
    
    //we must create the context in the main thread.
    Context::Window* window = new Context::Window();

    // this is for gl context / window related functions
    
    //apply GL error wrapper to our new window, this is for making sure we can print our stuff if we fail to do something.
    Error_GLWrapper(&errors,window);
    Controll::Safety::OnFatalCrash = [&errors](std::string data)
    {
        errors.reportError(data,true);
    };
    
    // this new function will now create a sandbox enviroment for us to play in.
    auto fn = errors.newFunction("main",[argc,argv,window]()
    {
        main_loop(argc,argv,window);
    });
    
    //if we have errors handle them here.
    errors.handleException(fn);
    errors.presentErrors(fn);

    window->destroy();
    delete window;
    return 0;
}
```
#Gamestate

Now you will need to establish a gamestate. To do so we need to create a new class, and use the gamestate base class out of one of the lucrative pre made bases such as Extended.h

```C++
#include <Lucia/State/Gamestate.h>
#include <Lucia/Graphics/Camera.h>
#include <Lucia/Graphics/Graphics.h>

#include <Lucia/Graphics/Objects/Model.h>
//Introduce our new and shiny Extended.h! this gamestate is designed for your comfort. putting you first.
#include <Lucia/State/Extended.h>
// include our scene of choice.
#include <Lucia/Graphics/Scene/Scene3D.h>
#include <Lucia/Maigui/Types/Container.h>
#include <Lucia/Maigui/Types/Button.h>

#include <cstdlib>

using namespace Lucia;
using namespace Graphics;
class MainGame : public State::Extended
{
    private:
        //create our new 3D scene.
        Scene::Scene3D* newScene = new Scene::Scene3D(); // a smart pointer would be much more appropriate here.
        // remember NEW IS EVIL!!!!!!
        
    public:
        MainGame(){};
        void enter()
        {
            
            newScene->generate(Window);
            //set our clear color.
            newScene->setColor(0,0,0,255);
            
            //generates a new Collider instance with camera projection.
            generateCollider(newScene->getCamera());
            //generates the user interface and applies default skin.
            generateUserInterface();
            //generates OpenGL collider wrappers for the UI
            applyColliderWraperToUI();
            
            //lets add a button in the center of the screen with size (200,200,1) Yes! the UI is 100% 3D!!!
            //although some parts are still in progress like text scaling.
            auto c = UserInterface->addButton((800/2.0f),(600/2.0f),1000, 200,200,1);
            auto image = std::make_shared<Image>("button.png");
            image->generate();
            auto w = image->getWidth();
            auto h = image->getHeight();
            c->setSprite(image,0,0,w,h);
            c->scaleTo(w,h,0);
            
            //get camera from our scene.
            auto cam = newScene->getCamera();
            cam->moveTo(0.0f,0.0f,-2.0f);
            
        };
        void draw()
        {
            //draw the whole sorted scene.
            newScene->draw();
            //same as draw skeleton from the UI.
            Collider->drawChunks(true);
            UserInterface->draw();
            //draw the skeleton around all the UI including Octtree implementation
            UserInterface->drawSkeleton();
        };

        void update(double delta_time)
        {
          //we need to update the scene to call a Update function on all of our objects within.
          // we do some sorting here too for transparent objects like windows.
            newScene->update(delta_time);
            
        };
        void keypressed(std::string key)
        {
            auto cam = newScene->getCamera();
            if (key == "w"){cam->strafe("forward",0.5f);}
            else if (key == "s"){cam->strafe("backward",0.5f);}
            else if (key == "a"){cam->strafe("left",0.5f);}
            else if (key == "d"){cam->strafe("right",0.5f);}
        };
        void mousepressed(std::string key,int x,int y)
        {

        };
        void mousereleased(std::string key, int x ,int y )
        {

        };
        void mousemotion(int x,int y,int relx,int rely)
        {
            
        }
        ~MainGame()
        {
            delete newScene; // Use smart pointers next time !!! grrr.
        };
};
```


WIP

