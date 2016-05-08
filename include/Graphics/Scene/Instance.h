#ifndef MIKUS_GRAPHICS_SCENE_H
#define MIKUS_GRAPHICS_SCENE_H
#include <Settings.h>
#include <vector>
#include <map>

#include <Graphics\Graphics.h>
#include <Graphics\Shaders.h>
#include <Graphics\Buffers\Mesh.h>
#include <Graphics\Scene\Model.h>
#include <Graphics\Scene\Object.h>
#include <utilities\Utils.h>
#include <Image.h>
#include <Camera.h>

#include <string>
#include <SDL.h>
#include <memory>

#ifdef USE_GLES2
    #include<GLES2/gl2.h>
#endif
namespace Graphics
{
    namespace Scene
    {
        class Instance
        {
            public:
                Instance();
                virtual ~Instance();

                std::shared_ptr<Object> insert(Mesh* m,float x=0,float y=0,float z=0);
                std::shared_ptr<Object> insert(Model* m,float x=0,float y=0,float z=0);
                void insert(Object* o);

                void sort(SORTMODE mode);
                void preDraw();
                void draw(glm::mat4* view,glm::mat4* projection);
                void postDraw();

                void remove(Object* o);
                void remove(std::shared_ptr<Object> target){remove(target.get());};

                void setFilter(std::string mip, std::string mag );
                void setWrap(std::string s, std::string m);

            protected:
                void SetData();

            private:
                std::vector<std::shared_ptr<Object>> Objects;
                Camera* default_camera;

        };
    }
}
namespace Graphics
{
    class Shader;
    namespace _Shaders
    {
        extern std::shared_ptr<Graphics::Shader> DefaultSceneShader;
    }
}
#endif // Scene_H
