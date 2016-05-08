#ifndef MIKUS_MAIGUI_H
#define MIKUS_MAIGUI_H
#include <Collider/Collider.h>
#include <Collider/Shape/Ray.h>
namespace Maigui
{
    typedef unsigned int uint;
    struct Vec2
    {
        Vec2(){};
        Vec2(float xt,float yt): x(xt),y(yt){};
        float x;
        float y;
    };
    // here we have a multiple inheritance:
    /*
    *                   Item [Abstract base class]
    *                    |
    *           +--------+-------+
    *           |        |       |
    *       Container  Widget   Abstract
    *           |        |           |
    *     --+---+-      -+----+     -+ Tooltip
    *       |   |        |    |
    *     Frame Plate  Button Slider
    */
    using Collider::Shape;
    using Collider::Vertex;
    using namespace Collider::Tools;
    enum mouseButton{
        left,
        right,
        middle
    };
    class Skin;

    class Manager;
    class Item; // abstract class for internal use only.

    class Container;
    class Widget;
    class Abstract;

    class Button;

    namespace Widgets
    {
        class Button;
        class Slider;
    };
    namespace Containers
    {
        class Frame;
        class Plate;
    };
    namespace Internal
    {
        class Tooltip;
    };
    namespace Default
    {
        extern shared_ptr<Skin> Skin;
        extern bool formed;
        void form();
    }
    //settings
    namespace Sizes
    {
        extern float FrameBorder;
    }
};
#endif // MIKUS_MAIGUI_H
