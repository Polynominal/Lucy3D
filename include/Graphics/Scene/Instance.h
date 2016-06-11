#ifndef MIKUS_GRAPHICS_SCENE_H
#define MIKUS_GRAPHICS_SCENE_H
#include <Settings.h>
#include <vector>
#include <map>

#include <Graphics\Graphics.h>
#include <Graphics\Shaders.h>
#include <Graphics\Scene\Object.h>

#include <utilities\Utils.h>
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
                // add and insert objects.
                std::shared_ptr<Container> addObject(std::shared_ptr<Object> m);
                //

                void sort(SORTMODE mode);
                void preDraw();
                void draw(glm::mat4* view,glm::mat4* projection);
                void postDraw();

                void remove(Container* o);
                void remove(std::shared_ptr<Container> target){remove(target.get());};

                void setFilter(std::string mip, std::string mag );
                void setWrap(std::string s, std::string m);

            protected:
                void SetData();

            private:
                void drawObject(std::shared_ptr<Container> a,glm::mat4* view,glm::mat4* projection);
                int lastShader=0;
                void insertTasks();
                SORTMODE mode;
                std::vector<std::shared_ptr<Container>> Objects;
                std::vector<std::shared_ptr<Container>> TransparentObjects;
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
