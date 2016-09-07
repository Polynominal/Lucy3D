#ifndef MIKUS_LUCIA_TEXT_SIMPLE_H
#define MIKUS_LUCIA_TEXT_SIMPLE_H
#include <Lucia\Graphics\Text\Text.h>
#include <Lucia\Graphics\Text\Instance.h>
namespace Lucia{
namespace Graphics
{
    class Text::Simple
    {
    public:
        Simple(){};
        virtual ~Simple(){remove();};
        void generate(std::string path_to_font=std::string("assets/Internal/Lucia/console.ttf"),unsigned int fontSize=25,float scw=800,float sch=600);
        void remove();
        Graphics::Text::Render* print = nullptr;
        
    private:
        Matrix<4>* projection=nullptr;
        Graphics::Text* text=nullptr;
        Graphics::Text::Instance* instance = nullptr;
    };
}
}
#endif