#include <Lucia/Settings.h>
#ifndef MIKUS_LUCIA_OPENGL_COLLIDER_WRAPPER_H_INCLUDED
#define MIKUS_LUCIA_OPENGL_COLLIDER_WRAPPER_H_INCLUDED

#include <Lucia/Collider/Collider.h>
#include <Lucia/Collider/Manager.h>

#include <Lucia/Maths/Matrix.h>

#include <Lucia/Utils/OpenGL.h>
#include <Lucia/Utils/log.h>

namespace Lucia {
    namespace Collider_OpenGL
    {

        using namespace Collider;
        using namespace Utils::OpenGL;
        typedef Utils::OpenGL::Vertex_Buffer Buffer_Vertex;

        extern bool clean;
        extern int draw_mode;
        extern std::map<Shape::Type,Buffer*> BufferIDs;
        extern std::vector<Buffer*> polygonBufferIDs;

        Buffer* makeSquare();
        int makePolygon(Collider::Manager &M, std::vector<Collider::Vertex> Points);
        Buffer* makeSphere(int subdiv_x,int subdiv_y);
        Buffer* makePoint();
        Buffer* makeRay();

        void drawRay(Buffer* Buff,Collider::Vertex A,Collider::Vertex B);
        void createWrappers(Collider::Manager *M,Maths::Matrix<4> *view,Maths::Matrix<4> *projection,bool setLine =true);
        void cleanup();
    }
}
#endif
