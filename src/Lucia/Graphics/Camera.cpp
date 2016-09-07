#include "Lucia\Graphics\Camera.h"
#include <SDL.h>
#include <iostream>
using namespace Lucia;
using namespace Graphics;
using namespace Maths;

Camera::Camera(float w,float h)
{
    width = w;
    height = h;
    setRange(fov,minRange,maxRange);
    rotate(0,0,0);
    //ctor
}
//Massive thanks to joe stevens for mentioning this method: http://joestevens.net/post/20063172257/alternate-view-matrix-construction
void Camera::lookAt(Quaternion rot, Vertex Pos)
{
    view = Matrix<4>();
    
    Vertex v1 = (rot*Vertex(0.0f,0.0f,1.0f)).normalize(); // foeward
    Vertex v2 = (rot*Vertex(0.0f,1.0f,0.0f)).normalize().cross(v1); // up
    Vertex v3 = v1.cross(v2);
    
    view[0][0] = v2.x;
    view[0][1] = v3.x;
    view[0][2] = v1.x;
    
    view[1][0] = v2.y;
    view[1][1] = v3.y;
    view[1][2] = v1.y;
    
    view[2][0] = v2.z;
    view[2][1] = v3.z;
    view[2][2] = v1.z;
    
    view[3][0] = -v2.dot(Pos);
    view[3][1] = -v3.dot(Pos);
    view[3][2] = -v1.dot(Pos);
    
    view = view.transpose();
}
void Camera::lookAt(Vertex eye, Vertex target, Vertex up)
{
    view = Matrix<4>();
    Vertex f = (eye - target).normalize();    
    Vertex s = (up.cross(f)).normalize();
    Vertex u = f.cross(s);     
    
    view[0][0] = s.x;
    view[1][0] = s.y;
    view[2][0] = s.z;
    view[0][1] = u.x;
    view[1][1] = u.y;
    view[2][1] = u.z;
    view[0][2] = f.x;
    view[1][2] = f.y;
    view[2][2] = f.z;
    view[3][0] = -s.dot(eye);
    view[3][1] = -u.dot(eye);
    view[3][2] = -f.dot(eye);
    
    view = view.transpose();
}
void Camera::update()
{
    lookAt(Rotation,Position);
}; // applies all of the translations and sends data to vertex shader .
void Camera::strafe(std::string direction,float speed)
{
    if (direction == "right")
        {
            this->translateLocal(speed);
        }
    else if (direction == "left")
        {
            this->translateLocal(-speed);
        }
    else if (direction == "forward")
        {
            this->translateLocal(0.0f,0.0f,speed);
        }
    else if (direction == "backward")
        {
            this->translateLocal(0.0f,0.0f,-speed);
        }
    else if (direction == "up")
        {
            this->translateLocal(0.0f,speed,0.0f);
        }
    else if (direction == "down")
        {
            this->translateLocal(0.0f,-speed,0.0f);
        }
    else if (direction == "roll_left")
        {
            rotate(-speed,0,0);
        }
    else if (direction == "roll_right")
        {
            rotate(speed,0,0);
        }
    else if (direction == "pitch_up")
    {
        rotate(0,0,speed);
    }
    else if (direction == "pitch_down")
    {
        rotate(0,0,-speed);
    };
}

void Camera::mousemotion(Context::Window *W,float x,float y,float relx,float rely)
{
    float yawAngle = 3.14159265358979323846 * 0.5 * 0.001;
    float pitchAngle = degrees(-rely * yawAngle);
    float mod = -relx * yawAngle;
    
    rotate(pitchAngle,0,0);
    rotateTo(yawAxis.x*mod,yawAxis.y*mod,yawAxis.z*mod);
    
    W->lockMouseRelative(true);
}
// returns a ray from where the mouse is pointing
std::pair<Vertex,Vertex> Camera::getRay(float x,float y)
{
    Vertex eye = Position;
    Vertex world;
    if (orthographic){eye = Rotation*Vertex(x,height-y,0.0f); world = Rotation*Vertex(x,height-y,maxRange*10000000);}
    else{
        Vector port = Vector(0.0f,0.0f,width,height);
        Vertex screen = Vertex(x,height-y,1.0f);
        
        world = projection.unProject(screen,view,port);
    }
    //            far
    // [camera] >-----> B Aligned based on Rotation
    //
    //      [camera]
    //         |
    //         |
    //         |
    //        \ /
    //         -
    return std::pair<Vertex,Vertex>(eye,world);
}
// translations.
void Camera::setRange(float a,float min,float max)
{
    fov = a;
    maxRange = max;
    minRange = min;
    projection = Perspective(a,float(width/height),minRange,maxRange);
}
void Camera::resize(float width,float height)
{
    if (!orthographic)
    {
        projection = Perspective(fov,float(width/height),minRange,maxRange);
    }else
    {
        projection = Ortho(0.0f,width,0.0f,height,0.0f,maxRange*10000000);
    }

    createMatrix = true;
}
void Camera::setOrtho()
{
    orthographic = true;
    projection = Ortho(0.0f,width,0.0f,height,0.0f,maxRange*10000000);
    createMatrix = true;
}
void Camera::setPerspective()
{
    orthographic = false;
    projection = Perspective(fov,float(width/height),minRange,maxRange);
    createMatrix = true;
}
Matrix<4> Camera::getView()
{
    lookAt(Rotation,Position);
    return view;
}
Matrix<4> Camera::getProjection()
{
    return projection;
}

// private
void Camera::normal()
{
    Local = Local.normalize();
    Up = Up.normalize();
    Forward = Forward.normalize();
    Rotation = Rotation.normalize();

    Local = Up.cross(Forward);
    Up = Forward.cross(Local);
    yawAxis = Up;
}
void Camera::translateLocal(float left,float up,float forward)
{
    Local       = (Rotation*Vertex(1.0f,0.0f,0.0f)).normalize();
    Up          = (Rotation*Vertex(0.0f,1.0f,0.0f)).normalize();
    Forward     = (Rotation*Vertex(0.0f,0.0f,1.0f)).normalize();
    
    Position += Up * up;
    Position += Local*left;
    Position += Forward*(forward);
    createMatrix = true;
    //needed to refresh the move stack.
    moveTo(Position);
    applyTranslations();
    view = Model_Matrix;
}
Camera::~Camera()
{
    //dtor
}