#include "Camera.h"
#include <SDL.h>
#include <iostream>
using namespace Graphics;
using namespace glm;
Camera::Camera(float w,float h)
{
    width = w;
    height = h;
    setRange(fov,minRange,maxRange);
    //ctor
}
void Camera::setShader(GLuint nshaderID)
{
    shaderID = nshaderID;
}
void Camera::step(float nx,float ny, float nz)
{
    Eye.x -= nx;
    Eye.y -= ny;
    Eye.z -= nz;
    createMatrix = true;
};

void Camera::setPosition(float x,float y,float z)
{
    Eye.x = x; Eye.y = y; Eye.z = z;
    createMatrix = true;
};
void Camera::update(bool b)
{

    if (b)
    {
        GLint programID = 0;
        glGetIntegerv(GL_CURRENT_PROGRAM,&programID);
        GLuint var=0;
        var = glGetUniformLocation(programID,"view");
        glUniformMatrix4fv(var,1,GL_FALSE,glm::value_ptr(this->getView()));

        var = glGetUniformLocation(programID,"projection");
        glUniformMatrix4fv(var,1,GL_FALSE,glm::value_ptr(projection));
    }else
    {
        this->getView();
    }


}; // applies all of the translations and sends data to vertex shader .
void Camera::strafe(bool direction)
{

}
void Camera::strafe(int direction,float speed)
{
    switch(direction)
    {
        case(RIGHT):
            {
                this->translateLocal(-speed);
                break;
            }
        case(LEFT):
            {
                this->translateLocal(speed);
                break;
            }
        case(FRONT):
            {
                this->translateLocal(0.0f,0.0f,speed);
                break;
            }
        case(BACK):
            {
                this->translateLocal(0.0f,0.0f,-speed);
                break;
            }
        case(UP):
            {
                this->translateLocal(0.0f,speed,0.0f);
                break;
            }
        case(DOWN):
            {
                this->translateLocal(0.0f,-speed,0.0f);
                break;
            }
        case(ROLLUP):
            {
                this->updateRoll(speed);
                break;
            }
        case(ROLLDOWN):
            {
                this->updateRoll(-speed);
                break;
            };
    }
}

void Camera::mousemotion(Windower::Window *W,float x,float y,float relx,float rely)
{


    float yawAngle = 3.14159265358979323846 * 0.5 * 0.001;
    float pitchAngle = -rely * yawAngle;

    this->pitch(pitchAngle,true);
    this->rotateByVec(-relx * yawAngle,yawAxis.x,yawAxis.y,yawAxis.z);

    createMatrix = true;
    W->lockMouseRelative(true);
}
// returns a ray from where the mouse is pointing
std::pair<glm::vec3,glm::vec3> Camera::getRay(float x,float y)
{
    glm::vec3 eye = Eye;
    glm::vec3 world;
    if (orthographic){eye = glm::vec3(x,height-y,0.0f)*Orientation; world = glm::vec3(x,height-y,maxRange*10000000)*Orientation;}
    else{
        glm::vec4 port = glm::vec4(0.0f,0.0f,width,height);
        glm::vec3 screen = glm::vec3(x,height-y,1.0f);
        world = glm::unProject(screen,view,projection,port);
    }
    //            far
    // [camera] >-----> B Aligned based on orientation
    //
    //      [camera]
    //         |
    //         |
    //         |
    //        \ /
    //         -
    return std::pair<glm::vec3,glm::vec3>(eye,world);
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
    projection = glm::perspective(a,float(width/height),minRange,maxRange);
}
void Camera::resize(float width,float height)
{
    if (!orthographic)
    {
        projection = glm::perspective(fov,float(width/height),minRange,maxRange);
    }else
    {
        projection = glm::ortho(0.0f,width,0.0f,height,0.0f,maxRange*10000000);
    }

    createMatrix = true;
}
void Camera::setOrtho()
{
    orthographic = true;
    projection = glm::ortho(0.0f,width,0.0f,height,0.0f,maxRange*10000000);
    createMatrix = true;
}
void Camera::setPerspective()
{
    orthographic = false;
    projection = glm::perspective(fov,float(width/height),minRange,maxRange);
    createMatrix = true;
}
mat4 Camera::getView()
{
    if (createMatrix)
    {
        quat q = Orientation;
        q.x *= -1.0f;
        q.y *= -1.0f;
        q.z *= -1.0f;
        view = mat4_cast(q);

        vec3 v = -Eye;
        mat4 m = view;
        view[3] = (m[0] * v[0]) + (m[1] * v[1]) + (m[2] * v[2]) + m[3];

        createMatrix  = false;
    }
    return view;
}
mat4 Camera::getProjection()
{
    return projection;
}

// private
void Camera::normal()
{
    Local = normalize(Local);
    Up = normalize(Up);
    Forward = normalize(Forward);
    Orientation = normalize(Orientation);

    Local = cross(Up,Forward);
    Up = cross(Forward,Local);
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
// Rotation functions via degree angles, they must be done one by one!

void Camera::pitch(float degrees,bool radians)
{
    float rad = 0;
    if (!radians)
    {
        rad = glm::radians(degrees);
    }
    else
    {
        rad = degrees;
    }
    quat q = angleAxis(rad,-Local);
    Up    = glm::rotate(q,Up);
    Local = glm::rotate(q,Local);

    Orientation = q * Orientation;

    regRot();
    createMatrix  = true;
};
void Camera::yaw(float degrees)
{
    float rad = radians(degrees);
    quat q = angleAxis(rad,Up);
    Local   = glm::rotate(q,Local);
    Forward = glm::rotate(q,Forward);

    Orientation = q * Orientation;

    regRot();
    createMatrix  = true;
};
void Camera::roll(float degrees)
{
    float rad = radians(degrees);
    quat q = angleAxis(rad,Forward);
    Up    = glm::rotate(q,Up);
    Forward = glm::rotate(q,Forward);

    Orientation = q * Orientation;

    regRot();
    createMatrix  = true;
};
void Camera::setRotation(float yaw,float pitch,float roll,bool radians)
{

};
void Camera::rotateByVec(float angle,float x,float y,float z)
{
    vec3 axis = vec3(x,y,z);
    vec3 n = normalize(axis);

    quat q = angleAxis(angle,n);

    Local   = glm::rotate(q,Local);
    Up      = glm::rotate(q,Up);
    Forward = glm::rotate(q,Forward);
    Orientation =  q * Orientation;

    regRot();
    createMatrix = true;
};
void Camera::updateRoll(float speed)
{

    this->roll(glm::degrees(speed));
    glm::quat q = glm::angleAxis(speed,Forward);
    yawAxis = glm::rotate(q,yawAxis);
}
void Camera::translateLocal(float left,float up,float forward)
{
    Eye += Up * up;
    Eye += Local*left;
    Eye += (Orientation * glm::vec3(0.0f,0.0f,-1.0f))*(forward);
    createMatrix = true;
}
Camera::~Camera()
{
    //dtor
}
