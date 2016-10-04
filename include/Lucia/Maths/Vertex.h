#ifndef MIKUS_LUCIA_MATHS_VERTEX_H
#define MIKUS_LUCIA_MATHS_VERTEX_H

#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <math.h>
#include <Lucia/Maths/General.h>
#include <Lucia\Settings.h>

namespace Lucia
{
namespace Maths
{
    class Vertex;
    
    bool insideSphere(Vertex V,float radius);
    std::pair<Vertex,Vertex> extremes(std::vector<Vertex>Points);
    Vertex centerFromPoints(std::vector<Vertex> Points);
    void subdivide(Vertex A,Vertex B,Vertex C,std::vector<Vertex>&,int level);
    std::vector<Vertex> subdividePoints(std::vector<Vertex> Points,int level=1);
/**
 * @class Vertex
 * @author Mikus
 * @date 07/08/2016
 * @file Vertex.h
 * @brief 
 * Vertex with 3 vars: x,y,z  to be used alongside quaternions and matrixes within 2D and 3D space.
 */
class Vertex
    {
    public:
        double x;
        double y;
        double z;

        Vertex(){x =0;y=0;z=0;};
        Vertex(double nx,double ny,double nz){x = nx;y = ny;z = nz; validate();};
        Vertex(double nx,double ny){x = nx;y = ny;z = 0; validate();};
        ~Vertex(){};

        inline bool operator == (const Vertex& A){return (x == A.x && y == A.y && A.z == z);};
        inline bool operator != (const Vertex& A){return (x != A.x && y != A.y && A.z != z);};

        inline Vertex operator + (const Vertex& A){return Vertex(x + A.x,y + A.y,z + A.z);};
        inline Vertex operator - (const Vertex& A){return Vertex(x - A.x,y - A.y,z - A.z);};

        inline Vertex operator + (float A){return Vertex(x + A,y + A,z + A);};
        inline Vertex operator - (float A){return Vertex(x - A,y - A,z - A);};

        inline Vertex operator -(){return Vertex(-x,-y,-z);};

        inline Vertex operator += (const Vertex& A){Vertex t = Vertex(x,y,z) + A;x = t.x;y=t.y;z=t.z;return *this;};
        inline Vertex operator -= (const Vertex& A){Vertex t = Vertex(x,y,z) - A;x = t.x;y =t.y;z=t.z;return *this;};

        inline Vertex operator ++ (int v){return Vertex(x + v,y + v,z + v);};
        inline Vertex operator -- (int v){return Vertex(x - v,y - v,z - v);};

        inline Vertex operator ^ (const Vertex& A){validate(); return Vertex(pow(x,A.x),pow(y,A.y),pow(z,A.z));};
        inline Vertex operator ^ (int A){validate(); return Vertex(pow(x,A),pow(y,A),pow(z,A));};

        inline Vertex operator % (const Vertex& A){validate(); return Vertex(fmod(x,A.x),fmod(y,A.y),fmod(z,A.z));};
        inline Vertex operator % (int A){validate(); return Vertex(fmod(x,A),fmod(y,A),fmod(z,A));};

        inline Vertex operator / (const Vertex& A)
        {
            Vertex t = Vertex(x/A.x,y/A.y,z/A.z);
            t.validate();
            return t;
        };

        inline Vertex operator / (int A)
        {
            Vertex t = Vertex(x/A,y/A,z/A);
            t.validate();
            return t;
        };

        inline Vertex operator * (const Vertex& A){validate(); return Vertex(x*A.x,y*A.y,z*A.z);};
        inline Vertex operator * (float A){validate(); return Vertex(x*A,y*A,z*A);};

        inline bool operator < (const Vertex& A) const {return (x < A.x && y < A.y && z < A.z);};
        inline bool operator > (const Vertex& A) const {return (x > A.x && y > A.y && z > A.z);};

        inline bool operator <= (const Vertex& A) const {return (x <= A.x && y <= A.y && z <= A.z);};
        inline bool operator >= (const Vertex& A) const {return (x >= A.x && y >= A.y && z >= A.z);};

        Vertex distance(Vertex& A){return Vertex(Maths::distance(x,A.x),Maths::distance(y,A.y),Maths::distance(z,A.z));};
        double magnitude(){validate(); return sqrt(pow(x,2) + pow(y,2) + pow(z,2));};
        Vertex normalize(){
        double m = magnitude();
        return Vertex(x/m,y/m,z/m);
        ;};
        void validate()
        {
            if (std::isnan(x))
            {x = 0;};
            if (std::isnan(y))
            {y = 0;};
            if (std::isnan(z))
            {z = 0;};
        };
        Vertex normal()
        {
            auto m = magnitude();
            return Vertex(x/m,y/m,z/m);
        };
        Vertex normalize(Vertex A,float lenght)
        {
            float dx = x - A.x;
            float dy = y - A.y;
            float dz = z - A.z;
            float distance = sqrt(dx*dx + dy*dy + dz*dz);
            dx = dx * (lenght/distance);
            dy = dy * (lenght/distance);
            dz = dz * (lenght/distance);
            return Vertex(A.x + dx,A.y + dy,A.z + dz);
        }
        float max() {return std::max(x,std::max(y,z));};
        Vertex max(Vertex &B) {return Vertex(std::max(x, B.x),std::max(y, B.y),std::max(z, B.z));};

