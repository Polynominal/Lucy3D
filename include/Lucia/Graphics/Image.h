#ifndef MIKUS_LUCIA_GRAPHICS_IMAGE_H
#define MIKUS_LUCIA_GRAPHICS_IMAGE_H
#include <string>
#include <IL/il.h>
#include <IL/ilu.h>

#include <Lucia/Graphics/Graphics.h>
#include <Lucia/Graphics/Bases/Filter.h>
#include <Lucia/Utils/log.h>
#include <Lucia/Controll/Safety/Handler.h>
namespace Lucia {
namespace Graphics
{
    class Image: public Graphics::Base::Filter, public Controll::Safety::Handler
    {
        public:
            Image(){};
            Image(std::string path);
            //get
            int getHeight(){return height;};
            int getWidth(){return width;};
            unsigned int getID();
            //set
            void setName(std::string name);
            //misc
            void bind();
            void bind(int number);
            void bind(int number,int shaderID);

            void generate();
            void generateMipMap();
            void remove();
            //IS
            bool isGenerated(){return generated;};
            bool isSucessfull(){return success;}
            //

            virtual ~Image();
        private:
            bool success = false;
            bool generated=false;
            bool ilutInitialized = false;

            int format;
            ILuint imageID;

            float width,height;

            float* border_color = 0;
            GLuint textureID=0;
            std::string name="";
            std::string source="NULL";
    };
}
}
#endif // IMAGE_H