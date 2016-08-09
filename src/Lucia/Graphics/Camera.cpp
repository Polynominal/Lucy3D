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
void Camera::update(bool b)
{
    applyTranslations();
    view = Model_Matrix;
    
    if (b)
    {
        GLint programID = 0;
        glGetIntegerv(GL_CURRENT_PROGRAM,&programID);
        GLuint var=0;
        var = glGetUniformLocation(programID,"view");
        glUniformMatrix4fv(var,1,GL_FALSE,this->getView().unpack());

        var = glGetUniformLocation(programID,"projection");
        glUniformMatrix4fv(var,1,GL_FALSE,projection.unpack());
    }else
    {
        this->getView();
    }


}; // applies all of the translations and sends data to vertex shader .
void Camera::pitch(float degrees)
{
    auto rad = radians(degrees);
    auto q = Quaternion(rad,-Local);
    Up    = q.rotate(Up);
    Local = q.rotate(Local);

    Rotation = q * Rotation;

    regRot();
};
void Camera::yaw(float degrees)
{
    float rad = radians(degrees);
    auto q = Quaternion(rad,Up);
    Local   = q.rotate(Local);
    Forward = q.rotate(Forward);

    Rotation = q * Rotation;

    regRot();
};
void Camera::roll(float degrees)
{
    float rad = radians(degrees);
    auto q = Quaternion(rad,Forward);
    Up    = q.rotate(Up);
    Forward = q.rotate(Forward);

    Rotation = q * Rotation;

    regRot();
};
void Camera::strafe(std::string direction)
{

}
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
            this->translateLocal(0.0f,-speed,0.0f);
        }
    else if (direction == "down")
        {
            this->translateLocal(0.0f,speed,0.0f);
        }
    else if (direction == "roll_left")
        {
            this->updateRoll(speed);
        }
    else if (direction == "roll_right")
        {
            this->updateRoll(-speed);
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
void Camera::lookAt(float x,float y,float z)
{

};
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
    view = Model_Matrix;
    if (createMatrix)
    {
//        pitch(Rotation.x);
//        yaw(Rotation.y);
//        roll(Rotation.z);
//        
//        Quaternion q = Rotation;
//        q.x *= -1.0f;
//        q.y *= -1.0f;
//        q.z *= -1.0f;
//        view.rotate(q.toAxis());

//        Vertex v = -Position;
//        Matrix<4> m = view;
//        view[0][3] = (m[0][0] * v.x) + (m[0][1] * v.y) + (m[0][2] * v.z) + m[0][3];

        createMatrix  = false;
    }
    
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
void Camera::regRot()
{
    rotations++;
    if (rotations > max_rotations)
    {
        rotations = 0;
        this->normal();
    };
}
void Camera::updateRoll(float speed)
{
    rotate(0,0,speed);
    
    Quaternion q = Quaternion(speed,Forward);
    yawAxis = q.rotate(yawAxis);
    
    applyTranslations();
    view = Model_Matrix;
    createMatrix = true;
}
void Camera::translateLocal(float left,float up,float forward)
{
    Position += Up * up;
    Position += Local*left;
    Position += (Rotation * Vertex(0.0f,0.0f,-1.0f))*(forward);
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
