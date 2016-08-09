#include <Lucia\Maigui\Bases\Drawable.h>
namespace Lucia {
namespace Maigui
{
    namespace Base
    {
        void Drawable::State::draw(shared_ptr<Skin> skin)
        {
            PreDraw(name);
            if (drawOverride)
            {
                Draw(name);
            }
            else
            {
                if (mode == 2)
                {
                    skin->draw(name,cr,cg,cb,ca);
                }else if (mode == 1)
                {
                    skin->drawColoredRectangle(cr,cg,cb,ca);
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
            drawBackground();
            drawBase();
            drawForeground();
            if (inactive)
            {
                drawInactive();
                
            }else if (hasFocus)
            {
                drawFocus();
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
