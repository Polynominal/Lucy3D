#ifndef MAINSTATE_H
#define MAINSTATE_H
#include <utilities/OpenGL.h>

#include <State/Gamestate.h>
#include <Camera.h>
#include <string>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include <Controll/Keyboard.h>
#include <Graphics/Graphics.h>
#include <Graphics/Scene/Model.h>
#include <Graphics/Scene/Instance.h>
#include <Graphics/Text/Text.h>
#include <Graphics/Text/Instance.h>
#include <Graphics/Text/Task.h>

#include <Collider/Collider.h>
#include <Collider/Manager.h>
#include <Collider/Opengl.h>

#include <Collider/Shape/Polygon.h>
#include <Collider/Shape/Sphere.h>
#include <Collider/Shape/Point.h>
#include <Collider/Shape/Ray.h>

#include <Maigui/Maigui.h>
#include <Maigui/Manager.h>
#include <Maigui/Wrappers/OpenGL.h>
#include <Maigui/Types/Frame.h>
#include <Compatability/GLES2/Shaders/Basic2D.h>
#include <Settings.h>
#include <Maigui\Types\Button.h>

using namespace Graphics;

class MainGame : public Gamestate
{
    public:

        Utils::OpenGL::Buffer* Buff;
        Utils::OpenGL::Buffer* Buff2;
        float mx = 0;
        float my = 0;

        Collider::Manager* Manager = new Collider::Manager();
        Maigui::Manager  Ui = Maigui::Manager();
        Graphics::Text* text;
        Graphics::Text::Render* render=nullptr;
        Graphics::Text::Task* job;
        Graphics::Text::Instance* inst;

        Mesh Block = Mesh();
        int id;
        Keyboard keyboard = Keyboard();
        glm::vec3 positions[10] = {
          glm::vec3( 100.0f,  0.0f,  0.0f),
        };
        std::vector<std::shared_ptr<Collider::Shape>> Shapes;
        void  enter()
        {
           cam = (new Camera(Window->getWidth(),Window->getHeight()));
           cam2D = (new Camera(Window->getWidth(),Window->getHeight()));
            float vertices[] = {
                -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
                 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
                 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

                -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
                 0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
                 0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
                 0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
                -0.5f,  0.5f,  0.5f, 0.0f, 1.0f,
                -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,

                -0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
                -0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
                -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
                -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
                -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
                -0.5f,  0.5f,  0.5f, 1.0f, 0.0f,

                 0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
                 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
                 0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
                 0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
                 0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
                 0.5f,  0.5f,  0.5f, 1.0f, 0.0f,

                -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
                 0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
                 0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
                 0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
                -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
                -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

                // this is the top part.
                //-0.5f,  0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
                // 0.5f,  0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
                // 0.5f,  0.5f,  0.5f, -0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
                // 0.5f,  0.5f,  0.5f, -0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
                //-0.5f,  0.5f,  0.5f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
                //-0.5f,  0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f
            };

             auto Buffer = Block.addSubBuffer();
             Buffer->setShaderVars(Graphics::_Shaders::DefaultSceneShader->getVars());
             Buffer->setData(vertices,30);

//            Block.addImage("assets/Textures/container.jpg","ourTexture1");
//            Block.addImage("assets/Textures/smiley.png","ourTexture2");
//            Block.finalize();
//
//            //auto a = newScene.insert(&Block);
//            //a->scale(-0.5,-0.5,-0.5);

            auto Model =  new Graphics::Model();
            Model->fromFile("assets/Models/nanosuit/nanosuit.obj");
            Model->generate();
            auto b = newScene.insert(Model);


            cam->step(0.0f,0.0f,-3.0f);
            Collider_OpenGL::createWrappers(Manager,&cam->view,&cam->projection);

            auto m =  Manager->addBox(0,0,0,2,2,2);

            //auto d = Manager.addBox(-10,0,0,1,1,1);
            //d->rotate(2,10,6);
            auto circle = Manager->addSphere(Collider::Vertex(0,0,0),1);
            circle->scaleTo(1,3,0.1);
            //Shapes.push_back(circle);
            Manager->OnCollision = [](Collider::Shape* A,Collider::Shape* B){
                A->Color = Collider::Vertex(0,1,0);
                B->Color = Collider::Vertex(0,1,0);
            };
            Manager->OnRelease = [](Collider::Shape* A,Collider::Shape* B){
                A->Color = Collider::Vertex(0,0,0);
                B->Color = Collider::Vertex(0,0,0);
            };
            cam2D->setRange(45,0.01,1000);
            cam2D->step(-800/2,-600/2,-540);
            Maigui::OpenGL::attachWrappers(Ui.getSkin().get(),&cam2D->view,&cam2D->projection);
            Collider_OpenGL::createWrappers(Ui.getWorld(),&cam2D->view,&cam2D->projection);
            Ui.formDefaultSkin();

//            Ui.setGlobalScale(800.0f/1024.0f,600.0f/720.0f);

            Ui.getWorld()->OnCollision = [](Collider::Shape* A,Collider::Shape* B){
                A->Color = Collider::Vertex(0,1,0);
                B->Color = Collider::Vertex(0,1,0);
            };
            Ui.getWorld()->OnRelease = [](Collider::Shape* A,Collider::Shape* B){
                A->Color = Collider::Vertex(0,0,0);
                B->Color = Collider::Vertex(0,0,0);
            };
//            Ui.addButton(400,300,0.0f,300,50,1);
//            auto b = Ui.addButton(350,220,2.0f,300,50,1);
//            b->rotateTo(0.0f,30.0f,0.0f);
//            b->setText("Hello!");
//            b->setName("Button1");
            auto f = Ui.addFrame(350,220,2.0f,300,50,1);
            f->move(10,0,0);
            f->scaleTo(300,300,0);
            text = new Graphics::Text("assets/Fonts/libel.ttf");
            text->setProjection(&cam2D->projection);


            inst = new Graphics::Text::Instance(text);
            render = new Graphics::Text::Render(inst);

            job = *render << Graphics::Text::Present::MatchOut;
            job->setSpeed(0.001f);
            *job << "Hello world!";
        };
        void draw()
        {

             newScene.preDraw();
             cam->update(true);
             newScene.draw(&cam->view,&cam->projection);
             newScene.postDraw();
//
             cam2D->update(false);
             //Ui.draw();
            //Ui.drawSkeleton();
             //Manager.drawChunks(false);
        };

