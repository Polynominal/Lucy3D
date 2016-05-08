#include "Graphics/Buffers/Mesh.h"
#include <Graphics/Bases/Moveable.h>
using namespace Graphics;
Mesh::Mesh()
{
}
void Mesh::generate()
{
    for (unsigned int i=0; i < Images.size(); i++)
    {
        Images[i]->generate();
    };
};
void Mesh::addImage(Image img)
{
    Images.push_back(&img);
};
void Mesh::addImage(std::string path)
{
    Image* texture  = new Image(path);
    if(!texture)
    {
        std::cout << "Out of Texture Memory!!" << std::endl;
    }else
    {
        Images.push_back(texture);
    };
    cleanTexture = true;
};
void Mesh::addImage(std::string path,std::string reference_name)
{
    this->addImage(path);
    Images[Images.size()-1]->setName(reference_name);
}
void Mesh::addImage(Image img,std::string reference_name)
{
    this->addImage(img);
    Images[Images.size()-1]->setName(reference_name);
}
void Mesh::addSubBuffer(Buff* b)
{
    Cores.push_back(b);
}
Buff* Mesh::addSubBuffer()
{
    if (Vars.get() == nullptr){Vars = Graphics::_Shaders::DefaultSceneShader->getVars();};
    Buff* b = new Buff(Vars);
    Cores.push_back(b);
    return b;
}
void Mesh::addSubMesh(Mesh* m)
{
    Children.push_back(m);
}
void Mesh::finalize()
{
    for (unsigned int i=0; i < Images.size(); i++)
    {
        Images[i]->generate();
    };
}
void Mesh::render(int id)
{
    refresh();
    for (unsigned int i=0; i < Images.size(); i++)
    {
        Images[i]->bind(i);
    };
    if (Cores.size() != 0)
    {
        int last_id = 0;
        Cores[0]->attach();
        Vars->apply();
        for (auto v: Cores)
        {
            int id = v->getShaderVars()->getID();
            if (id != last_id)
            {
                v->getShaderVars()->use();
                SwapShaders(id,&Model_Matrix);
                last_id = id;
            };
            v->attach();
            v->render(id);
        };
        Cores[0]->detach();
    };
    for (auto v: Children)
    {
        v->render(id);
    }
}
void Mesh::draw(bool a)
{
    //if (texture) {texture.bind();};
    render();
}
void Mesh::destroy(bool use_delete)
{
    if (Cores.size() != 0)
    {
        if (use_delete)
        {
            for (Image* v: Images)
            {
                delete v;
            };
            for (Buff* v: Cores)
            {
                delete v;
            };
        }
        Images.clear();
        Cores.clear();
    }
}
Mesh::~Mesh()
{
    destroy();
    //dtor
}
