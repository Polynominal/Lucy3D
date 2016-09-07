#ifndef MIKUS_LUCIA_GRAPHICS_TOOLS_SPRITESHEET_H
#define MIKUS_LUCIA_GRAPHICS_TOOLS_SPRITESHEET_H

#include <memory>

#include <Lucia/Graphics/Image.h>
#include <Lucia/Graphics/Scene/Object.h>
#include <Lucia/Graphics/Shaders.h>
#include <Lucia/Graphics/Bases/ShaderHolder.h>

#include <Lucia/Utils/tween.h>
#include <Lucia/Utils/logistics.h>
#include <Lucia/Utils/OpenGL.h>
#include <Lucia/Graphics/Primitives/Quad.h>

#include <Lucia/Maths/Vec2.h>
#include <Lucia/Maths/SpriteSheet.h>

namespace Lucia {
namespace Graphics
{
    namespace _Shaders
    {
        extern std::unique_ptr<Graphics::Shader> VaryingUV;
    };
    namespace Object
    {
        class Sprite;
        class Animation;
    };
    namespace Buffer
    {
        class Spritesheet: public std::enable_shared_from_this<Spritesheet>, public Graphics::Base::ShaderHolder
        {
            public:
                Spritesheet();
                //set
                void setFrameDimensions(float width,float height){defaultWidth = width;defaultHeight = height;};
                //
                void load(std::string file_path);
                void useShader(Maths::Matrix<4>* view,Maths::Matrix<4>* projection);
                void preDraw();
                std::shared_ptr<Object::Sprite> create(float x,float y,float width=-1,float height=-1);
                std::shared_ptr<Object::Animation> newAnimation(unsigned int startFrame,unsigned int finalFrame=0,float speed=1,float ax=0,float ay=0,float w=-1,float h=-1);
                //get
                std::shared_ptr<Image> getImage(){return image;};
                Vec2 getFrameDimensions(){return Vec2(defaultWidth,defaultHeight);};
                Vec2 getDimensions(){return Vec2(image->getWidth(),image->getHeight());};
                //set

                //
                virtual ~Spritesheet();


            private:
                float defaultWidth = 0;
                float defaultHeight = 0;
                std::shared_ptr<Image> image;
        };
    }
}
}
#endif // MIKUS_GRAPHICS_TOOLS_SPRITESHEET_H
