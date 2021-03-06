#ifndef MIKUS_LUCIA_UTILS_H
#define MIKUS_LUCIA_UTILS_H
#include <string>
#include <ctime>
#include <thread>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
namespace Lucia {
namespace Utils
{
    using std::vector;
    struct Vec2
    {
        Vec2(){};
        Vec2(float xt,float yt): x(xt),y(yt){};
        float x;
        float y;
    };
    std::string getDateTime();
    std::string getCleanDate();
    std::string getDate();
    std::string getTime();
    bool fileExists(const char* name);
    bool accumulate(int ticks);
    void sleep(int milliseconds);
    const tm* updateTime();
    template <typename T>
    inline T difference(T a,T b)
    {
        return std::max(a,b) - std::min(a,b);
    };
    template <typename T,typename T2>
    inline T2 findInMap(std::map<T2,T> map, T value)
    {
        T2 value2;
        for (auto it : map)
        {
            if (it.second == value){return it.first;};
        }
        return value2;
    };
    template<typename T,typename A>
    inline void erase_if (vector<T>*a,A condition)
    {
        unsigned int no = 0;
        for (auto v: *a)
        {
            if (condition(v))
            {
                a->erase(a->begin() + no);
                erase_if(a,condition);
            };
            no++;
        }
    };
    template<typename T,typename A>
    inline void erase_if_once (vector<T>*a,A condition)
    {
        unsigned int no = 0;
        for (auto v: *a)
        {
            if (condition(v))
            {
                a->erase(a->begin() + no);
                return;
            };
            no++;
        }
    };
    template<typename T,typename A>
    inline bool find_if (vector<T>*a,A condition)
    {
        unsigned int no = 0;
        for (auto v: *a)
        {
            if (condition(v))
            {
                return true;
            };
            no++;
        }
        return false;
    };
    template<typename T,typename A>
    inline bool contains_if(vector<T>*a,A condition)
    {
        unsigned int no = 0;
        for (auto v: *a)
        {
            if (condition(v))
            {
                return true;
            };
            no++;
        }
        return false;
    };
    template<typename T,typename A>
    inline vector<T> find_store_if (vector<T>*a,A condition,vector<T>data={})
    {
        unsigned int no = 0;
        for (auto v: *a)
        {
            if (condition(v))
            {
                data.push_back(v);
                return find_if(a,condition,data);
            };
            no++;
        }
        return data;
    };
    inline float getFPS(double dt)
    {
        return 1.0f/(dt);
    }
};}
#endif // UTILS_H