        void update(double delta_time)
        {
            auto p = cam2D->getRay(mx,my);
            auto Eye = p.first;
            auto Forward = p.second;
            Ui.setRay(Collider::Vertex(Eye.x,Eye.y,Eye.z),Collider::Vertex(Forward.x,Forward.y,Forward.z));
            Ui.update(delta_time);

            Manager->update();
            if (keyboard.isDown("w"))
            {
//                Window->setSize(1024,720);
//
//                cam2D->resize(1024,700);
//                cam2D->setPosition(0,0,-1);
                cam->strafe(cam->FRONT,0.1);

            };
            if (keyboard.isDown("s"))
            {
                cam->strafe(cam->BACK,0.1);
            };
            if (keyboard.isDown("a"))
            {
                cam->strafe(cam->LEFT,0.01);
            };
            if (keyboard.isDown("d"))
            {
                cam->strafe(cam->RIGHT,0.01);
            }
            if (keyboard.isDown("left shift"))
            {
                cam->strafe(cam->UP,2);
            }
            if (keyboard.isDown("left ctrl"))
            {
                cam->strafe(cam->DOWN,2);
            }
            if (keyboard.isDown("esc"))
            {
                exit(0);
            }
            if (keyboard.isDown("f"))
            {
                cam->pitch(3,false);
            }
            if (keyboard.isDown("q"))
            {
                cam->strafe(cam->ROLLDOWN,0.001);
            }
            if (keyboard.isDown("e")){
                cam->strafe(cam->ROLLUP,0.001);
            };
        };

        void keypressed(std::string key)
        {
            if (key == "escape")
            {
                exit(0);
            }else if(key == "x")
            {
                auto Shape = Shapes[0];
                Shape->remove();
            }else if (key == "left")
            {
                auto Shape = Shapes[0];
                Shape->move(-1,0,0);
            }else if (key == "right")
            {
                auto Shape = Shapes[0];
                Shape->move(1,0,0);
            }
            else if (key == "up")
            {
                auto Shape = Shapes[0];
                Shape->move(0,1,0);
            }
            else if(key == "down")
            {
                auto Shape = Shapes[0];
                Shape->move(0,-1,0);
            };
        };
        void mousepressed(std::string key,int x,int y)
        {
            Ui.mousepressed(key);
        };
        void mousereleased(std::string key, int x ,int y )
        {
            Ui.mousereleased(key);
        };
        void mousemotion(int x,int y,int relx,int rely)
        {
            mx = x;
            my = y;
            //cam->mousemotion(x,y,relx,rely);
        }
        ~MainGame(){delete cam;delete cam2D;};
    private:
        GLuint VBO;
        Scene::Instance newScene = Scene::Instance();
        Camera* cam;
        Camera* cam2D;

};
#endif
