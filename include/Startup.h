#ifndef MIKUS_STARTUP_H_INCLUDED
#define MIKUS_STARTUP_H_INCLUDED
#include <Settings.h>
#include <iostream>
#define LUCY_PLEASE_USE_STARTUP
inline int startup_tasks()
{
    #ifdef USE_OPENGL3
    std::cout << "Apply GLEW" << std::endl;
    glewExperimental=true;
    int err = glewInit();
    if (err != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW: " << err << std::endl;
        return -1;
    }
    #endif
}
#endif // STARTUP_H_INCLUDED
