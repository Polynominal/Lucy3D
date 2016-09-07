#include <Lucia/Maths/SpriteSheet.h>
using namespace Lucia;
using namespace Maths;
std::vector<Sprite> Maths::createSprites(float ox,float oy,float w,float h,float imgw,float imgh,unsigned int startFrame,unsigned int frame_no)
{
    std::vector<Sprite> Frames;
    float width =Utils::Logic::lessThanZeroIsDefault(w,imgw);
    float height =Utils::Logic::lessThanZeroIsDefault(h,imgh);
    float rowsize = imgw/width;
    if (frame_no == 0)
    {
        frame_no = imgw/width * imgh/height;
    };
    for (unsigned int i=startFrame; i < frame_no+1; i++)
    {
        unsigned int row = floor(i/rowsize);
        unsigned int column = std::fmod(i,rowsize);
        Frames.push_back(Sprite((column*width) + ox, (row*height) + oy,width,height,imgw,imgh));
    };
    return Frames;
};