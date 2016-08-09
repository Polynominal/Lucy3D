#ifndef MIKUS_LUCIA_COLLIDER_MIKUS_RAY_H
#define MIKUS_LUCIA_COLLIDER_MIKUS_RAY_H

#include <Lucia/Collider/Collider.h>
#include <Lucia/Collider/Shape.h>
namespace Lucia {
class Collider::Ray: public Collider::Shape
{
    public:
        Ray();

        void make(Manager *M,Vertex a,Vertex b);
        void make(Manager *M,float x,float y,float z,float X,float Y,float Z){
            make(M,Vertex(x,y,z),Vertex(X,Y,Z));
        };
        void scale(float x,float y,float z){
            Vertex A = A + (A*Vertex(x,y,z));
            Vertex B = B + (B*Vertex(x,y,z));
            make(Parent,A,B);
        };
        void scaleTo(float x,float y,float z){
            Vertex A = A*Vertex(x,y,z);
            Vertex B = B*Vertex(x,y,z);
            make(Parent,A,B);
        };
        // for moving we must re-make the ray each time, otherwise we may have a conflict of translation
        void move(float x,float y,float z){
            Vertex A = A + Vertex(x,y,z);
            Vertex B = B + Vertex(x,y,z);
            make(Parent,A,B);
        };
        void moveTo(float x,float y,float z){
            Vertex A = Vertex(x,y,z);
            Vertex B = Vertex(x,y,z);
            make(Parent,A,B);
        };
        void setPoints(Vertex a,Vertex b)
        {
            make(Parent,a,b);
            sendMoveSignal();
        };
        Vertex getDistance(Shape *b,float range,bool a_is_first=true); // gets the contact point
        Vertex recrusiveContact(std::vector<Vertex> points,Shape *b,float minRange);

        bool collidesWith(std::shared_ptr<Shape> A);
        void draw();
        Vertex support(Vertex dir);

        std::pair<Vertex,Vertex> getVertex(){return std::pair<Vertex,Vertex>(A,B);};
        virtual ~Ray();
    protected:
    private:
        Vertex A;
        Vertex B;
};
}
#endif // COLLIDER_MIKUS_RAY_H
