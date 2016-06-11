#ifndef MIKUS_GRAPHICS_GLBUFFER_H
#define MIKUS_GRAPHICS_GLBUFFER_H
#include <vector>
#include <string>
#include <iostream>

//GLM includes used for transformation maths.
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include <Graphics/Graphics.h>
#include <Graphics/Image.h>

#include <Graphics/Scene/Object.h>

#include <utilities/OpenGL.h>
#include <utilities/Log.h>

namespace Graphics
{
    typedef Utils::OpenGL::Buffer Buff;
    typedef Utils::OpenGL::Shader_Vars Shader_Vars;

    class Mesh: public Graphics::Scene::Object
    {
        public:
            Mesh();

            void generate();

            void addImage(Image img);
            void addSubBuffer(Buff* UBuffer);
            Buff* addSubBuffer();
            void addSubMesh(Mesh* m);
            // path to the file
            void addImage(std::string path);
            void addImage(std::string path,std::string reference_name);
            void addImage(Image img,std::string reference_name);
            //misc
            std::function<void(int,glm::mat4*)> SwapShaders = [](int id,glm::mat4* t){};
            void finalize();
            virtual void render(glm::mat4* model,DRAW mode); // rendering assuming that everything is bound correctly.
            //sets
            Buff*  getSubBuffer(int id=0){return Cores[id];};
            Image* getImage(int id=0){return Images[id];};

            void destroy(bool use_delete=false);

            virtual ~Mesh();
        private:
            std::vector<Mesh*>  Children;
            std::vector<Buff*>  Cores;
            std::vector<Image*> Images;
            bool cleanTexture = false;
    };
}


#endif // GLBUFFER_H
