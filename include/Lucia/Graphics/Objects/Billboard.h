#ifndef LUCIA_GRAPHICS_OBJECTS_BILBOARD
#define LUCIA_GRAPHICS_OBJECTS_BILBOARD
#include <Lucia/Graphics/Scene/Object.h>
#include <Lucia/Utils/OpenGL.h>
#include <Lucia/Graphics/Bases/Texture.h>

namespace Lucia{
namespace Graphics{
namespace Object{
    
    typedef Utils::OpenGL::Buffer Buff;
    typedef Utils::OpenGL::Shader_Vars Shader_Vars;
    class Billboard:
    public Scene::Object
    {
        enum MODE
        {
            STATIC,
            DYNAMIC
        };
        public:
            
            Billboard();
            virtual ~Billboard(){};
            
            //SET
            void setMode(MODE m){mode = m;};
            void setAxis(bool x,bool y,bool z){axis = std::tuple<bool,bool,bool>(x,y,z);};
            void scaleToPixels(float w,float h,float screen_w,float screen_h,float original_fraction = 0.5f);
            void setView(Matrix<4>* view);
            void setTexture(std::shared_ptr<Graphics::Base::Texture> t){image = t;};
            void setStatic(bool t);
            //GET
            std::tuple<bool,bool,bool> getAxis(){return axis;};
            MODE getMode(){return mode;}
            //Inherited            
            virtual void render(Scene::Container* c,DRAW mode);
            
            private:
            float pixelScale[2]{1.0f,1.0f};
            std::shared_ptr<Graphics::Base::Texture> image = nullptr;
            Matrix<4>* view;
            MODE mode = MODE::DYNAMIC;
            std::tuple<bool,bool,bool> axis = std::tuple<bool,bool,bool>(true,true,true);
    };
 
}}}
#endif 