#ifndef MIKUS_LUCIA_MATHS_QUATERNION_H
#define MIKUS_LUCIA_MATHS_QUATERNION_H

#include <Lucia\Maths\Vertex.h>
#include <Lucia\Maths\Vector.h>
#include <Lucia\Maths\Matrix.h>
#include <Lucia\Settings.h>

namespace Lucia{
namespace Maths{

class Quaternion
{
public:
    float x=0;
    float y=0;
    float z=0;
    float w=1;
    Quaternion(){};
    Quaternion(float vx,float vy,float vz,float vw){
        x = vx;y = vy;z = vz;w = vw;
    };
    Quaternion(Vertex v)
    {
        fromAxis(v.x,v.y,v.z);
    };
    Quaternion(float angle,Vertex normal){fromAngleAxis(angle,normal);}
    virtual ~Quaternion(){};
    //
    
    Vertex multiplyOut(Vertex v){
        Vertex spoof = Vertex(x,y,z);
        Vertex uv = spoof.cross(v);
        Vertex uuv = spoof.cross(uv); 
        return v + ((uv * w) + uuv)*2;
    };
    Vertex rotate(Vertex v)
    {
        return multiplyOut(v);
    };
    Quaternion conjugate()
    {
        return Quaternion(-x,-y,-z,w);
    };
    Quaternion getNormal()
    {
        float magnitude = sqrt(std::pow(w,2) + std::pow(x,2) + std::pow(y,2) + std::pow(z,2));
        if (std::isnan(magnitude)){magnitude = 1;};
        return Quaternion(x / magnitude,y / magnitude,z / magnitude,w / magnitude);
    };
    Quaternion normalize()
    {
        return getNormal();
    };
    Quaternion fromAxis(float dx,float dy,float dz)
    {        
        float nx = dx;
        float ny = dy;
        float nz = dz;
        #ifdef LUCIA_USE_DEGREES
        nx =  Maths::radians(dx);
        ny =  Maths::radians(dy);
        nz =  Maths::radians(dz);
        #endif
        float c1 = cos(nx/2);
        float s1 = sin(nx/2);
        float c2 = cos(ny/2);
        float s2 = sin(ny/2);
        float c3 = cos(nz/2);
        float s3 = sin(nz/2);
        float c1c2 = c1*c2;
        float s1s2 = s1*s2;
        w =c1c2*c3 - s1s2*s3;
        x =c1c2*s3 + s1s2*c3;
        y =s1*c2*c3 + c1*s2*s3;
        z =c1*s2*c3 - s1*c2*s3;

        return *this;
    };
    Vertex toAxis();
    Maths::Matrix<4> toMatrix()
    {
        Maths::Matrix<4> mat = Maths::Matrix<4>();
        float qxx(x * x);
        float qyy(y * y);
        float qzz(z * z);
        float qxz(x * z);
        float qxy(x * y);
        float qyz(y * z);
        float qwx(w * x);
        float qwy(w * y);
        float qwz(w * z);

        mat[0][0] = 1 - 2 * (qyy +  qzz);
        mat[0][1] = 2 * (qxy + qwz);
        mat[0][2] = 2 * (qxz - qwy);

        mat[1][0] = 2 * (qxy - qwz);
        mat[1][1] = 1 - 2 * (qxx +  qzz);
        mat[1][2] = 2 * (qyz + qwx);

        mat[2][0] = 2 * (qxz + qwy);
        mat[2][1] = 2 * (qyz - qwx);
        mat[2][2] = 1 - 2 * (qxx +  qyy);
        
        return mat.transpose();
    };
    
    Quaternion fromAngleAxis(float angle,Vertex normal)
    {
        Quaternion q = Quaternion();
        auto a = 
        #ifdef LUCIA_USE_DEGREES
        Maths::radians(angle);
        #else
        angle;
        #endif
        
        auto s = std::sin(a * 0.5f);

        q.w = std::cos(a * 0.5f);
        q.x = normal.x * s;
        q.y = normal.y * s;
        q.z = normal.z * s;
        return q;
    };
    Quaternion pitch(float angle,Vertex l,Vertex& f,Vertex& u)
    {
        auto q = Quaternion().fromAngleAxis(angle,-l);
        f = q*f;
        u = q*u;
        return q*(*this);
    };
    Quaternion yaw(float angle,Vertex &l,Vertex &f,Vertex u)
    {
        auto q = Quaternion();
        q = q.fromAngleAxis(angle,u);
        l = q*l;
        f = q*f;
        return q*(*this);
    };
    Quaternion roll(float angle,Vertex &l,Vertex f,Vertex &u)
    {
        auto q = Quaternion().fromAngleAxis(angle,f);
        u = q*u;
        l = q*u;
        return q*(*this);
    };
    Vertex operator* (const Vertex &A){return rotate(A);};
    Quaternion operator* (const Quaternion &A)
    {
        Quaternion Q;
        Q.w = w*A.w - x*A.x - y*A.y - z*A.z;
        Q.x = w*A.x + x*A.w + y*A.z - z*A.y;
        Q.y = w*A.y + y*A.w + z*A.x - x*A.z;
        Q.z = w*A.z + z*A.w + x*A.y - y*A.x;
        return Q;
    };
    Quaternion operator+ (const Quaternion &A) 
    {
        Quaternion Q;
        Q.w = w+A.w;
        Q.x = x+A.x;
        Q.y = y+A.y;
        Q.z = z+A.z;
        return Q;
    };
    bool operator == (const Quaternion &A)
    {
        return A.x == x and A.y == y and A.z == z and A.w == w;
    }
};

}}
#endif