#ifndef MIKUS_MAIGUI_BASES_COLOR_H
#define MIKUS_MAIGUI_BASES_COLOR_H
namespace Maigui
{
    namespace Base
    {
        class Color
        {
            public:
                virtual void setColor(int r,int g,int b,int a=255)
                {cr =r/255.0f;cg=g/255.0f;cb=b/255.0f;ca=a/255.0f;};
                virtual void setFloatColor(float r,float g,float b,float a=1.0f){cr=r;cg=g;cb=b;ca=a;};
                virtual void getColor(int* array){
                    array[0] = cr*255;
                    array[1] = cg*255;
                    array[2] = cb*255;
                    array[3] = ca*255;
                };
                virtual void getFloatColor(float* array){
                    array[0] = cr/255;
                    array[1] = cg/255;
                    array[2] = cb/255;
                    array[3] = ca/255;
                };
            protected:
                float cr=1.0f;
                float cg=1.0f;
                float cb=1.0f;
                float ca=1.0f;
        };
    }
}

#endif // MIKUS_MAIGUI_BASES_COLOR_H
