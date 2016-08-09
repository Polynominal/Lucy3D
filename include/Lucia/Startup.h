#ifndef MIKUS_LUCIA_STARTUP_H_INCLUDED
#define MIKUS_LUCIA_STARTUP_H_INCLUDED
#include <Lucia/Settings.h>
#include <iostream>
#define LUCIA_PLEASE_USE_STARTUP
#ifdef ANDROID_5
#include <Lucia/Compatability/Android.h>
#endif
namespace Lucia {
inline int startup_tasks()
{
    #ifdef LUCIA_USE_OPENGL3
    std::cout << "Apply GLEW" << std::endl;
    glewExperimental=true;
    int err = glewInit();
    if (err != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW: " << err << std::endl;
        return -1;
    }
    #endif
    return 0;
}}
#endif // STARTUP_H_INCLUDED
