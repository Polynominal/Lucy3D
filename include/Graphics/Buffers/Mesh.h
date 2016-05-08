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
#include <Graphics/Scene/Instance.h>
#include <Graphics/Image.h>

#include <Graphics/Bases/Moveable.h>
#include <Graphics/Bases/DrawMode.h>
#include <Graphics/Bases/ShaderHolder.h>

#include <utilities/OpenGL.h>

namespace Graphics
{
    typedef Utils::OpenGL::Buffer Buff;
    typedef Utils::OpenGL::Shader_Vars Shader_Vars;

    class Mesh:

    public Graphics::Base::Moveable,
    public Graphics::Base::DrawMode,
    public Graphics::Base::ShaderHolder

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
            void draw(int programID,bool send_model=false);
            void draw(bool apply_matrix=false);
            //sets
            Buff*  getSubBuffer(int id=0){return Cores[id];};
            Image* getImage(int id=0){return Images[id];};

            void destroy(bool use_delete=false);

            virtual ~Mesh();
        private:
            void render(int mech_ref);
            void render(){render(mechanical_reference);};
            std::vector<Mesh*>  Children;
            std::vector<Buff*>  Cores;
            std::vector<Image*> Images;
            bool cleanTexture = false;
    };
}


#endif // GLBUFFER_H
