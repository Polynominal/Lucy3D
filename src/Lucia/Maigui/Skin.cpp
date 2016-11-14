#include <Lucia/Maigui/Skin.h>
#include <Lucia/Maigui/Item.h>
using namespace Lucia;
using namespace Maigui;
Skin::Skin()
{

}
void Skin::setPath(string path_to_image)
{
    ImageData = std::make_shared<Graphics::Image>(path_to_image);
    ImageData->setFilter(Graphics::NEAREST);
    ImageData->generate();
    Dimensions = Vec2(ImageData->getWidth(),ImageData->getHeight());
}
void Skin::draw(Maths::Sprite* s,float r,float g,float b,float a)
{
    auto d = Drawable();
    d.create(s->x,s->y,s->w,s->h,Dimensions.x,Dimensions.y);
    drawColoredItem(&d,r,g,b,a);
};
void Skin::draw(string item,float r,float g,float b,float a)
{
    auto i = Items.find(item);
    if (i != Items.end())
    {
        Drawable d = i->second;
        drawColoredItem(&d,r,g,b,a);
        
        
    }else{LOG << "Warning" << "Couldnt find item by name: " << item << std::endl;};
};
Vec2 Skin::toMechanical(float nx,float ny)
{
    float x = nx/Dimensions.x;
    float y = (Dimensions.y - ny);
    y = y/Dimensions.y;
    return Vec2(x,y);
}
Skin::Drawable Skin::form(string item,float x,float y,float w,float h)
{
    auto d = Drawable();
    d.create(x,y,w,h,Dimensions.x,Dimensions.y);
    Items.insert({item,d});
    return d;
}
Skin::Drawable Skin::form(string item,Skin::Shapes type,float x,float y,float w,float h)
{
    // U and V are in machine cords in other words percentages. Hence we need to:
    // 1. find the percentage for each x and y.
    auto d = Drawable();
    d.create(x,y,w,h);
    d.type = type;
    Items.insert({item,d});
    return d;
}
Skin::~Skin()
{

}
namespace Lucia {
namespace Maigui{
    namespace Default
    {
        bool formed = false;
        std::shared_ptr<Maigui::Skin> Skin = std::make_shared<Maigui::Skin>();
        void form()
        {
            if (!formed)
            {
                Skin->DrawState = [](string name,int state)
                {
                    switch(state)
                    {
                        case Item::FOCUS:
                        {
                            Skin->drawColoredRectangle(1.0f,1.0f,1.0f,0.5f);
                            break;
                        }
                        case Item::INACTIVE:
                        {
                            Skin->drawColoredRectangle(0.0f,0.0f,0.0f,0.3f);
                            break;
                        }
                    }
                };
                Skin->setPath("assets/Maigui/default.png");

                Skin->form("Container",5,265,26,26);
                Skin->form("Item",5,265,26,26);
                Skin->form("Frame",5,265,26,26);
                Skin->form("Plate",305,152,154,153);
                
                Skin->form("Titlebar",Skin::Shapes::Rectangle,255,0,0,255);
                Skin->form("Button",Skin::Shapes::Rectangle,0,0,0,255);
                Skin->form("Button1",Skin::Shapes::Rectangle,255,0,0,255);
                formed = true;
            }
            Skin->setFont("assets/fonts/west_england.ttf");
        };
    }
}
}