        float min() {return std::max(x,std::min(y,z));};
        Vertex min(Vertex &B) {return Vertex(std::min(x, B.x),std::min(y, B.y),std::min(z, B.z));};
        
        Vertex clamp(double min,double max){return Vertex(Maths::clamp(x,min,max),Maths::clamp(y,min,max),Maths::clamp(z,min,max));};
        Vertex loop(double min,double max)
        {
            return Vertex(Maths::loop(x,min,max),Maths::loop(y,min,max),Maths::loop(z,min,max));
        };
        double dot(const Vertex& A){return x*A.x + y*A.y + z*A.z;};
        Vertex cross(const Vertex& A){return Vertex(
            (y*A.z) - (z*A.y),
            (z*A.x) - (x*A.z),
            (x*A.y) - (y*A.x));};
        Vertex doubleCross(const Vertex& A){
            Vertex Cross = this->cross(A);
            return cross(Cross);
        };
        Vertex radians(){return Vertex(Maths::radians(x),Maths::radians(y),Maths::radians(z));};
        Vertex degrees(){return Vertex(Maths::degrees(x),Maths::degrees(y),Maths::degrees(z));};
        Vertex sin()
        {
            return Vertex(std::sin(x),std::sin(y),std::sin(z));
        };
        Vertex cos()
        {
            return Vertex(std::cos(x),std::cos(y),std::cos(z));
        };
        Vertex tan()
        {
            return Vertex(std::tan(x),std::tan(y),std::tan(z));
        };
        double size(){return abs(x)+abs(y)+abs(z);};
        // The sum of the vector
        inline int sum(){return x+y+z;};
};
    inline std::ostream& operator<< (std::ostream& os, Vertex &A){
        os << std::endl << "--Vertex--" << std::endl;
        os << "X: " << A.x << " Y: "<< A.y << " Z: " << A.z << std::endl;
        os << "----------" << std::endl;
        return os; };
    inline Vertex operator* (float x, Vertex A){
        return Vertex(x*A.x,x*A.y,x*A.z);
    };

    inline bool insideSphere(Vertex V,float radius)
    {
        if (pow(V.x,2) + pow(V.y,2) + pow(V.z,2) <= pow(radius,2))
        {
            return true;
        }
        return false;
    };
    inline std::pair<Vertex,Vertex> extremes(std::vector<Vertex>Points)
    {
        Vertex Max = Vertex(0,0,0);
        Vertex Min = Vertex(0,0,0);
        bool set = false;
        for (auto vert: Points)
        {
            if (!set)
            {
                Max = vert;
                Min = vert;
                set = true;
            }
            Max.x = std::max(vert.x,Max.x);
            Max.y = std::max(vert.y,Max.y);
            Max.z = std::max(vert.z,Max.z);

            Min.x = std::min(vert.x,Min.x);
            Min.y = std::min(vert.z,Min.y);
            Min.z = std::min(vert.y,Min.z);

        }
        return std::pair<Vertex,Vertex>{Min,Max};
    };
    inline Vertex centerFromPoints(std::vector<Vertex> Points)
    {
        auto p = extremes(Points);
        auto Min = p.first;
        auto Max = p.second;
        Vertex Dimensions = (Max - Min);
        return Max - Dimensions/2;
    }
    inline void subdivide(Vertex v1,Vertex v2,Vertex v3, std::vector<Vertex> &ret,int level)
    {
        if (level > 0 )
        {
            auto v12 = (v1 + v2)/2;
            auto v13 = (v1 + v3)/2;
            auto v23 = (v2 + v3)/2;

            ret.push_back(v12);
            ret.push_back(v13);
            ret.push_back(v23);

            subdivide(v1,v12,v13,ret,level -1);
            subdivide(v12,v2,v23,ret,level-1);
            subdivide(v13,v23,v3,ret,level-1);
            subdivide(v12,v23,v13,ret,level -1);
        }
    }
    inline std::vector<Vertex> subdividePoints(std::vector<Vertex> Points,int level)
    {
        std::vector<Vertex> p;
        return p;
    }
}}

#endif