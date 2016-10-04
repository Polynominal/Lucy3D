#include "Lucia/Maths/General.h"
using namespace std;
using namespace Lucia;
namespace Lucia{
namespace Maths
{
    std::random_device rand_device;
    std::default_random_engine engine;
};
}
float Maths::distance(float x,float y){return max(x,y) - min(x,y);};
double Maths::radians(float deg){if (isnan(deg) or deg == 0){return 0;}else{return deg * LUCIA_PI / 180.0f;}};
float Maths::degrees(long double rad){if (isnan(rad) or rad == 0){return 0;}else{return rad/LUCIA_PI *180.0f;}};
