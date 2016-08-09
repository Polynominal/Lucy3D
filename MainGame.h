#ifndef MAINSTATE_H
#define MAINSTATE_H
#include <string>
#include <Lucia/Utils/OpenGL.h>
#include <Lucia/State/Gamestate.h>
#include <Lucia/Graphics/Camera.h>
#include <Lucia/Graphics/Graphics.h>
#include <Lucia/Graphics/Scene/Model.h>
#include <Lucia/Graphics/Scene/Instance.h>
#include <Lucia/Graphics/Text/Text.h>
#include <Lucia/Graphics/Text/Instance.h>
#include <Lucia/Graphics/Text/Task.h>
#include <Lucia/Graphics/Buffers/Mesh.h>
#include <Lucia/Graphics/Buffers/Spritesheet.h>
#include <Lucia/Graphics/Scene/Scene2D.h>

using namespace Lucia;
using namespace Graphics;
class MainGame : public Gamestate
{
    public:

        std::shared_ptr<Mesh> Block = std::make_shared<Mesh>();

        void enter()
        {
            newScene->generate(Window);
//            auto Buffer = Block->addSubBuffer();
//            Buffer->setShaderVars(Graphics::_Shaders::DefaultSceneShader->getVars());
//            Buffer->setData(vertices,30);
//
//            Block->addImage("assets/Textures/container.jpg","ourTexture1");
//            Block->addImage("assets/Textures/smiley.png","ourTexture2");
//            Block->finalize();

            //auto a = newScene->addObject(Block);

            //auto sprite = std::make_shared<Graphics::Buffer::Spritesheet>();
            //sprite->load("assets/Animation/bunnytest.png");

            auto model = std::make_shared<Model>();
            model->fromFile("assets/Models/Plate/basic.obj");
            model->generate();
            auto ab = newScene->addObject(model);
            ab->moveTo(0.0f,0.0f,0.0f);
            ab->scaleTo(1.0f,1.0f,1.0f);
            //auto otherObject = newScene->addObject(model);
            //otherObject->moveTo(2.0f,0.0f,0.0f);
            //otherObject->scaleTo(1.0f,1.0f,1.0f);
//            for (auto i =0;i < 100; i++)
//            {
//                auto obj = newScene->addObject(model);
//                obj->moveTo(2.0f*i,0.0f,0.0f);
//                obj->scaleTo(1.0f,1.0f,1.0f);
//            }

        };
        void draw()
        {
             newScene->draw();
        };

        void update(double delta_time)
        {
            newScene->update(delta_time);
        };

        void keypressed(std::string key)
        {
            auto cam = newScene->getCamera();
            if (key == "w"){cam->strafe("forward",0.5f);std::cout << cam->getEye().z << std::endl;}
            else if (key == "s"){cam->strafe("backward",0.5f);;std::cout << cam->getEye().z << std::endl;}
            else if (key == "a"){cam->strafe("left",0.5f);}
            else if (key == "d"){cam->strafe("right",0.5f);}
            else if (key == "left ctrl"){cam->strafe("down",0.5f);}
            else if (key == "left shift"){cam->strafe("up",0.5f);};

        };
        void mousepressed(std::string key,int x,int y)
        {

        };
        void mousereleased(std::string key, int x ,int y )
        {

        };
        void mousemotion(int x,int y,int relx,int rely)
        {

        }
        ~MainGame()
        {
            delete newScene;
        };
    private:
        GLuint VBO;
        Scene::Scene2D* newScene = new Scene::Scene2D();

};
#endif
