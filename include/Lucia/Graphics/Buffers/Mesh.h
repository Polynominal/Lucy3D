#ifndef MIKUS_GRAPHICS_GLBUFFER_H
#define MIKUS_GRAPHICS_GLBUFFER_H
#include <vector>
#include <string>
#include <iostream>

#include <Lucia/Maths/Matrix.h>

#include <Lucia/Graphics/Graphics.h>
#include <Lucia/Graphics/Image.h>

#include <Lucia/Graphics/Scene/Object.h>

#include <Lucia/Utils/OpenGL.h>
#include <Lucia/Utils/Log.h>
namespace Lucia {
namespace Graphics
{
    using namespace Utils::OpenGL;
    class Mesh: public Graphics::Scene::Object
    {
        public:
            Mesh();

            void generate();

            void addImage(Image img);
            void addSubBuffer(Utils::OpenGL::Buffer* UBuffer);
            Utils::OpenGL::Buffer* addSubBuffer();
            void addSubMesh(Mesh* m);
            // path to the file
            void addImage(std::string path);
            void addImage(std::string path,std::string reference_name);
            void addImage(Image img,std::string reference_name);
            //misc
            std::function<void(int,Maths::Matrix<4>*)> SwapShaders = [](int id,Maths::Matrix<4>* t){};
            void finalize();
            virtual void render(Maths::Matrix<4>* model,DRAW mode); // rendering assuming that everything is bound correctly.
            //sets
            Utils::OpenGL::Buffer*  getSubBuffer(int id=0){return Cores[id];};
            Image* getImage(int id=0){return Images[id];};

            void destroy(bool use_delete=false);

            virtual ~Mesh();
        private:
            std::vector<Mesh*>  Children;
            std::vector<Utils::OpenGL::Buffer*>  Cores;
            std::vector<Image*> Images;
            bool cleanTexture = false;
    };
}
}

#endif // GLBUFFER_H
