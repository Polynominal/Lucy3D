#ifndef MIKUS_LUCIA_GRAPHICS_CANVAS_H
#define MIKUS_LUCIA_GRAPHICS_CANVAS_H
#include <Lucia/Graphics/Graphics.h>
#include <Lucia/Graphics/Shaders.h>
#ifdef LUCIA_USE_GLES2
#include <Lucia/Compatability/GLES2/Shaders/Blit2D.h>
#endif
#include <Lucia/Graphics/Bases/Texture.h>
namespace Lucia {
namespace Graphics{
namespace Buffer{
    
    class Canvas:
    public Lucia::Graphics::Base::Texture
    {
         public:
            Canvas(){};
            virtual ~Canvas(){remove();};
            //overloaded
            #ifndef LUCIA_USE_GLES2
            virtual ImageData getImageData()
            {
                GLubyte data = GLubyte();
                attach();
                glReadPixels(0,0,width,height,GL_RGBA,GL_UNSIGNED_BYTE,&data);
                detach();
                return ImageData(reinterpret_cast<const char*>(data),width,height);
            };
            #endif
            //
            bool generate(float width=0,float height=0);
            void clear();
            void remove();
            void attach(bool clear=false,bool clearColor=false);
            void detach(int nbuffer=1.0f);
            void resize(float w,float h,bool keepAspect=false);
            std::unique_ptr<Graphics::Buffer::Canvas> getBlitSize(float w,float h,bool keepAspect=false);
            bool isDone(){return !passive;};
            //set
            void setClearColor(float r=0.0f,float g=0.0f,float b=0.0f,float a=0.0f){Color[0]=r;Color[1]=g;Color[2]=b;Color[3]=a;};
            //
        protected:
        private:
            bool passive=true;
            GLuint FBO=0;
            GLuint RBO=0;

            float Color[4]{0.0f,0.0f,0.0f,0.0f};
    };
}}}
#endif // MIKUS_GRAPHICS_CANVAS_H
