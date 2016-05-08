#ifndef MIKUS_SCENE_OBJECT_H
#define MIKUS_SCENE_OBJECT_H

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include <Graphics\Graphics.h>

#include <Graphics\Buffers\Mesh.h>
#include <Graphics\Scene\Model.h>

#include <Graphics\Bases\Moveable.h>
#include <Graphics\Scene\Instance.h>
#include <utilities\OpenGL.h>
#include <algorithm>

namespace Graphics
{
    namespace Scene
    {
        class Object: public Graphics::Base::Moveable
        {
            public:
                Object(){};
                //set
                void setScene(Graphics::Scene::Instance* s){Core = s;};
                //adds
                void addMesh(Mesh* mesh){Items.push_back(mesh);};
                void addModel(Model* model){Models.push_back(model);};
                //gets
                std::vector<Mesh*> getItems(){return Items;};
                std::vector<Model*> getModels(){return Models;};
                // updates the shader use it alongside Mesh.
                void updateShader(int id,glm::mat4* view,glm::mat4* projection,glm::mat4* local);
                // draws all the meshes with the same shader.
                void drawShader(Utils::OpenGL::Shader_Vars *Vars);
                // draws on random.
                void preDraw();
                void draw(glm::mat4* view,glm::mat4* projection);
                void sort(SORTMODE mode);

                void remove();

                virtual ~Object(){};

            private:
                int lastShader=0;
                std::function<void()> OnDraw=[](){};
                Graphics::Scene::Instance* Core=nullptr;
                std::vector<Mesh*> Items;
                std::vector<Model*> Models;
        };
    }
}


#endif // SCENE_OBJECT_H
