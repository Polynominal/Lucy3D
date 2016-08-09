#ifndef MIKUS_LUCIA_MAIGUI_OPENGL_H_INCLUDED
#define MIKUS_LUCIA_MAIGUI_OPENGL_H_INCLUDED

// we can re-use some of the collider opengl wrappers since we will be using that anyway.
#include <Lucia/Maigui/Maigui.h>
#include <Lucia/Collider/Opengl.h>
namespace Lucia {
namespace Maigui
{
    namespace OpenGL
    {
        struct Vertex_Buffer
        {
            Vertex_Buffer(float nx,float ny,float nz,float nu,float nv,float ne,float nn):
            x(nx),y(ny),z(nz),u(nu),v(nv),e(ne),n(nn){};
            float x;
            float y;
            float z;
            float u;
            float v;
            float e;
            float n;
        };
        enum Shader
        {
            ITEM,
            TEXT,
            NIL
        };
        extern Shader activeShader;
        extern GLuint programID;
        // We do not need a sphere or any other primitive just yet.
        // vao and size of the square
        extern GLuint VaoID;
        extern GLuint VboID;
        extern string vertex;
        extern string Fragment;
        extern std::vector<Vertex_Buffer> data;

        void attachWrappers(Skin *s,Maths::Matrix<4> *view,Maths::Matrix<4> *projection);
        void cleanUp();
    };
};
}

#endif // MAIGUI_OPENGL_H_INCLUDED
