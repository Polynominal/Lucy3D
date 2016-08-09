#ifndef MIKUS_LUCIA_GRAPHICS_SCENE_H
#define MIKUS_LUCIA_GRAPHICS_SCENE_H
#include <Lucia/Settings.h>
#include <vector>
#include <map>
#include <Lucia/Maths/Matrix.h>
#include <Lucia/Graphics/Graphics.h>
#include <Lucia/Graphics/Shaders.h>
#include <Lucia/Graphics/Scene/Object.h>

#include <Lucia/Utils/Utils.h>
#include <Lucia/Graphics/Camera.h>

#include <string>
#include <SDL.h>
#include <memory>

#ifdef LUCIA_USE_GLES2
    #include<GLES2/gl2.h>
#endif
namespace Lucia
{
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
                    void draw(Maths::Matrix<4>* view,Maths::Matrix<4>* projection);
                    void update(double dt);
                    void postDraw();

                    void remove(Container* o);
                    void remove(std::shared_ptr<Container> target){remove(target.get());};
                    //set
                    void setFilter(std::string mip, std::string mag );
                    void setWrap(std::string s, std::string m);

                protected:
                    void SetData();

                private:
                    void drawObject(std::shared_ptr<Container> a,Maths::Matrix<4>* view,Maths::Matrix<4>* projection);
                    int lastShader=0;
                    void insertTasks();
                    SORTMODE mode;
                    std::vector<std::shared_ptr<Container>> Objects;
                    std::vector<std::shared_ptr<Container>> TransparentObjects;
                    Camera* default_camera;
            };
        }
        class Shader;
        namespace _Shaders
        {
            extern std::shared_ptr<Graphics::Shader> DefaultSceneShader;
        }
    }
}
#endif // Scene_H
