#ifndef MIKUS_LUCIA_GRAPHICS_PRIMITIVES_QUAD_H
#define MIKUS_LUCIA_GRAPHICS_PRIMITIVES_QUAD_H
#include <Lucia/Graphics/Graphics.h>
#include <Lucia/Graphics/Shaders.h>
#include <Lucia/Utils/OpenGL.h>
namespace Lucia {
namespace Graphics
{
    namespace Primitive
    {
        typedef Utils::OpenGL::Vertex_Buffer Vertex_Buffer;
        typedef Utils::OpenGL::Buffer Buffer;
        typedef Utils::OpenGL::Shader_Vars Shader_Vars;
        class Quad
        {
            public:
                Quad();
                bool isDone(){return init;};
                //set
                void setMode(bool centered){centeredCordinates = centered;}
                //get 
                Buffer* getBuffer(){return Buff.get();};

                void generate(float width,float height,float sw=0.0f,float sh=0.0f,
                float bottomx=0.0f,float bottomy=0.0f, float topx=1.0f,float topy=1.0f);
                void generate(unsigned int id =0);
                void draw();
                void draw(std::shared_ptr<Shader_Vars> vars);
                void remove();
                void setImageRange(float bottomx=0,float bottomy=0,float topx=1,float topy=1);
                virtual ~Quad();
                
            protected:
            private:
                std::vector<std::shared_ptr<Vertex_Buffer>> Data;
                void initTasks(int id);
                float gw;
                float gh;
                bool centeredCordinates = true;
                bool init = false;
                std::unique_ptr<Buffer> Buff;
        };
    }
    namespace Shared
    {
        extern std::unique_ptr<Primitive::Quad> Quad;
    }
}
}
#endif // MIKUS_GRAPHICS_PRIMITIVES_QUAD_H
