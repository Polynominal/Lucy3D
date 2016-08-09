#ifndef MIKUS_LUCIA_GRAPHICS_CANVAS_H
#define MIKUS_LUCIA_GRAPHICS_CANVAS_H
#include <Lucia/Graphics/Graphics.h>
#include <Lucia/Graphics/Shaders.h>
#ifdef LUCIA_USE_GLES2
#include <Lucia/Compatability/GLES2/Shaders/Blit2D.h>
#endif
#include <Lucia/Graphics/Primitives/Quad.h>
namespace Lucia {
namespace Graphics
{
    class Canvas
    {
         public:
            Canvas();

            bool generate(float width=0,float height=0);
            void bindTexture();
            void clear();
            void remove();
            void attach(bool clear=false,bool clearColor=false);
            void detach(int nbuffer=1.0f);
            void resize(float w,float h,bool keepAspect=false);
            std::unique_ptr<Graphics::Canvas> getSize(float w,float h,bool keepAspect=false);
            void setQuad(Graphics::Primitive::Quad* q){Quad = q;};
            void renderQuad(bool ownShader=false,bool resetShader=true);
            GLuint getTextureID(){return texture;};
            bool isDone(){return !passive;};
            // gets
            float getWidth();
            float getHeight();

            void setClearColor(float r=0.0f,float g=0.0f,float b=0.0f,float a=0.0f)
            {Color[0]=r;Color[1]=g;Color[2]=b;Color[3]=a;};

            virtual ~Canvas();
        protected:
        private:

            Graphics::Primitive::Quad* Quad=nullptr;
            bool passive=true;
            GLint lastShader=-1.0f;
            GLuint texture=0;
            GLuint FBO=0;
            GLuint RBO=0;
            float lw;
            float lh;

            float Color[4]{0.0f,0.0f,0.0f,0.0f};
    };
}}
#endif // MIKUS_GRAPHICS_CANVAS_H
