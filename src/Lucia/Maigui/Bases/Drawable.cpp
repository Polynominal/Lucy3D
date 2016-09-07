#include <Lucia\Maigui\Bases\Drawable.h>
namespace Lucia {
namespace Maigui
{
    namespace Base
    {
        void Drawable::State::draw(shared_ptr<Skin> skin,Graphics::Image* img,Maths::Sprite* sprite)
        {
            PreDraw(name);
            auto c = getMechanicalColor();
            if (drawOverride)
            {
                Draw(name);
            }
            else
            {
                if (mode == 2)
                {
                    if (img != nullptr)
                    {
                        skin->drawImageItem(img,sprite,c.r,c.g,c.b,c.a);
                    }    
                    else if(sprite != nullptr)
                    {
                        skin->draw(sprite,c.r,c.g,c.b,c.a);
                    }else
                    {
                        skin->draw(name,c.r,c.g,c.b,c.a);
                    }
                }else if (mode == 1)
                {
                    skin->drawColoredRectangle(c.r,c.g,c.b,c.a);
                }else
                {
                    skin->DrawState(name,enumref);
                }
            }
            PostDraw(name);
        };
        void Drawable::draw(Matrix<4> Mat,bool hasFocus,bool inactive)
        {
            skin->setMatrix(Mat);
            drawBackground(image.get(),sprite.get());
            drawBase(image.get(),sprite.get());
            drawForeground(image.get(),sprite.get());
            if (inactive)
            {
                drawInactive(image.get(),sprite.get());
                
            }else if (hasFocus)
            {
                drawFocus(image.get(),sprite.get());
            }
        };
        void Drawable::setDrawState(DrawState state,float r,float g,float b,float a)
        {
            auto s = DrawStates[state];
            s->mode = 1;
            s->setColor(r,g,b,a);
        };
        void Drawable::setDrawState(DrawState state,string Name,float r,float g,float b,float a)
        {
            auto s = DrawStates[state];
            s->mode = 2;
            s->name = Name;
            s->setColor(r,g,b,a);
        };
    }
}
}
