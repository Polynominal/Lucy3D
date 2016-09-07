#ifndef LUCIA_MATHS_SPRITESHEET
#define LUCIA_MATHS_SPRITESHEET
#include <vector>
#include <Lucia\Maths\Vec2.h>
#include <Lucia/Utils/logistics.h>
#include <Lucia\Maths\General.h>

namespace Lucia{
namespace Maths 
{
    class Sprite // used for a quad
    {public:
        // x and y from the top
        // w and h of the size of the quad
        float x=0;
        float y=0;
        float w=0;
        float h=0;
        // machine cords
        Vec2 top = Vec2(0,0);
        Vec2 bottom = Vec2(0,0);
        //
        Sprite(){};
        virtual ~Sprite(){};
        Sprite(float nx,float ny,float nw,float nh){create(nx,ny,nw,nh);};
        Sprite(float ix,float iy,float iw,float ih,float imgw,float imgh){create(ix,iy,iw,ih,imgw,imgh);};
        
        virtual void create(float nx,float ny,float nw,float nh)
        {
            x = nx;y = ny;w = nw;h = nh;
        };
        virtual void create(float ix,float iy,float iw,float ih,float imgw,float imgh)
        {
            auto ny = imgh - iy;
            float topx = ix/imgw;
            float topy = ny/imgh;
            float bottomx = ((ix + iw)/imgw);
            float bottomy = ((ny - ih)/imgh);
            if (isnan(topx)){topx = 1;};
            if (isnan(topy)){topy = 1;};
            if (isnan(bottomx)){bottomx = 0;};
            if (isnan(bottomy)){bottomy = 0;};
            
            top = Vec2(topx,bottomy);
            bottom = Vec2(bottomx, topy);
            
            x = ix;
            y = iy;
            //y set previously.
            w = iw;
            h = ih;
        };
    };
    std::vector<Sprite> createSprites(float ox,float oy,float w,float h,float imgw,float imgh,unsigned int startFrame,unsigned int frame_no=0);
    
    class SpriteSheet
    {
        public:
            SpriteSheet(float image_w,float image_h)
            {
                Dimensions = Vec2(image_w,image_h);
            };
            virtual ~SpriteSheet();
            std::vector<Sprite> createSprites(float ox,float oy,float w,float h,unsigned int startFrame=0,unsigned int frame_no=0)
            {
                return Maths::createSprites(ox,oy,w,h,Dimensions.x,Dimensions.y,startFrame,frame_no);
            };
            Sprite create(float x,float y,float w,float h)
            {
                // U and V are in machine cords in other words percentages. Hence we need to find the percentages
                return Sprite(x,y,w,h,Dimensions.x,Dimensions.y);
            };
        private:
            Vec2 Dimensions = Vec2(0,0);
    };
}}
#endif