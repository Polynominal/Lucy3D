#ifndef MIKUS_LUCIA_GRAPHICS_SCENE2D
#define MIKUS_LUCIA_GRAPHICS_SCENE2D
#include <Lucia\Graphics\Scene\Instance.h>
#include <Lucia\Graphics\Camera.h>

namespace Lucia
{
    namespace Graphics
    {
        namespace Scene
        {
            class Scene2D: public Graphics::Scene::Instance
            {
                public:
                    Scene2D();
                    void generate(Context::Window* window);
                    void preDraw();
                    void draw();
                    virtual ~Scene2D();
                    //get
                    Graphics::Camera* getCamera(){return cam.get();};
                    //
                private:
                    std::unique_ptr<Graphics::Camera> cam;
            };
        }
    }
}


#endif // MIKUS_GRAPHICS_SCENE2D
