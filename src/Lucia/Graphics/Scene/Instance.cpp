#include "Lucia/Graphics/Scene/Instance.h"
#include <Lucia\Graphics\Scene\Object.h>
using namespace Lucia;
typedef Graphics::Scene::Instance Instance;
using namespace Graphics;
using namespace Graphics::Scene;
namespace Lucia {
namespace Graphics
{
    namespace _Shaders
    {
        std::shared_ptr<Graphics::Shader> DefaultSceneShader = std::make_shared<Graphics::Shader>();
    }
}
}
Instance::Instance()
{
    if ( !Graphics::_Shaders::DefaultSceneShader->isBuilt() )
    {
        Graphics::_Shaders::DefaultSceneShader->fromFile("assets/Shaders/Shader.vert","assets/Shaders/Shader.frag","DefaultScene");
        Graphics::_Shaders::DefaultSceneShader->addVar("position",0,3);
        Graphics::_Shaders::DefaultSceneShader->addVar("texCoord2",3,5);
    };
    //ctor
}
std::shared_ptr<Container> Instance::add(std::shared_ptr<Object> o)
{
    std::shared_ptr<Container> no = std::make_shared<Container>(o);
    insertTasks(o.get(),no);
    return no;
};
void Instance::insert(std::shared_ptr<Container> o)
{
    insertTasks(o->getCore(),o);
    o->setScene(this);
}
void Instance::remove(Container* subject)
{
    Utils::erase_if(&Objects,[subject](std::shared_ptr<Container> target){return (target.get() == subject);});
    needsRefresh = true;
};
void Instance::sort(SORTMODE m)
{
//    for (auto v: Objects)
//    {
//        v->sort(m);
//    }
}
// Dont forget!!
//1. Draw all opaque objects first.
//2. Sort all the transparent objects.
//3. Draw all the transparent objects in sorted order.
void Instance::drawObject(std::shared_ptr<Container> a,Maths::Matrix<4>* view,Maths::Matrix<4>* projection)
{
    auto Core = a->getCore();
    int id = Core->getShaderVars()->programID;
    if ( id != lastShader){lastShader = id;Core->useShader(view,projection);};
    Core->preDraw();
    a->applyTranslations();
    Core->render(a.get());
}
void Instance::draw(Maths::Matrix<4>* view,Maths::Matrix<4>* projection)
{
    lastShader = -1;
    view->unpack();
    projection->unpack();
    //draw opaque stuff first.
    for (auto v: Objects)
    {
        drawObject(v,view,projection);
    };
    // our stuff is sorted by furtherest away first so we dont need to do anything here.
    // Draw all sorted transparent objects.
    for (auto v: TransparentObjects)
    {
        drawObject(v,view,projection);
    };
}
void Instance::update(double dt)
{
    for (auto v: Objects)
    {
        v->getCore()->update(dt,v.get());
        v->updateInstance();
    }
    for (auto v: TransparentObjects)
    {
        v->getCore()->update(dt,v.get());
        v->updateInstance();
    };
}
void Instance::insertTasks(Object* o, std::shared_ptr<Container> no)
{
    if (o->hasTransparency()){TransparentObjects.push_back(no);}else{Objects.push_back(no);};
    needsRefresh = true;
}
void Instance::refresh()
{
    if (needsRefresh)
    {
        std::sort(TransparentObjects.begin(),TransparentObjects.end(),[](std::shared_ptr<Container>a,std::shared_ptr<Container>b)
          {
            return a->getPosition().z < b->getPosition().z;
          });
        std::sort(Objects.begin(),Objects.end(),[](std::shared_ptr<Container>a,std::shared_ptr<Container>b)
          {
            return a->getCore()->getShaderVars()->programID < b->getCore()->getShaderVars()->programID;
          });
          needsRefresh = false;
    }
}
void Instance::preDraw()
{
    refresh();
    int ProgramID = Graphics::_Shaders::DefaultSceneShader->programID;
    glEnable(GL_DEPTH_TEST);
    glUseProgram(ProgramID);
    GLfloat ticks = SDL_GetTicks()/60;
    ticks = ticks/60;
    GLint u_time = glGetUniformLocation(ProgramID,"time");
    glUniform1f(u_time,ticks);
    
    auto color = getMechanicalColor();
    glClearColor(color.r,color.g,color.b,color.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}
void Instance::postDraw()
{

}
Instance::~Instance()
{
    //dtor
}




