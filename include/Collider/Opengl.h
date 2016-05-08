#include <Settings.h>
#ifndef OPENGL_COLLIDER_WRAPPER_H_INCLUDED
#define OPENGL_COLLIDER_WRAPPER_H_INCLUDED

#include <Collider/Collider.h>
#include <Collider/Manager.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <utilities/OpenGL.h>

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
    void createWrappers(Collider::Manager *M,glm::mat4 *view,glm::mat4 *projection,bool setLine =true);
    void cleanup();
}
#endif
