#include "Lucia\State/GameManager.h"
using namespace Lucia;
GameManager::GameManager(Gamestate *state,Context::Window *w)
{

    Window = w;
    state->setWindow(w);
    state->enter();
    Active_Gamestate = state;
    for(auto v: state->internalStates ) {v->enter();};
    Maths::randomSeed();
    //ctor
}
GameManager::GameManager()
{
    //ctor2
}
void GameManager::push(Gamestate *target)
{
    post_pone = true;
    target = target;
}
void GameManager::shift(Gamestate *target)
{

    Active_Gamestate->leave();
    target->enter();
    Active_Gamestate = target;

}
const char* errorToString(GLenum s)
{
    const char* output;
    switch(s)
    {
        case GL_INVALID_ENUM:
            output = "GL_INVALID_ENUM";
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            output = "GL_INVALID_FRAMEBUFFER_OPERATION";
            break;
        case GL_INVALID_VALUE:
            output = "GL_INVALID_VALUE";
            break;
        case GL_INVALID_OPERATION:
            output = "GL_INVALID_OPERATION";
            break;
        case GL_OUT_OF_MEMORY:
            output = "GL_OUT_OF_MEMORY";
            break;
        case GL_NO_ERROR:
            output = "GL_NO_ERROR";
            break;
    }
    return output;
};
void GameManager::update(double dt)
{
    for (auto v: Active_Gamestate->internalStates)
    {
        v->preUpdate(dt);
        v->update(dt);
        v->postUpdate(dt);
    };
    Active_Gamestate->preUpdate(dt);
    Active_Gamestate->update(dt);
    Active_Gamestate->postUpdate(dt);
    
    int err;
    if ((err = glGetError()) != last_case)
    {
        std::cerr << "OpenGL error: " << err << errorToString(err) << std::endl;
        last_case = err;
    }
}
void GameManager::preDraw()
{
    for (auto v: Active_Gamestate->internalStates){v->preDraw();};
    Active_Gamestate->preDraw();
}
void GameManager::postDraw()
{
    for (auto v: Active_Gamestate->internalStates){v->postDraw();};
    Active_Gamestate->postDraw();
}
void GameManager::draw()
{
  for (auto v: Active_Gamestate->internalStates){v->draw();};
  Active_Gamestate->draw();
}
void GameManager::wrap()
{
    if (post_pone)
    {
        this->shift(target);
    };
}
std::string intToStringMouse(int a)
{
    std::string key = "NULL";
    switch(a)
    {
        case SDL_BUTTON_LEFT:
        {
            key = "l";
            break;
        }
        case SDL_BUTTON_MIDDLE:
        {
            key = "m";
            break;
        }
        case SDL_BUTTON_RIGHT:
        {
            key = "r";
            break;
        }
        case SDL_BUTTON_X1:
        {
            key = "x";
            break;
        };
        case SDL_BUTTON_X2:
        {
            key = "x2";
            break;
        }
    };
    return key;
}
void GameManager::event(SDL_Event *event)
{
    switch(event->type)
    {
    case SDL_KEYDOWN:
        {
            std::string key = SDL_GetKeyName(event->key.keysym.sym);
            std::transform(key.begin(), key.end(),key.begin(), ::tolower);
            
            for (auto v: Active_Gamestate->internalStates){v->keypressed(key);};
            Active_Gamestate->keypressed(key);
        }
        break;
    case SDL_KEYUP:
        {
            std::string key = SDL_GetKeyName(event->key.keysym.sym);
            std::transform(key.begin(), key.end(),key.begin(), ::tolower);
            
            for (auto v: Active_Gamestate->internalStates){v->keyreleased(key);};
            Active_Gamestate->keyreleased(key);
            break;
        }
    case SDL_MOUSEBUTTONDOWN:
        {
            int x = event->button.x;
            int y = event->button.y;
            auto key = intToStringMouse(event->button.button);
            for (auto v: Active_Gamestate->internalStates){v->mousepressed(key,x,y);};
            Active_Gamestate->mousepressed(key,x,y);

            break;
        }
    case SDL_MOUSEBUTTONUP:
        {
            int x = event->button.x;
            int y = event->button.y;
            
            auto key = intToStringMouse(event->button.button);
            for (auto v: Active_Gamestate->internalStates){v->mousereleased(key,x,y);};
            Active_Gamestate->mousereleased(key,x,y);
            
            break;
        }
    case SDL_MOUSEMOTION:
        {
            int x           = event->motion.x;
            int y           = event->motion.y;

            int relativex   = event->motion.xrel;
            int relativey   = event->motion.yrel;
            for (auto v: Active_Gamestate->internalStates){v->mousemotion(x,y,relativex,relativey);};
            Active_Gamestate->mousemotion(x,y,relativex,relativey);

            break;
        }
    case SDL_WINDOWEVENT:
        {

        }
        break;

    case SDL_DROPFILE:
        {

            std::string dir = event->drop.file;
            std::string directory = dir;
            SDL_free(event->drop.file);

        }
        break;

    case SDL_QUIT:

        for (auto v: Active_Gamestate->internalStates){v->quit();v->cleanUp();};
        Active_Gamestate->quit();
        Active_Gamestate->cleanUp();
        exit(1);

        break;
    };
}
GameManager::~GameManager()
{
    //dtor
}
