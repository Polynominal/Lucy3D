#include "Lucia\Graphics\Scene\Scene2D.h"
using namespace Lucia;
using namespace Graphics;
using namespace Scene;
Scene2D::Scene2D()
{

}
void Scene2D::generate(Context::Window* Window)
{
    float w = Window->getWidth();
    float h = Window->getHeight();
    cam.reset(new Graphics::Camera(800,600));

    float prc = 800.0f/600.0f;
    float prc2 = w/h;
    float actual = prc2/prc;

    //cam->setOrtho();
    cam->setRange(45,0.1f,90000.0f);
    cam->move(0.0f,0.0f,-2.0f);
    //at 45 -540
}
void Scene2D::preDraw()
{
    Instance::preDraw();
    cam->update();
}
void Scene2D::draw()
{
    preDraw();
    Instance::draw(&cam->view,&cam->projection);
    postDraw();
}
Scene2D::~Scene2D()
{
    //dtor
}
