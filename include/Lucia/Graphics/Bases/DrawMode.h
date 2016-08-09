#ifndef MIKUS_LUCIA_GRAPHICS_BASES_DRAWMODE_H
#define MIKUS_LUCIA_GRAPHICS_BASES_DRAWMODE_H
#include <Lucia/Settings.h>
namespace Lucia {
namespace Graphics
{
    enum DRAW
    {
        POINTS,
        TRIANGLE,
        TRIANGLE_STRIP,
        TRIANGLE_FAN,
        QUAD,
        QUAD_STRIP,
        QUAD_FAN,
        POLYGON,

        LINES,
        STRIP,
        LOOP,
    };
    inline int getMechanicalTarget(DRAW c)
    {
        int id=GL_TRIANGLES;
        switch(c)
        {
        case POINTS:
            {
                id = GL_POINTS;
                break;
            }
        case TRIANGLE:
            {
                id = GL_TRIANGLES;
                break;
            }
        case TRIANGLE_STRIP:
            {
                id = GL_TRIANGLE_STRIP;
                break;
            }
        case TRIANGLE_FAN:
            {
                id = GL_TRIANGLE_FAN;
                break;
            }
        #ifndef LUCIA_USE_GLES2
        case QUAD:
            {
                id = GL_QUADS;
                break;
            }
        case QUAD_STRIP:
            {
                id = GL_QUAD_STRIP;
                break;
            }
        case QUAD_FAN:
            {
                id = GL_QUAD_STRIP;
                break;
            }
        case POLYGON:
            {
                id = GL_POLYGON;
                break;
            }
        #else
        case QUAD:
        {
            id = GL_TRIANGLES;
            break;
        }
        case QUAD_STRIP:
        {
            id = GL_TRIANGLE_STRIP;
            break;
        }
        case QUAD_FAN:
        {
            id = GL_TRIANGLE_FAN;
            break;
        }
        case POLYGON:
        {
            id = GL_TRIANGLE_STRIP;
            break;
        }
        #endif
        case LINES:
            {
                id = GL_LINES;
                break;
            }
        case STRIP:
            {
                id = GL_LINE_STRIP;
                break;
            }
        case LOOP:
            {
                id = GL_LINE_LOOP;
                break;
            }
        }
        return id;
    };

    namespace Base
    {
        class DrawMode
        {
            public:
                DrawMode(){setDrawMode(DRAW::TRIANGLE);};
                virtual ~DrawMode(){};
                virtual void setDrawMode(DRAW Mode)
                {
                    drawMode = Mode;
                    drawmode_mechanical_reference = getMechanicalTarget(Mode);
                };
                virtual DRAW getDrawMode(){return drawMode;};
                virtual int getMechanicalMode(){return drawmode_mechanical_reference;}
            protected:
                int drawmode_mechanical_reference=0;
                DRAW drawMode       =DRAW::TRIANGLE;
        };
    }
}
}

#endif // MIKUS_GRAPHICS_BASES_DRAWMODE_H
