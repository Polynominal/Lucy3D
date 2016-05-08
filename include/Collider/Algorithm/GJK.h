#ifndef COLLIDER_MIKUS_GJK_H
#define COLLIDER_MIKUS_GJK_H
#include <Collider/Collider.h>
#include <Collider/Shape.h>
class Collider::GJK
{
    public:
        GJK(){};
        virtual ~GJK(){};

        bool checkCollision(std::vector<Vertex>Points1,std::vector<Vertex>Points2);
        bool collidesWith(Shape *A,Shape *B);
        bool collidesWith(Shape *A,Vertex B);
        bool collidesWith(std::vector<Vertex> points, Shape *A);
        std::vector<Vertex> simplex;

    private:
        bool tetrahedron(Vertex &dir);
        bool triangle(Vertex &dir);
        bool line(Vertex &dir,Vertex a,Vertex b);
        bool checkTetrahedron(Vertex ao,Vertex ab,Vertex ac,Vertex abc,Vertex &dir);
        bool containsOrigin(Vertex &dir);
        // support for points only comparison
        Vertex farthestPoint(std::vector<Vertex> Points,Vertex dir);
        Vertex support(std::vector<Vertex> Points1,std::vector<Vertex> Points2,Vertex dir);
        // support for shapes
        Vertex minowskiSum(Shape *A,Shape *B,Vertex dir);
        Vertex minowskiSum(std::vector<Vertex> A,Shape *B,Vertex dir);
};

#endif // COLLIDER_MIKUS_GJK_H
