#ifndef LUCIA_MATHS_VEC2
#define LUCIA_MATHS_VEC2
namespace Lucia
{
    namespace Maths
    {
        struct Vec2
        {
            Vec2(){};
            Vec2(float xt,float yt): x(xt),y(yt){};
            float x;
            float y;
        };
        template <typename T>
        class vec2
        {
            vec2(){};
            vec2(T xt,float yt): x(xt),y(yt){};
            float x;
            float y;
        };
    }
}
#endif