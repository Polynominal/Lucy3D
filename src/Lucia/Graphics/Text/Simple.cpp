#include "Lucia/Graphics/Text/Simple.h"
using namespace Lucia;
using namespace Graphics;

void Text::Simple::generate(std::string path_to_font,unsigned int fontSize,float width,float height)
{
    remove();
    text = new Graphics::Text(path_to_font);
    text->generate();
   
    projection = new Maths::Ortho(0.0f, width,0.0f,height, 0.01f, 100.0f);
    text->setProjection(projection);

    instance = new Graphics::Text::Instance(text);
    instance->setSize(fontSize);
    instance->setWrapWidth(width);

    print =  new Graphics::Text::Render(instance);
    print->moveTo(0.0f,2.0f - ((fontSize/height)),1.2f);
    print->generate();
};
void Text::Simple::remove()
{
    delete text;text=nullptr;
    delete instance;instance=nullptr;
    delete print; print=nullptr;
};