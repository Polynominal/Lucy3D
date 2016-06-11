#ifndef MIKUS_GRAPHICS_H
#define MIKUS_GRAPHICS_H

#include <Settings.h>
#include <SDL.h>

#include <IL/il.h>
#include <IL/ilut.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <string>
#include <map>
#include <functional>
#include <vector>
#include <settings.h>

namespace Graphics
{
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
// preform compatability task such as loading specific files.
#ifdef USE_GLES2
#include <Compatability/GLES2/Framebuffer.h>
#endif // WSS_GRAPHICS_H

#endif
