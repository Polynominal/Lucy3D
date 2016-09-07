#ifndef MIKUS_LUCIA_MATHS_MATRIX_H
#define MIKUS_LUCIA_MATHS_MATRIX_H

#include <iostream>
#include <stdio.h>
#include <initializer_list>
#include <vector>
#include <cmath>
#include <cassert>

#include <Lucia\Maths\Vector.h>
#include <Lucia\Maths\Vertex.h>


//TODO
//Replace STL vectors with the Core. so that we dont need to recode it each time we want to use the matrix..

namespace Lucia{
namespace Maths{
typedef unsigned int uint;
template <unsigned int N>
class Matrix
{
    private:
        float* Core = new float[N*N];
    public:
        std::vector<std::vector<float>> array;
        void createArray(bool identity = false)
        {
            array = std::vector<std::vector<float>>(N);
            for (auto i=0;i < N;i++)
            {
                array[i] = std::vector<float>(N);
                for (auto j=0; j < N;j++)
                {
                    array[i][j] = 0;
                    if (i == j and identity){
                        array[i][j] = 1;
                    }
                }
            }
        };
        Matrix<N>(){createArray(true);};
        Matrix<N>(unsigned int size2){
            for (unsigned int i =0; i < N; i++){
                array.push_back(std::vector<float>(size2));
            }
        }
        Matrix<N>(std::initializer_list<std::initializer_list<float>> data){
            for (auto v: data){
                array.push_back(std::vector<float>(v));
            };
        }
        
