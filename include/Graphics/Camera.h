#ifndef CAMERA_H
#define CAMERA_H
//GLM includes used for transformation maths.
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <gtx/transform.hpp>
#include <gtx/quaternion.hpp>
#include <gtc/quaternion.hpp>
// GLEW for gl stuff.

#include <Graphics/Graphics.h>
#include <Window/Windower.h>

class Graphics::Camera
    {
        public:
            enum Directions{
                LEFT,RIGHT,FRONT,BACK,UP,DOWN,ROLLUP,ROLLDOWN
            };
            Camera(float w,float h);

            void setShader(GLuint shaderID);
            glm::mat4 getView();
            glm::mat4 getProjection();

            void setRange(float a,float near,float far);
            void resize(float width,float height);

            void setOrtho();
            void setPerspective();

            void setRotation(float yaw=0,float pitch=0,float roll=0,bool radians=false);
            void rotate(float yaw,float pitch,float roll);
            void yaw(float degrees); // side to side
            void pitch(float degrees,bool radians=false); // up-down
            void roll(float degrees); // do a barrel roll.

            void rotateByVec(float angle,float x,float y,float z);

            void step(float x=0,float y=0, float z=0); //[short cut] adds to the current translation.

            void setPosition(float x,float y,float z); // sets the point of the watcher

            void mousemotion(Windower::Window*,float x,float y,float relx,float rely);

            void strafe(bool direction);

            void strafe(int direction,float speed);

            void translateLocal(float left=0,float up=0,float forward=0);

            void lookAt(float x,float y,float z); // sets the object the watcher is looking at
            std::pair<glm::vec3,glm::vec3> getRay(float x,float y); // gets the ray from mouse.

            void update(bool send_to_shader=true); // applies all of the translations and sends data to vertex .

            void key_controll(const char* key);
            void mouse_controll(const char* button);

            void target(float x,float y,float z);


            void updateRoll(float speed);


            // gets!
            glm::vec3 getForward(){return Forward;};
            glm::vec3 getLocal(){return Local;};
            glm::vec3 getUp(){return Up;};
            glm::vec3 getEye(){return Eye;};

            glm::quat getOrientation(){return Orientation;};
            float getFov(){return fov;};
            float getNear(){return minRange;};
            float getFar(){return maxRange;};

            // public
            glm::mat4 view;
            glm::mat4 projection;

            virtual ~Camera();
        protected:
        private:
            bool orthographic = false;
            bool createMatrix = false;

            float fov = glm::radians(40.0f);
            float minRange = 0.1f;
            float maxRange = 100.0f;

            int rotations = 0;
            int max_rotations = 10;

            float mx = 0;
            float my = 0;

            float width = 0;
            float height = 0;

            glm::vec3 yawAxis     = glm::vec3(0.0f,1.0f,0.0f);;

            glm::quat Orientation = glm::quat(1,0,0,0);

            glm::vec3 Eye         = glm::vec3(0.0f,0.0f,0.0f);
            glm::vec3 Local       = glm::vec3(-1.0f,0.0f,0.0f);
            glm::vec3 Up          = glm::vec3(0.0f,1.0f,0.0f);
            glm::vec3 Forward     = glm::vec3(0.0f,0.0f,-1.0f);

            GLuint shaderID;
            bool clamped = false;

            void normal();
            void regRot();
            void reset();
    };

#endif // CAMERA_H
