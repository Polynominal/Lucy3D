#ifndef MIKUS_LUCIA_GRAPHICS_CAMERA_H
#define MIKUS_LUCIA_GRAPHICS_CAMERA_H

#include <Lucia/Maths/Vertex.h>
#include <Lucia/Maths/Matrix.h>
#include <Lucia/Maths/Quaternion.h>
#include <Lucia/Maths/Moveable.h>

#include <Lucia/Graphics/Graphics.h>
#include <Lucia/Window/Context.h>

#include <Lucia/Graphics/Bases/ShaderHolder.h>
namespace Lucia {
class Graphics::Camera:
    public Graphics::Base::ShaderHolder,
    public Maths::Moveable
    {
        public:
            Camera(float w,float h);

            Matrix<4> getView();
            Matrix<4> getProjection();
            
            void setRange(float a,float near,float far);
            void setOrtho();
            void setPerspective();

            void resize(float width,float height);
            void mousemotion(Context::Window*,float x,float y,float relx,float rely);
            void strafe(std::string direction);
            void strafe(std::string direction,float speed);

            void translateLocal(float left=0,float up=0,float forward=0);

            void lookAt(float x,float y,float z); // sets the object the watcher is looking at
            std::pair<Vertex,Vertex> getRay(float x,float y); // gets the ray from mouse.

            void update(bool send_to_shader=true); // applies all of the translations and sends data to vertex .

            void key_controll(const char* key);
            void mouse_controll(const char* button);

            void target(float x,float y,float z);


            void updateRoll(float speed);


            // gets!
            
            Vertex getForward(){return Forward;};
            Vertex getLocal(){return Local;};
            Vertex getUp(){return Up;};
            Vertex getEye(){return Position;};

            float getFov(){return fov;};
            float getNear(){return minRange;};
            float getFar(){return maxRange;};

            // public
            void onMorph(){createMatrix = true;};
            Matrix<4> view = Matrix<4>();
            Matrix<4> projection = Matrix<4>();

            virtual ~Camera();
        protected:
        private:
            void pitch(float p);
            void yaw(float y);
            void roll(float r);
            bool orthographic = false;
            bool createMatrix = false;

            float fov = radians(40.0f);
            float minRange = 0.1f;
            float maxRange = 100.0f;

            int rotations = 0;
            int max_rotations = 10;

            float mx = 0;
            float my = 0;

            float width = 0;
            float height = 0;

            Vertex yawAxis     = Vertex(0.0f,1.0f,0.0f);;
            
            Vertex Local       = Vertex(-1.0f,0.0f,0.0f);
            Vertex Up          = Vertex(0.0f,1.0f,0.0f);
            Vertex Forward     = Vertex(0.0f,0.0f,-1.0f);

            bool clamped = false;

            void normal();
            void regRot();
            void reset();
    };
}
#endif // CAMERA_H
