#ifndef MIKUS_LUCIA_GRAPHICS_BASE_FILTER_H
#define MIKUS_LUCIA_GRAPHICS_BASE_FILTER_H
#include <Lucia/Settings.h>
namespace Lucia {
namespace Graphics
{
    enum FILTER
    {
        NEAREST,
        LINEAR,
    };
    enum WRAP
    {
        EDGE,
        MIRROR,
        BORDER
    };
    inline int getMechanicalFilter(FILTER f)
    {
        switch(f)
        {
            case FILTER::LINEAR:
            {
                return GL_LINEAR;
            }
            case FILTER::NEAREST:
            {
                return GL_NEAREST;
            }
        }
        return GL_NEAREST;
    };
    inline int getMechanicalWrap(WRAP f)
    {
        switch(f)
        {
            case WRAP::BORDER:
            {
                return GL_CLAMP_TO_EDGE;
            }
            case WRAP::EDGE:
            {
                return GL_CLAMP_TO_EDGE;
            }
            case WRAP::MIRROR:
            {
                return GL_MIRRORED_REPEAT;
            }
        }
    };

    namespace Base
    {
        class Filter
        {
            public:
                Filter(){};
                //gets
                int getHorizWrap(){return HorizontalW;};
                int getVertWrap(){return VerticalW;};
                std::pair<int,int> getWrap(){  return std::pair<int,int>(getHorizWrap(),getVertWrap());};
                int getDownFilter(){return dFilter;};
                int getUpFilter(){return uFilter;};
                std::pair<int,int> getFilter(){return std::pair<int,int>(getDownFilter(),getUpFilter());};
                //sets
                void setUpFilter(FILTER f){uFilter = f;needsRefresh=true;};
                void setDownFilter(FILTER f){dFilter = f; needsRefresh=true;};
                void setFilter(FILTER f){setUpFilter(f);setDownFilter(f);};

                void setHorizWrap(WRAP w){VerticalW = w; needsRefresh=true;};
                void setVertWrap (WRAP w){HorizontalW = w; needsRefresh=true;};
                void setWrap(WRAP wrap){setHorizWrap(wrap);setVertWrap(wrap);};
                //apply
                void applyWrap()
                {
                    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,getMechanicalWrap(HorizontalW));
                    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,getMechanicalWrap(VerticalW));
                };
                void applyFilter()
                {
                    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,getMechanicalFilter(dFilter));
                    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,getMechanicalFilter(uFilter));
                };
                //requirement for apply border color.
                bool checkBorderEdge(WRAP w)
                {
                    #if defined LUCIA_USE_GLES2 || defined LUCIA_USE_GLES3
                    return (w == WRAP::BORDER or w == WRAP::EDGE);
                    #else
                    return (w == WRAP::EDGE);
                    #endif
                };
                //
                void applyBorderColor()
                {
                    #ifndef LUCIA_USE_GLES2
                    if (checkBorderEdge(HorizontalW) or checkBorderEdge(VerticalW))
                    {
                        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, Color);
                    };
                    #endif
                };
                void forceApplyFilters()
                {
                    applyWrap();
                    applyFilter();
                    applyBorderColor();
                };
                void applyFilters()
                {
                    if (needsRefresh)
                    {
                        forceApplyFilters();
                    }
                };
                //[!] Must have bound image with glBindTexture [!]
                virtual ~Filter(){};

            private:
                bool needsRefresh = true;
            protected:
                
                float Color[4] = {1.0f,1.0f,1.0f,1.0f};

                FILTER dFilter=FILTER::NEAREST;
                FILTER uFilter=FILTER::NEAREST;
                FILTER mipFilter= FILTER::NEAREST;

                WRAP HorizontalW=WRAP::EDGE;
                WRAP VerticalW=WRAP::EDGE;
        };
    }
}}

#endif // MIKUS_GRAPHICS_FILTER_H
