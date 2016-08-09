#ifndef MIKUS_LUCIA_MATHS_GENERALS_H
#define MIKUS_LUCIA_MATHS_GENERALS_H
#include <cmath>
#include <algorithm>
namespace Lucia {
namespace Maths{
    using namespace std;
    
    float distance(float x,float y);
    double radians(float deg);
    double degrees(float rad);
    
    template<typename T>
    inline double linearInterpolation(T v,T v2,T alpha){return fma(alpha,v2,fma(-alpha,v,v));};
    template<typename T> 
    T clamp(const T &val, const T &min, const T &max) 
    { 
        return std::max(min, std::min(max, val)); 
    }   
}}
#endif