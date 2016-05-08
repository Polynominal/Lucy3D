#include <Graphics/Graphics.h>
namespace Graphics
{
    // variables
    std::map<std::string,ILuint> Loaded_Images;
    float GLOBAL_BORDER_COLOR;
    // methods
    void clear(std::initializer_list<BUFFER> b)
    {
        for (auto v: b)
        {
            clear(v);
        };
    };
    void clear(BUFFER b)
    {
        switch(b)
        {
            case BUFFER::COLOR:
                glClear(GL_COLOR_BUFFER_BIT);
                break;

            case BUFFER::DEPTH:
                glClear(GL_DEPTH_BUFFER_BIT);
                break;

            case BUFFER::STENCIL:
                glClear(GL_STENCIL_BUFFER_BIT);
                break;

            case BUFFER::ACCUM:
                glClear(GL_ACCUM_BUFFER_BIT);
                break;
        }
    };
    void destroy()
    {

    };
}
