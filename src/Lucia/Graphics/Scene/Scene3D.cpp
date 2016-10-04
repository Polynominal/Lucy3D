#include "Lucia\Graphics\Scene\Scene3D.h"
using namespace Lucia;
using namespace Graphics;
using namespace Scene;
Scene3D::Scene3D()
{

}
void Scene3D::generate(Context::Window* Window)
{
    float w = Window->getWidth();
    float h = Window->getHeight();
    cam.reset(new Graphics::Camera(w,h));

    float prc = w/h;
    float prc2 = w/h;
    float actual = prc2/prc;

    //cam->setOrtho();
    cam->setRange(45,0.1f,90000.0f);
    cam->move(0.0f,0.0f,-2.0f);
    //at 45 -540
}
void Scene3D::preDraw()
{
    Instance::preDraw();
    glDepthFunc(GL_LEQUAL);
    cam->update();
}
void Scene3D::draw()
{
    preDraw();
    Instance::draw(&cam->view,&cam->projection);
    postDraw();
}
Scene3D::~Scene3D()
{
    //dtor
}
