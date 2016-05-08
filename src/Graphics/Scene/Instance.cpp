#include "Graphics/Scene/Instance.h"
#include <Graphics\Scene\Object.h>
typedef Graphics::Scene::Instance Instance;
using namespace Graphics;
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
std::shared_ptr<Scene::Object> Instance::insert(Mesh* Mesh,float x,float y,float z)
{
    std::shared_ptr<Scene::Object> scene_obj(new Object());
    scene_obj->addMesh(Mesh);
    scene_obj->moveTo(x,y,z);
    Objects.push_back(scene_obj);
    return scene_obj;
};
std::shared_ptr<Scene::Object> Instance::insert(Model* model,float x,float y,float z)
{
    std::shared_ptr<Scene::Object> scene_obj(new Object());
    scene_obj->addModel(model);
    scene_obj->moveTo(x,y,z);
    Objects.push_back(scene_obj);
    return scene_obj;
};
void Instance::remove(Object* subject)
{
    Utils::erase_if(&Objects,[subject](std::shared_ptr<Object> target){return (target.get() == subject);});
};
void Instance::sort(SORTMODE m)
{
    for (auto v: Objects)
    {
        v->sort(m);
    }
}
void Instance::draw(glm::mat4* view,glm::mat4* projection)
{
    for (auto v: Objects)
    {
        v->draw(view,projection);
    }
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




