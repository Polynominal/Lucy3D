#ifndef MIKUS_MAIGUI_SKIN_H_INCLUDED
#define MIKUS_MAIGUI_SKIN_H_INCLUDED
#include <Maigui/Maigui.h>
#include <Graphics/Image.h>
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
        struct Drawable // used for a quad
        {
            // x and y from the top
            // w and h of the size of the quad
            float x;
            float y;
            float w;
            float h;
            Shapes type = Shapes::None;
            uint id; // id for shaders and so on.
            // machine cords
            Vec2 top = Vec2(0,0);
            Vec2 bottom = Vec2(0,0);
            //
            const char* Vertex_s = nullptr;
            const char* Vertex_f = nullptr;
            //
            Drawable(float nx,float ny,float nw,float nh) : x(nx),y(ny),w(nw),h(nh){};
        };
        // item reference
        Vec2 toMechanical(float nx,float ny);
        Drawable form(string item,float x,float y,float w,float h);// make a quad
        Drawable form(string item,Shapes shape_type=Shapes::Rectangle,float r=1.0f,float g=0.0f,float b=0.0f,float a=0.0f);// make a quad
        void draw(string item,float r=1.0f,float g=1.0f,float b=1.0f,float a=1.0f);

        void clear();
        Graphics::Image* getData(){return ImageData;};

        virtual ~Skin();

//////////////////
        Vec2 Dimensions;
        std::map<string,Drawable> Items;

        std::function<void()>                                                   PreDraw  = [](){};
        std::function<void()>                                                   PostDraw = [](){};
        std::function<void(Collider::Tools::Matrix<4>)>                         setMatrix= [](Collider::Tools::Matrix<4> translation){};
        std::function<void(Drawable*)>                                          drawItem = [](Drawable *d){};
        std::function<void(Drawable*,float,float,float,float)>                  drawColoredItem = [](Drawable *d,float r,float g,float b,float a){};
        std::function<void(float,float,float,float)>                            drawColoredRectangle =[](float r,float g,float b,float a){};

        std::function<int(string)>                                              bufferText= [](string name){return -1;};
        std::function<void(int)>                                                delBuffer = [](int id){};
        std::function<void(string,Collider::Tools::Matrix<4>)>                  printText = [](string text,Collider::Tools::Matrix<4>translation){};
        std::function<void(int,Collider::Tools::Matrix<4>)>                     printBufferedText = [](int id,Collider::Tools::Matrix<4>translation){};
        std::function<void(string)>                                             setFont = [](string path){};
        std::function<void(float,float,float,float)>                            setTextColor  = [](float r,float g,float b,float a=1.0f){};
        std::function<float(string,int)>                                        getTextWidth  = [](string t,int size){return 0.0f;};
        std::function<float(int)>                                               getFontHeight = [](int size){return 0.0f;};

        std::function<void(string,int)>                                         DrawState = [](string s,int state){};

        Graphics::Image* ImageData;
};

#endif // MIKUS_MAIGUI_SKIN_H_INCLUDED
