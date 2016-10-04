#ifndef MIKUS_LUCIA_MATHS_GENERALS_H
#define MIKUS_LUCIA_MATHS_GENERALS_H
#include <cmath>
#include <ctime>
#include <algorithm>
#include <random>
#include <iostream>
#include <Lucia/Settings.h>

#define LUCIA_PI 3.141592653589793
namespace Lucia {
namespace Maths{
    using namespace std;
    
    float distance(float x,float y);
    double radians(float deg);
    float degrees(long double rad);
    
    template<typename T>
    inline double linearInterpolation(T v,T v2,T alpha){return fma(alpha,v2,fma(-alpha,v,v));};
    template<typename T> 
    T clamp(const T &val, const T &min, const T &max) 
    { 
        return std::max(min, std::min(max, val)); 
    }
    template<typename T>
    T loop(const T &val,const T &min,const T &max)
    {
        if (val > max)
        {
            return loop(val - max,min,max);
            
        }else if (val < min)
        {
            return loop(max - std::abs(val),min,max);
        }
        return val;
    }
    #ifdef LUCIA_USE_DEGREES
    inline float getAngle(float angle){return Maths::loop(angle,0.0f,360.0f);};
    #else
    inline double getAngle(double angle){return Maths::loop(angle,0,LUCIA_PI*2.0f);};
    #endif

    extern std::random_device rand_device;
    extern std::default_random_engine engine;
    
    template <typename T>
    T random(const T &low,const T &high)
    {        
        // Seed with a real random value, if available
        std::random_device r;
        std::default_random_engine e1(r());
        std::uniform_real_distribution<T> dist(low,high);
        return dist(e1);
    };
    template <typename T>
    inline void randomSeed(T seed)
    {
        engine.seed(seed);
    };
    template <typename T>
    inline void randomSeed(std::initializer_list<T>t)
    {
        engine.seed(t);
    };
    inline void randomSeed()
    {
        randomSeed(std::time(nullptr));
    };
}}  
#endif