        unsigned int getLenght(){return array.size();};
        //GET
        Matrix<(N > 0 ? N-1 : N)> getMinor(unsigned int delted_i,unsigned int deleted_j){
            auto matrix = array;
            unsigned int new_i = 0;
            unsigned int new_j = 0;
            auto nmatrix = Matrix<(N > 0 ? N-1 : N)>((N > 0 ? N-1 : N));
            for (unsigned int i=0;i < N; i++)
            {
                if (i != delted_i){
                    for (unsigned int j=0;j < N; j++)
                    {
                        if (j != deleted_j){
                            nmatrix[new_i][new_j] = matrix[i][j];
                            new_j++;
                            if (new_j == N-1)
                            {
                                new_j = 0;
                                new_i++;
                            }
                        }
                    }
                }
            };
            return nmatrix;  
        };
        float getCofactor(float minor_determinent, unsigned int i,unsigned int j){return std::pow(-1,(1 + i)+(1+j))*minor_determinent;};
        float getDeterminent()
        {
            switch(N){
                case 2:
                    return (array[0][0] * array[1][1]) - (array[0][1]*array[1][0]);
                case 1:
                    return 0;
            };
            
            // Solved using the Laplace method.
            // Check out this great video which explained it to me: https://www.youtube.com/watch?v=HhGLJzmChH0
            float det=0;
            for (unsigned int i =0;i < N;i++)
            {
                 det += array[0][i]*getCofactor(getMinor(0,i).getDeterminent(),0,i);
            };
            return det;
        }
        //
        void preparePackage()
        {
            if (Core != nullptr){delete Core; Core = nullptr;};
            Core = new float[N*N];
            unsigned int index = 0;
            for (unsigned int i =0; i < N;i++)
            {
                for (unsigned int j =0; j < N;j++){
                    Core[index] = array[i][j];
                    index++;
                }
            }
        }
        float* unpack(){
            preparePackage();
            return Core;
        };
        Matrix<N> transpose()
        {
            Matrix<N> newmat = Matrix<N>(N);
            for (auto i =0;i < N;i++)
            {
                for (auto j =0;j < N;j++)
                {
                    newmat[i][j] = array[j][i];
                };
            };
            return newmat;
        };
        Vertex multiplyOut(Vertex& v){
              return Vertex(                
                (v.x * array[0][0]) + (v.y * array[0][1]) + (v.z * array[0][2]) + array[0][3],
                (v.x * array[1][0]) + (v.y * array[1][1]) + (v.z * array[1][2]) + array[1][3],
                (v.x * array[2][0]) + (v.y * array[2][1]) + (v.z * array[2][2]) + array[2][3]
            );  
        };
        Vertex getScale()
        {
            return Vertex(array[0][0],array[1][1],array[2][2]);
        };
        Vertex unProject(Vertex from,Matrix model,Vector viewport){
            //THIS matrix is ALWAYS projection
            Matrix<N> cross = model*(*this);
            
            Matrix<N> Inverse = cross.invert();
            
            Vector tmp = Vector(from.x,from.y,from.z,1.0);
            
            tmp[0] = (tmp[0] - viewport[0])/viewport[2];
            tmp[1] = (tmp[1] - viewport[1])/viewport[3];
            
            tmp[0] = tmp[0] * 2 - 1;
            tmp[1] = tmp[1] * 2 - 1;
            
            Vector obj = Inverse * tmp;
        
            obj =  obj/obj[3];
            
            return Vertex(obj[0],obj[1],obj[2]);
        }
        void scale(Vector A){
            auto V = A.value;
            unsigned int limit =  min(V.size(),N);
            
            for (auto i=0;i <limit;i++)
            {
                array[i][i] *= V[i];
            };
        };
        void scale(Vertex A){scale(Vector(A.x,A.y,A.z,1.0f));};
        void translate(Vector A)
        {
            auto V = A.value;
            for (auto j=0;j < min(V.size(),N);j++)
            {
                array[j][N-1] += V[j];
            };
        };
        void translate(Vertex A){translate(Vector(A.x,A.y,A.z));};
        void rotx(float angle)
        {
            float c = cosf(Maths::radians(angle));
            float s = sinf(Maths::radians(angle));
            float m1  = array[0][1],  m2  = array[0][2],
                  m5  = array[1][1],  m6  = array[1][2],
                  m9  = array[2][1],  m10 = array[2][2],
                  m13 = array[3][1],  m14 =  array[3][2];

            array[0][1] = m1 * c + m2 *-s;
            array[0][2] = m1 * s + m2 * c;
            array[1][1] = m5 * c + m6 *-s;
            array[1][2] = m5 * s + m6 * c;
            array[2][1] = m9 * c + m10*-s;
            array[2][2]= m9 * s + m10* c;
            array[3][1]= m13* c + m14*-s;
            array[3][2]= m13* s + m14* c;
        };
        void roty(float angle)
        {
            float c = cosf(Maths::radians(angle));
            float s = sinf(Maths::radians(angle));
            float m0  = array[0][0],  m2  = array[0][2],
                  m4  = array[1][0],  m6  = array[1][2],
                  m8  = array[2][0],  m10 = array[2][2],
                  m12 = array[3][0],  m14 = array[3][2];

            array[0][0] = m0 * c + m2 * s;
            array[0][2] = m0 *-s + m2 * c;
            array[1][0] = m4 * c + m6 * s;
            array[1][2] = m4 *-s + m6 * c;
            array[2][0] = m8 * c + m10* s;
            array[2][2] = m8 *-s + m10* c;
            array[3][0] = m12* c + m14* s;
            array[3][2] = m12*-s + m14* c;
        };
        void rotz(float angle)
        {
            float c = cosf(Maths::radians(angle));
            float s = sinf(Maths::radians(angle));
            float m0  = array[0][0], m1 = array[0][1],
                  m4  = array[1][0], m5 = array[1][1],
                  m8  = array[2][0], m9 = array[2][1],
                  m12 = array[3][0], m13 =  array[3][1];

            array[0][0] = m0 * c + m1 *-s;
            array[0][1] = m0 * s + m1 * c;
            array[1][0] = m4 * c + m5 *-s;
            array[1][1] = m4 * s + m5 * c;
            array[2][0] = m8 * c + m9 *-s;
            array[2][1] = m8 * s + m9 * c;
            array[3][0]= m12* c + m13*-s;
            array[3][1]= m12* s + m13* c;
        };
        void rotate(Vertex A)
        {
            rotx(A.x);
            roty(A.y);
            rotz(A.z);  
        };
        //
        Matrix<N> flipAcross()
        {
            unsigned int new_i=0;
            unsigned int new_j=0;
            
            Matrix newmat(N);
            for (unsigned int i = 0;i < N; i++)
            {
                for (unsigned int j =0;j < N; j++)
                {
                    newmat[i][j] = array[new_i][new_j];
                    new_i++;
                }
                new_j++;
                new_i = 0;
            }
            return newmat;
        };
        // finding the inverse: https://www.youtube.com/watch?v=h4Za9W2H6fk
        Matrix<N> invert()
        {
            float determinant = getDeterminent();
            Matrix minorMatrix(N);
            for (unsigned int i =0; i < N;i++){
                for (unsigned int j =0; j < N; j++)
                {
                    minorMatrix[i][j] = getCofactor(getMinor(i,j).getDeterminent(),i,j);
                };
            }
            minorMatrix = minorMatrix.flipAcross();
            minorMatrix = minorMatrix*(1.0f/determinant);
            return minorMatrix;
        };
        //
        std::vector<float>& operator[](unsigned int index){return array[index];};
        
