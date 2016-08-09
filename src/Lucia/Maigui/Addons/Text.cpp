#include "Lucia\Maigui\Addons\Text.h"
using namespace Lucia;
typedef Maigui::Addon::Text Text;
typedef Maigui::Vertex Vertex;
Text::Text()
{

}
void Text::generate(Vertex position,Vertex dimensions,shared_ptr<Skin> sk)
{
    Position    = position;
    Dimensions  = dimensions;
    skin        = sk;
    //ctor
};
float Text::getHeight()
{
    return height;
}
float Text::getWidth()
{
    return width;
}
void Text::setSize(int s)
{
    size = s;
    Vertex dimensions = Vertex(skin->getTextWidth(text,s),(skin->getFontHeight(s))*getLines(),Dimensions.z);
    scaleTo(dimensions);
}
int Text::getLines()
{
    int no =1;
    for (auto v: text)
    {
        if (v == '\n'){no++;};
    }
    return no;
}
int Text::getMaxLineLenght()
{
    int maxLenght=0;
    int lenght = 0;
    for (auto v: text)
    {
        if (v == '\n'){lenght = 0;}else{lenght++;};
        maxLenght = std::max(maxLenght,lenght);
    }
    return maxLenght;
}
void Text::remove()
{
    if (id != -1){skin->delBuffer(id);};
}
void Text::setSkin(std::shared_ptr<Maigui::Skin> s)
{
    skin=s;
}
void Text::setText(string T)
{
    remove();
    text = T;
    id = skin->bufferText(T);
    width = skin->getTextWidth(text,size);
    height = skin->getFontHeight(size);
    setSize(size);
};
void Text::draw()
{
    if (id == -1){setText(text);};
    skin->setTextColor(cr,cg,cb,ca);
    skin->printBufferedText(id,Translation);
};
Text::~Text()
{

    //dtor
}
