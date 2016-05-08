#ifndef MIKUS_WINDOWER_DATA_H
#define MIKUS_WINDOWER_DATA_H
#include <Window/Windower.h>
#include <iostream>
namespace Windower
{
    using namespace std;
    namespace Data
    {
        namespace Limits
        {
            inline std::pair<vec2,vec2> bounds(uint displayID){
                SDL_Rect info;
                if(SDL_GetDisplayBounds(displayID,&info)==0)
                {
                    return std::pair<vec2,vec2>(
                        vec2(info.x,info.y),
                        vec2(info.w,info.h));
                }else
                {return std::pair<vec2,vec2>(vec2(320,240),vec2(0,0));};
            };
            inline vec2 size(uint displayID){
                return bounds(displayID).second;
            };
            inline vec2 position(uint displayID){
                return bounds(displayID).first;
            };
        }
        inline void dpi(uint displayID,float* ddpi,float * hdpi,float* vdpi){SDL_GetDisplayDPI(displayID,ddpi,hdpi,vdpi);};
        inline string driver(uint driverID){return string(SDL_GetCurrentVideoDriver());};
        inline string name(uint displayID){return string(SDL_GetDisplayName(displayID));};

        inline Mode mode(uint i){
            SDL_DisplayMode current;
            SDL_GetCurrentDisplayMode(i,&current);
            return Mode(current.w,current.h,current.refresh_rate);
        };

        inline Mode desktopMode(uint i){
            SDL_DisplayMode current;
            SDL_GetDesktopDisplayMode(i,&current);
            return Mode(current.w,current.h,current.refresh_rate);
        };

        inline uint Displays(){return SDL_GetNumVideoDisplays();};

        inline std::vector<Mode> modes(){
            std::vector<Mode> mdes;
            for (uint i=0;i < Displays();i++)
            {
                mdes.push_back(mode(i));
            };
            return mdes;
        };
    }
};

#endif // MIKUS_WINDOWER_DATA_H
