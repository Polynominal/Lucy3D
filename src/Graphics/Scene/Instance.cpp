#include "Graphics/Scene/Instance.h"
#include <Graphics\Scene\Object.h>
typedef Graphics::Scene::Instance Instance;
using namespace Graphics;
using namespace Graphics::Scene;
namespace Graphics
{
    namespace _Shaders
    {
        std::shared_ptr<Graphics::Shader> DefaultSceneShader = std::make_shared<Graphics::Shader>();
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
std::shared_ptr<Container> Instance::addObject(std::shared_ptr<Object> o)
{
    std::shared_ptr<Container> no = std::make_shared<Container>(o);
    if (o->hasTransparency()){TransparentObjects.push_back(no);}else{Objects.push_back(no);};
    insertTasks();
    return no;
};
void Instance::remove(Container* subject)
{
    Utils::erase_if(&Objects,[subject](std::shared_ptr<Container> target){return (target.get() == subject);});
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
void Instance::drawObject(std::shared_ptr<Container> a,glm::mat4* view,glm::mat4* projection)
{
    auto Core = a->getCore();
    int id = Core->getShaderVars()->programID;
    if ( id != lastShader){lastShader = id;Core->useShader(view,projection);};
    Core->preDraw();
    a->applyTranslations();
    Core->render(a->getModelMatrix());
}
void Instance::draw(glm::mat4* view,glm::mat4* projection)
{
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
void Instance::insertTasks()
{
    std::sort(TransparentObjects.begin(),TransparentObjects.end(),[](std::shared_ptr<Container>a,std::shared_ptr<Container>b)
      {
        return a->getPosition().z < b->getPosition().z;
      });
    std::sort(Objects.begin(),Objects.end(),[](std::shared_ptr<Container>a,std::shared_ptr<Container>b)
      {
        return a->getCore()->getShaderVars()->programID < b->getCore()->getShaderVars()->programID;
      });
}
void Instance::preDraw()
{
    int ProgramID = Graphics::_Shaders::DefaultSceneShader->programID;
    glEnable(GL_DEPTH_TEST);
    glUseProgram(ProgramID);
    GLfloat ticks = SDL_GetTicks()/60;
    ticks = ticks/60;
    GLint u_time = glGetUniformLocation(ProgramID,"time");
    glUniform1f(u_time,ticks);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}
void Instance::postDraw()
{

}
Instance::~Instance()
{
    //dtor
}




