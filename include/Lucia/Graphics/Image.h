#ifndef MIKUS_LUCIA_GRAPHICS_IMAGE_H
#define MIKUS_LUCIA_GRAPHICS_IMAGE_H
#include <string>
#include <IL/il.h>
#include <IL/ilu.h>

#include <Lucia/Graphics/Graphics.h>
#include <Lucia/Graphics/Bases/Filter.h>
#include <Lucia/Graphics/Bases/Texture.h>
#include <Lucia/Utils/log.h>
#include <Lucia/Controll/Safety/Handler.h>
#ifdef LUCIA_USE_GLES2
    #include <Lucia/Graphics/Buffers/Canvas.h>
#endif

namespace Lucia {
namespace Graphics
{
    class Image: public Controll::Safety::Handler, public Graphics::Base::Texture
    {
        public:
            Image(){};
            Image(std::string path);
            //set
            void generate(ImageData* data);
            void generate();
            void remove();
            //GET 
            std::string getSource(){return source;};
            //TODO Please use a new method for get image data
            #ifdef LUCIA_USE_GLES2
            virtual ImageData getImageData()
            {
                Canvas target = Canvas();
                target.generate(width,height);
                target.setClearColor(0,0,0,0);
                target.attach(true,true);
                renderQuad();
                target.detach();
                return target.getImageData();
            };
            #endif
            //IS
            bool isGenerated(){return generated;};
            bool isSucessfull(){return success;}
            //
            
            virtual ~Image();
        private:
            bool success = false;
            bool generated=false;
            bool ilutInitialized = false;
            ILuint imageID;

            float* border_color = 0;
            
            std::string source="NULL";
    };
}
}
#endif // IMAGE_H