        Matrix<N> operator * (float f){
            Matrix<N> mat = Matrix<N>(N);
            for (unsigned int i=0;i < N; i++){
                for (unsigned int j=0;j < N; j++){
                    mat[i][j] = array[i][j]*f;
                }
            }
            return mat;
        }
        Vector operator * (Vector A)
        {
            Vector v = A;
            for (unsigned int i =0;i < N; i++)
            {
                
                float nv = 0;
                for (unsigned int j=0;j < N;j++)
                {
                    nv += array[j][i]*A[j];
                };
                v[i] = nv;
            }
            return v;
        };
        Vertex operator * (Vertex A){return multiplyOut(A);};
        Matrix<N> operator * (Matrix<N> A){
            Matrix<N> nMat(N);
            for (auto i=0;i < N; i++)
            {
                for (auto j=0;j < N; j++)
                {
                    for (auto v=0; v < N;v++)
                    {
                        nMat[i][j] += array[i][v] * A[v][j];
                    }
                }
            }
            return nMat;
        }
        
        Matrix<N> operator / (float f){return *this*(1/f);};
        
        bool operator == (Matrix &b){
            for (unsigned int i=0;i < N; i++)
            {
                for (unsigned int j=0;i < N; j++)
                {
                    if (array[i][j] == b.array[i][j])
                    {
                        return false;
                    }
                };
            };
            return true;
        }
        bool operator != (Matrix &b){return !(this == b);};
};

template <unsigned int N>
inline std::ostream& operator<< (std::ostream& os, Matrix<N> &A){
    os << std::endl << "--Matrix " << N <<  " --" << std::endl;
    for (unsigned int i =0;i < N;i++){
        for (unsigned int j =0;j < N;j++){
            os << A[i][j] << " | ";
        }
        os << std::endl;
    };
    os << "----------" << std::endl;
    return os; 
};
// I took some of the equations here from GLM
// All credit for the maths equations go to GLM.
class Perspective:
public Matrix<4>
{
    public:
    Perspective(
        float fovy,
        float aspect,
        float zNear,
        float zFar
    ){
        make(fovy,aspect,zNear,zFar);
    };
    Perspective(
        float fov,
        float width,
        float height,
        float zNear,
        float zFar
    );
    void make(float fovy,float aspect,float zNear,float zFar)
    {
        createArray();
        assert(abs(aspect - std::numeric_limits<float>::epsilon()) > 0);
        auto tanHalfFovy = tan(fovy / 2);
        array[0][0] = 1 / (aspect * tanHalfFovy);
        array[1][1] = 1 / (tanHalfFovy);
        
        array[2][3] = 1;
        
        array[2][2] = (zFar + zNear) / (zFar - zNear);
        array[3][2] = - (2 * zFar * zNear) / (zFar - zNear);
    };
    void make(float fov,float width,float height,float zNear,float zFar){
        createArray();
        assert(width  > 0);
        assert(height > 0);
        assert(fov > 0);
        
        float h = cos(0.5 * fov) / sin(0.5 * fov);
        float w = h * height / width;
        array[0][0] = w;
        array[1][1] = h;
        
        array[2][3] = 1;
        
        array[2][2] = (zFar + zNear) / (zFar - zNear);
        array[3][2] = - (2 * zFar * zNear) / (zFar - zNear);
    };
    
};
class Ortho:
public Matrix<4>
{
    public:
        Ortho(){createArray(true);};
        Ortho(
            float left,
            float right,
            float bottom,
            float top,
            float zNear,
            float zFar
        ){
            make(left,right,bottom,top,zNear,zFar);
        };
        Ortho(
            float left,
            float right,
            float bottom,
            float top
        )
        {
            make(left,right,bottom,top);
        };
        void make(float left,float right,float bottom,float top)
        {
            buildShell(left,right,bottom,top);
            array[2][2] = 1;
        };
        void make(float left,float right,float bottom,float top,float zNear,float zFar)
        {
            buildShell(left,right,bottom,top);
            array[2][2] = 2/(zFar - zNear);
            array[3][2] = -(zFar + zNear)/(zFar - zNear);  
        }
    private:
        void buildShell(float left,float right,float bottom,float top){
            createArray(true);
            array[0][0] = 2/(right - left);
            array[1][1] = 2/(top - bottom);
            array[3][0] = -(right + left)/(right - left);
            array[3][1] = -(top + bottom)/(top - bottom);
        };
};
}};
#endif