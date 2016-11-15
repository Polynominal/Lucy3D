#include "Lucia/State/Gamestate.h"
using namespace Lucia;
Gamestate::Gamestate()
{

    //ctor
}
void Gamestate::preDraw()
{
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}
void Gamestate::extend(std::shared_ptr<Gamestate> state)
{
    internalStates.push_back(state);
    state->Window = Window;
    state->keyboard = keyboard;
    state->mouse = mouse;
}
Gamestate::~Gamestate()
{
    //dtor
}
