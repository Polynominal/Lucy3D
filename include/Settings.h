#ifndef MIKUS_LUCY_SETTINGS_H_INCLUDED
#define MIKUS_LUCY_SETTINGS_H_INCLUDED
// use these at ur own risk:

// GLEW and opengl

//[-
//#define USE_OPENGL3
//#include <GL/glew.h>
//#include <GL/GL.h>

// when linking with these make sure to add the following to ur linker:
// -lGLEW -lGL [OR -lopengl32 for WIN32]
// and Dependencies/build/<your compiler>/lib
//-]

//GLES2
//[-
#define USE_GLES2
// Dont forget to unlink the -lgl librares as they will mess with libEGL causing a blackscreen and multiple crashes etc etc.
// [!!!] You will have to rebuild if this happens D: [!!!]
#include <GLES2/gl2.h>
//-]
#endif // SETTINGS_H_INCLUDED
