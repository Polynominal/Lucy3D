#include "Lucia/Collider/BoundingBox.h"
using namespace Lucia;
using namespace Collider;

// BOUNDING BOX

// 6-------------5
// |##           ##
// |   ##          ##
// |     ##          ##
// 7        1-----------2
//  ##      |     4#    |
//    ##    |       ##  |
//      ##  |         ##|
//        ##0-----------3

BoundingBox::BoundingBox(){};
BoundingBox::BoundingBox(float x,float y,float z,float w,float h,float d)
{
    Center = Vertex(x,y,z);
    Dimensions = Vertex(w,h,d);
    refresh();
}
bool BoundingBox::contains(float x,float y,float z)
{
    bool collides = false;
    if (std::abs(Center.x - x)*2 < (Dimensions.x + 1)&&
        std::abs(Center.y - y)*2 < (Dimensions.y + 1)&&
        std::abs(Center.z - z)*2 < (Dimensions.z + 1))
    {
        collides = true;
    };
    return collides;
}
void BoundingBox::refresh()
{
    // make sure you set either Dimensions or Center before calling refresh!
    float w = Dimensions.x;
    float h = Dimensions.y;
    float d = Dimensions.z;
    float x = Center.x;
    float y = Center.y;
    float z = Center.z;
    
    Points.clear();
    Faces.clear();
    Points.push_back(Vertex(x - w/2,y - h/2,z - d/2)); //
    Points.push_back(Vertex(x - w/2,y + h/2,z - d/2));
    Points.push_back(Vertex(x + w/2,y + h/2,z - d/2));
    Points.push_back(Vertex(x + w/2,y - h/2,z - d/2));
    Points.push_back(Vertex(x + w/2,y -h/2,z + d/2));
    Points.push_back(Vertex(x + w/2,y +h/2,z + d/2));
    Points.push_back(Vertex(x - w/2, y + h/2, z + d/2));
    Points.push_back(Vertex(x - w/2,y -h/2,z + d/2));

    Max = Vertex(Points[0].x,Points[1].y,Points[4].z);
    Min = Vertex(Points[3].x,Points[7].y,Points[3].z);

    w++;
    h++;
    d++;
    Faces.push_back(Vertex(x,y,z + h/2)); // front
    Faces.push_back(Vertex(x,y,z -h/2)); // back
    Faces.push_back(Vertex(x-w/2,y,z)); // left
    Faces.push_back(Vertex(x+w/2,y,z)); // right
    Faces.push_back(Vertex(x,y + h/2,z)); // top
    Faces.push_back(Vertex(x,y-h/2,z)); // bottom

    Transformation = Matrix<4>();
    Transformation.scale(Dimensions);

    //Transformation.rotate(Rotation); - to be used with

    Transformation.translate(Center);
}
Vertex BoundingBox::getMaxDimensions()
{
    float w = 0;
    float M = Dimensions.max();
    unsigned int deg = 360/2;
    auto rad = Collider::radians(deg);
    w = std::max(w,(M*abs(sinf(rad)) + M*abs(cosf(rad)) + (M/2)*abs(cosf(rad))));
    return Vertex(w,w,w);
}
bool BoundingBox::collidesWith(BoundingBox *B)
{
    auto Max2 = B->Max;
    auto Min2 = B->Min;
    bool collides = false;

    auto Center2 = B->Center;
    if (std::abs(Center.x - Center2.x)*2 < (Dimensions.x + B->Dimensions.x)&&
        std::abs(Center.y - Center2.y)*2 < (Dimensions.y + B->Dimensions.y)&&
        std::abs(Center.z - Center2.z)*2 < (Dimensions.z + B->Dimensions.z))
    {
        collides = true;
    };
    return collides;
}
bool BoundingBox::engulfs(BoundingBox *B)
{
    if (Faces.size() == 0)
    {
        return false;
    }
    for (auto v : Faces)
    {
        if  (!B->contains(v))
        {
            return false;
        }
    }
    return true;
}
BoundingBox::~BoundingBox()
{

}