#ifndef MIKUS_LUCIA_SETTINGS_H_INCLUDED
#define MIKUS_LUCIA_SETTINGS_H_INCLUDED
//Would you like Lucia to build main functio for you?
//#define LUCIA_INCLUDE_MAIN
// Maths specific.
#define LUCIA_USE_DEGREES
// use these at ur own risk:

// GLEW and opengl

//[-
#define LUCIA_USE_OPENGL3


// when linking with these make sure to add the following to ur linker:
// -lGLEW -lGL [OR -lopengl32 for WIN32]
// and Dependencies/build/<your compiler>/lib
//-]

//OPEN GL ES
//[-
//#define ANDROID_5
//#define LUCIA_USE_GLES2

// Dont forget to unlink the -lgl librares as they will mess with libEGL causing a blackscreen and multiple crashes etc etc.
// [!!!] You will have to rebuild if this happens D: [!!!]
#ifdef LUCIA_USE_OPENGL3
    #include <GL/glew.h>
    #include <GL/GL.h>
#endif
#ifdef LUCIA_USE_GLES2
    #include <GLES2\gl2.h>
#endif
//-]
#include <SDL.h>

#define LUCY_PLEASE_USE_STARTUP
#define GENERATE_SHARED_RECOURCES

#endif // SETTINGS_H_INCLUDED

#ifdef __WIN32
#include <Lucia/Compatability\Windows.h>
#endif // __WIN32
