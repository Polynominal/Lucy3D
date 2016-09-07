#ifndef MIKUS_LUCIA_MAIGUI_SKIN_H_INCLUDED
#define MIKUS_LUCIA_MAIGUI_SKIN_H_INCLUDED
#include <Lucia/Maigui/Maigui.h>
#include <Lucia/Graphics/Image.h>
#include <Lucia/Maths/SpriteSheet.h>

namespace Lucia {
class Maigui::Skin
{
    public:
        // create and finalize the image here.
        Skin();
        enum Shapes
        {
            None,
            Rectangle
        };
        enum DrawModes
        {
            FILL,
            LINE
        };
        void setPath(string path_to_image);
        class Drawable: // used for a quad
        public Lucia::Maths::Sprite
        {
            public:
                Drawable(){};
                virtual ~Drawable(){};
                Shapes type = Shapes::None;
                uint id; // id for shaders and so on.
        };
        // item reference
        Vec2 toMechanical(float nx,float ny);
        Drawable form(string item,float x,float y,float w,float h);// make a quad
        Drawable form(string item,Shapes shape_type=Shapes::Rectangle,float r=1.0f,float g=0.0f,float b=0.0f,float a=0.0f);// make a quad
        void draw(string item,float r=1.0f,float g=1.0f,float b=1.0f,float a=1.0f);
        void draw(Maths::Sprite* s,float r=1.0f,float g=1.0f,float b=1.0f,float a=1.0f);
        
        void clear();
        std::shared_ptr<Graphics::Image> getData(){return ImageData;};

        virtual ~Skin();

//////////////////
        Vec2 Dimensions;
        std::map<string,Drawable> Items;

        std::function<void()>                                                   PreDraw  = [](){};
        std::function<void()>                                                   PostDraw = [](){};
        std::function<void(Matrix<4>)>                                          setMatrix= [](Matrix<4> translation){};
        std::function<void(Graphics::Image*,Maths::Sprite*,float,float,float,float)>                    drawImageItem=[](Graphics::Image* img,Maths::Sprite* s,float r,float g,float b,float a){};
        std::function<void(Drawable*)>                                          drawItem = [](Drawable *d){};
        std::function<void(Drawable*,float,float,float,float)>                  drawColoredItem = [](Drawable *d,float r,float g,float b,float a){};
        std::function<void(float,float,float,float)>                            drawColoredRectangle =[](float r,float g,float b,float a){};

        std::function<int(string)>                                              bufferText= [](string name){return -1;};
        std::function<void(int)>                                                delBuffer = [](int id){};
        std::function<void(string,Matrix<4>)>                                   printText = [](string text,Matrix<4>translation){};
        std::function<void(int,Matrix<4>)>                                      printBufferedText = [](int id,Matrix<4>translation){};
        std::function<void(string)>                                             setFont = [](string path){};
        std::function<void(float,float,float,float)>                            setTextColor  = [](float r,float g,float b,float a=1.0f){};
        std::function<float(string,int)>                                        getTextWidth  = [](string t,int size){return 0.0f;};
        std::function<float(int)>                                               getFontHeight = [](int size){return 0.0f;};

        std::function<void(string,int)>                                         DrawState = [](string s,int state){};

        std::shared_ptr<Graphics::Image> ImageData;
};
}
#endif // MIKUS_MAIGUI_SKIN_H_INCLUDED
