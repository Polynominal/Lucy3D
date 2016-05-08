#include "Compatability/GLES2/Shaders/Basic2D.h"
typedef Graphics::Shaders::Basic2D Basic2D;
Basic2D::Basic2D()
{
    //ctor
}
void Basic2D::sendData(bool useColor,float r,float g,float b)
{
    glUniform1i(glGetUniformLocation(Graphics::_Shaders::Basic2D->programID, "useColor"),1);
    glUniform4f(glGetUniformLocation(Graphics::_Shaders::Basic2D->programID, "Color"),r,g,b,1.0f);
}
void Basic2D::use()
{
    glUseProgram(Graphics::_Shaders::Basic2D->programID);
}
Basic2D::~Basic2D()
{
    //dtor
}
