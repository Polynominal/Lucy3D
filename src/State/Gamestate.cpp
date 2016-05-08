#include "State/Gamestate.h"

Gamestate::Gamestate()
{

    //ctor
}
void Gamestate::preDraw()
{
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}
Gamestate::~Gamestate()
{
    //dtor
}
