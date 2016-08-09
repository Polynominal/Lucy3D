#include "Lucia\Utils\tween.h"
namespace Lucia {
namespace Utils
{

    std::map<std::string,std::function<double(double)>> TweenModes =
    {
        {"Linear", [](double x){return x;}},
        {"Quad",   [](double x){return x*x;}},
        {"Cubic",  [](double x){return x*x*x;}},
        {"Quint",  [](double x){return x*x*x*x;}},
        {"Sin",    [](double x){return sin(x);}},
        {"Cos",    [](double x){return cos(x);}},
        {"Tan",    [](double x){return tan(x);}}
    };
}
}
