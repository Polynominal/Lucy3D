#include "Lucia/Maths/Quaternion.h"

using namespace Lucia;
using namespace Maths;
Vertex Quaternion::toAxis() 
{
    float pitch  = 0;
    float yaw = 0;
    float roll = 0;
    
    float sqw = w*w;
    float sqx = x*x;
    float sqy = y*y;
    float sqz = z*z;
    float unit = sqx + sqy + sqz + sqw; // if normalised is one, otherwise is correction factor
    float test = x*y + z*w;
    if (test > 0.499f*unit) { // singularity at north pole
        pitch = -2.0f * atan2(x,w);
        roll = 0.0f;
        return Vertex(Maths::degrees(pitch),360-90,Maths::degrees(roll));
    
    }else if (test < -0.499f*unit) { // singularity at south pole
        pitch = 2.0f * atan2(x,w);
        roll = 0.0f;
        return Vertex(Maths::degrees(pitch),90,Maths::degrees(roll));
    }
    pitch = atan2(2*y*w-2*x*z , sqx - sqy - sqz + sqw);
    yaw = asin(2*test/unit);
    roll = atan2(2*x*w-2*y*z , -sqx + sqy - sqz + sqw);
    return Vertex(Maths::degrees(pitch),Maths::degrees(yaw),Maths::degrees(roll));
};