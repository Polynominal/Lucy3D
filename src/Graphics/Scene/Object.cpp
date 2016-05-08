#include "Graphics/Scene/Object.h"
using namespace Graphics;
using namespace Graphics::Scene;
namespace Graphics
{
    void Object::preDraw()
    {
        refresh();
        lastShader = 0;
    }
    void Object::updateShader(int id,glm::mat4* view,glm::mat4* projection,glm::mat4* local)
    {
        if (lastShader != id)
        {
            glUniformMatrix4fv(glGetUniformLocation(id,"view"),1,GL_FALSE,glm::value_ptr(*view));
            glUniformMatrix4fv(glGetUniformLocation(id,"projection"),1,GL_FALSE,glm::value_ptr(*projection));
            glUniformMatrix4fv(glGetUniformLocation(id,"model_view"),1,GL_FALSE,glm::value_ptr(Model_Matrix));
            //glUniformMatrix4fv(glGetUniformLocation(id,"local"),1,GL_FALSE,glm::value_ptr(Model_Matrix));
            lastShader = id;
        }
    }
    void Object::draw(glm::mat4* view,glm::mat4* projection)
     {
        preDraw();
        OnDraw();
        for (auto v: Items)
        {
            v->SwapShaders = [this,view,projection](int id,glm::mat4* local)
            {
                updateShader(id,view,projection,local);
            };
            v->draw(false);
        }
        for (auto v: Models)
        {
            v->getShaderVars()->use();
            updateShader(v->getShaderVars()->getID(),view,projection,v->getModelMatrix());
            v->draw();
        }
     };
     void Object::sort(SORTMODE mode)
     {
         switch(mode)
         {
             case SORTMODE::SHADER:
             {
                 std::sort(Items.begin(), Items.end(), [](Mesh* a, Mesh* b)
                       {
                            return a->getShaderVars()->getID() < b->getShaderVars()->getID();
                       });
                std::sort(Models.begin(), Models.end(), [](Model* a, Model* b)
                       {
                            return a->getShaderVars()->getID() < b->getShaderVars()->getID();
                       });
             }
             case SORTMODE::ZMAP:
             {

             }
         }
     }
      void Object::drawShader(Utils::OpenGL::Shader_Vars *Vars)
      {
         preDraw();
          Utils::find_if(&Items,[Vars](Mesh* ptr)
                         {
                            if (Vars == ptr->getShaderVars().get())
                            {
                                ptr->draw(false);
                            }
                            return false;
                         });
      }
}
