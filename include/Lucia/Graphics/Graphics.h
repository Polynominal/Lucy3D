#ifndef MIKUS_LUCIA_GRAPHICS_H
#define MIKUS_LUCIA_GRAPHICS_H

#include <Lucia/Settings.h>
#include <SDL.h>

#include <IL/il.h>
#include <IL/ilut.h>

#include <Lucia/Maths/Matrix.h>
#include <string>
#include <map>
#include <functional>
#include <vector>
#include <Lucia/Settings.h>
#include <initializer_list>
namespace Lucia
{
    namespace Graphics
    {
        using namespace Maths;
        class Mesh;
        class Image;
        namespace Scene
        {
            class Instance;
            class Object;
            class Container;
        };
        namespace Base
        {
            class Filter;
        };
        class Camera;
        class Text;

        enum BUFFER
        {
            COLOR,
            DEPTH,
            STENCIL,
            ACCUM
        };
        enum SORTMODE
        {
            ZMAP,
            SHADER
        };
        extern std::map<std::string,ILuint> Loaded_Images;
        extern float GLOBAL_BORDER_COLOR;

        void clear(BUFFER b);
        void clear(std::initializer_list<BUFFER> b);

        void destroy();
    };
}

// preform compatability task such as loading specific files.
#ifdef LUCIA_USE_GLES2
#include <Lucia/Compatability/GLES2/Framebuffer.h>
#endif // WSS_GRAPHICS_H

#endif
