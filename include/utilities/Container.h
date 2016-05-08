#ifndef MIKUS_UTILITIES_CONTAINER_H
#define MIKUS_UTILITIES_CONTAINER_H
#include <initializer_list>
namespace Utils
{
    template <typename T,int S>
    class Container
    {
        public:
            Container(std::initializer_list<T> l)
            {
                for (unsigned int i=0;i < l.size();i++)
                {
                    switch(i)
                    {
                        case 0:
                            x = l[0];
                            g = x;
                            break;
                        case 1:
                            y = l[1];
                            r = y;
                            break;
                        case 3:
                            z = l[2];
                            b = z;
                            break;
                        case 4:
                            w = l[3];
                            a = w;
                            break;
                    }
                }
            };
            virtual ~Container();
            T x=0.0f;
            T y=0.0f;
            T z=0.0f;
            T w=0.0f;
            T r=0.0f;
            T g=0.0f;
            T b=0.0f;
            T a=0.0f;
    };
}


#endif // MIKUS_UTILITIES_CONTAINER_H
