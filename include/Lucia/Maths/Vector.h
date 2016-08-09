#ifndef MIKUS_LUCIA_MATHS_VECTOR_H
#define MIKUS_LUCIA_MATHS_VECTOR_H
#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <math.h>

namespace Lucia{
namespace Maths{
class Vector
{
    public:
        // minimal operators
        std::vector<float> value;
        unsigned int size=0;
        float& operator[](unsigned int s)
        {
            return value[s];
        };
        Vector(float x,float y,float z=0,float w=0){
            value = std::vector<float>(4);
            value[0] = x;
            value[1] = y; 
            value[2] = z;
            value[3] = w;
        };
        Vector(std::initializer_list<float> V)
        {
            for (auto v: V)
            {
                value.push_back(v);
            };
            size = V.size();
        };
        Vector(std::vector<float> V){
            size = V.size();
            value = V;
        };
        float sum()
        const{
            float val=0;
            for (auto v: value)
            {
                val = val + v;
            }
            return val;
        };
        bool operator == (const Vector& A)const{return sum() == A.sum();};
        bool operator != (const Vector& A)const{return sum() != A.sum();};
        inline Vector operator + (const Vector& A){
            auto S = A.value;
            unsigned int s = S.size();
            unsigned int d = value.size();
            unsigned int max = std::max(s,d);
            std::vector<float> v(value.size());
            for (unsigned int i=0;i < max;i++)
            {
                float n = 0;
                if (i < d){n = value[i];};
                if (i < s){n = n + S[i];};
                v[i] = n;
            };
            return Vector(v);
        };
        inline Vector operator +(const float& a){
            for (unsigned int i = 0;i < value.size();i++)
            {
                value[i] += a;
            };
            return Vector(value);
        };
        inline Vector operator -(const float& a){
            for (unsigned int i = 0;i < value.size();i++)
            {
                value[i] -= a;
            };
            return Vector(value);
        };
        inline Vector operator - (const Vector& A){
            auto S = A.value;
            unsigned int s = S.size();
            unsigned int d = value.size();
            unsigned int max = std::max(s,d);
            std::vector<float> v(value.size());
            for (unsigned int i=0;i < max;i++)
            {
                float n = 0;
                if (i < s){n = value[i];};
                if (i < d){n = n - S[i];};
                v[i] = n;
            };
            return Vector(v);
        };
        inline Vector operator -(){
            std::vector<float> v(value.size());
            for (unsigned int i=0;i < value.size();i++)
            {
                v[i] = -value[i];
            };
            return Vector(v);
        };

        inline Vector operator += (const Vector& A)
        {
            auto S = A.value;
            unsigned int s = S.size();
            unsigned int d = value.size();
            unsigned int max = std::max(s,d);
            std::vector<float> v(value.size());
            for (unsigned int i=0;i < max;i++)
            {
                float n = 0;
                if (i < d){n = value[i]*2;};
                if (i < s){n = n + S[i];};
                v[i] = n;
            };
            return Vector(v);
        };
        inline Vector operator -= (const Vector& A)
        {
            auto S = A.value;
            unsigned int s = S.size();
            unsigned int d = value.size();
            unsigned int max = std::max(s,d);
            std::vector<float> v(value.size());
            for (unsigned int i=0;i < max;i++)
            {
                float n = 0;
                if (i < d){n = value[i] - value[i];};
                if (i < s){n = n - S[i];};
                v[i] = n;
            };
            return Vector(v);
        };

        inline Vector operator ++ (int s)
        {
            std::vector<float> v(value.size());
            for (unsigned int i=0;i < value.size();i++)
            {
                v[i] = value[i] + s;
            };
            return Vector(v);
        };
        inline Vector operator -- (int s)
        {
            std::vector<float> v(value.size());
            for (unsigned int i=0;i < value.size();i++)
            {
                v[i] = value[i] - s;
            };
            return Vector(v);
        };
        inline Vector operator ^ (const Vector& A){
            auto S = A.value;
            unsigned int s = S.size();
            unsigned int d = value.size();
            unsigned int max = std::max(s,d);
            std::vector<float> v(value.size());
            for (unsigned int i=0;i < max;i++)
            {
                float n = 0;
                if (i < d){n = value[i];}else{break;};
                if (i < s){n = pow(n,S[i]);};
                v[i] = n;
            };
            return Vector(v);
        };
        inline Vector operator ^ (int A){
            std::vector<float> v(value.size());
            for (unsigned int i=0;i < value.size();i++)
            {
                v[i] = pow(value[i],A);
            };
            return Vector(v);
        };

        inline Vector operator % (const Vector& A){
            auto S = A.value;
            unsigned int s = S.size();
            unsigned int d = value.size();
            unsigned int max = std::max(s,d);
            std::vector<float> v(d);
            for (unsigned int i=0;i < max;i++)
            {
                float n = 0;
                if (i < d){n = value[i];}else{break;};
                if (i < s){n = fmod(n,S[i]);};
                v[i] = n;
            };
            return Vector(v);
        };
        inline Vector operator % (int A){
            std::vector<float> v(value.size());
            for (unsigned int i=0;i < value.size();i++)
            {
                v[i] = fmod(value[i],A);
            };
            return Vector(v);
        };

        inline Vector operator / (const Vector& A){
            auto S = A.value;
            unsigned int s = S.size();
            unsigned int d = value.size();
            unsigned int max = std::max(s,d);

            std::vector<float> v(d);

            for (unsigned int i=0;i < max;i++)
            {
                float n = 0;
                if (i < d){n = value[i];}else{break;};
                if (i < s){n = n/S[i];};
                v[i] = n;
            };
            return Vector(v);
        };
        inline Vector operator / (float A){
            std::vector<float> v(value.size());
            for (unsigned int i=0;i < value.size();i++)
            {
                v[i] = value[i]/A;
            };
            return Vector(v);
        };

        inline Vector operator * (const Vector& A){
            auto S = A.value;

            unsigned int s = S.size();
            unsigned int d = value.size();
            unsigned int max = std::max(s,d);

            std::vector<float> v(max);

            for (unsigned int i=0;i < max;i++)
            {
                float n = 0;
                if (i < d){n = value[i];}else{break;};
                if (i < s){n = n*S[i];};
                v[i] = n;
            };
            return Vector(v);
        };
        inline Vector operator * (float A){
            std::vector<float> v(value.size());
            for (unsigned int i=0;i < value.size();i++)
            {
                v[i] = value[i]*A;
            };
            return Vector(v);
        };

        inline bool operator < (const Vector& A)
        const{
            return sum() < A.sum();
        };
        inline bool operator > (const Vector& A)
        const{
            return sum() > A.sum();
        };
        inline bool operator <= (const Vector& A)const{return sum() <= A.sum();};
        inline bool operator >= (const Vector& A)const{return sum() >= A.sum();};

        virtual ~Vector(){};
};
inline std::ostream& operator<< (std::ostream& os, Vector &A){
    os << std::endl << "--Vector[ " << A.size << "]--" << std::endl;
    int no =0;
    for (auto v: A.value)
    {
        if (no%5 == 0)
        {
            os << std::endl;
        };
        os << "[" << no << "] : " << v;
        no++;
    };
    os << "------------" << std::endl;
return os; };
}}
#endif