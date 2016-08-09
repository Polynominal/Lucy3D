#include "Lucia\Graphics\Primitives\Quad.h"
using namespace Lucia;
typedef Graphics::Primitive::Quad Quad;
Quad::Quad()
{

}
void Quad::initTasks(int id)
{
    auto SVars = std::make_shared<Shader_Vars>();
    SVars->setProgram(id);
    SVars->add("vertex",0,2);
    SVars->add("uv",2,4);
    if (Buff.get() != nullptr)
    {
        Buff->destroy();
    }
    Buff.reset(new Buffer(SVars));
    Buff->allocateData(6);
    init = true;
}
void Quad::generate(unsigned int id)
{
    if (id == 0){initTasks(Graphics::_Shaders::Basic2D->programID);}else{initTasks(id);};
    Data.clear();
    Data.push_back(std::make_shared<Vertex_Buffer>(-0.5f, -0.5f,  0.0f, 0.0f));
    Data.push_back(std::make_shared<Vertex_Buffer>(0.5f, -0.5f, 1.0f, 0.0f));
    Data.push_back(std::make_shared<Vertex_Buffer>(0.5f,  0.5f, 1.0f, 1.0f));

    Data.push_back(std::make_shared<Vertex_Buffer>(0.5f,  0.5f, 1.0f, 1.0f));
    Data.push_back(std::make_shared<Vertex_Buffer>(-0.5f,  0.5f, 0.0f, 1.0f));
    Data.push_back(std::make_shared<Vertex_Buffer>(-0.5f, -0.5f, 0.0f, 0.0f));
    // Update content of VBO memory
    Buff->sendData(Data,0);
}
void Quad::generate(float w,float h,float saw,float sah,float bx,float by,float tx,float ty)
{
    initTasks(Graphics::_Shaders::Basic2D->programID);
    float degree_x;
    float degree_y;

    float topx =1.0f;
    float topy =1.0f;
    float sh=saw;
    float sw=sah;
    if (sh == 0.0f or sw == 0.0f)
    {
        GLint d[4];
        glGetIntegerv(GL_VIEWPORT,(GLint*)&d);
        sw = d[2];
        sh = d[3];
    }
    degree_x = ((1.0f/sw));
    degree_y = ((1.0f/sh));

    if (w != 0.0f or h != 0.0f)
    {
        topx = (degree_x*w);
        topy = (degree_y*h);
    }

    float bottomx=-1.0f;
    float bottomy=-1.0f;
    // 0+-----+3
    //  | \   |
    //  |  \  |
    //  |   \ |
    //  |    \|
    // 1+-----+2
    // 0,1,2, 0,2,3
    if (centeredCordinates)
    {
        topx = topx;
        topy = topy;
        bottomx = -topx;
        bottomy = -topy;
    }else
    {
        bottomx = -1.0f;
        bottomy = -1.0f;
        topx = bottomx + (topx*2.0f);
        topy = bottomy + (topy*2.0f);
    };
    Data.clear();
    Data.push_back(std::make_shared<Vertex_Buffer>(bottomx,bottomy,  bx, by));
    Data.push_back(std::make_shared<Vertex_Buffer>(topx,bottomy,     tx, by));
    Data.push_back(std::make_shared<Vertex_Buffer>(topx,topy,        tx, ty));

    Data.push_back(std::make_shared<Vertex_Buffer>(topx,topy,        tx, ty));
    Data.push_back(std::make_shared<Vertex_Buffer>(bottomx,topx,     bx, ty));
    Data.push_back(std::make_shared<Vertex_Buffer>(bottomx,bottomy,  bx, by));
    // Update content of VBO memory
    Buff->sendData(Data,0);
}
void Quad::draw()
{
      Buff->draw(GL_TRIANGLES);
};
void Quad::draw(std::shared_ptr<Shader_Vars> vars)
{
    Buff->draw(GL_TRIANGLES,vars.get());
}
void Quad::setImageRange(float bx,float by,float tx,float ty)
{
    if (!init)
    {
        generate(false);
    }
    Data[0]->Data[2] = bx;  Data[0]->Data[3] = by;
    Data[1]->Data[2] = tx;  Data[1]->Data[3] = by;
    Data[2]->Data[2] = tx;  Data[2]->Data[3] = ty;

    Data[3]->Data[2] = tx;  Data[3]->Data[3] = ty;
    Data[4]->Data[2] = bx;  Data[4]->Data[3] = ty;
    Data[5]->Data[2] = bx;  Data[5]->Data[3] = by;
    // Update content of VBO memory
    Buff->sendData(Data,0);
};
void Quad::remove()
{
    Buff->destroy();
}
Quad::~Quad()
{
    // YOU MUST AT ALL TIMES CALL REMOVE BEFORE DESTRUCTOR!!
    //remove();
    //dtor
}
namespace Lucia {
    namespace Graphics
    {
        namespace Shared
        {
            std::unique_ptr<Primitive::Quad> Quad = std::unique_ptr<Primitive::Quad>(new Primitive::Quad);
        }
    }
}
