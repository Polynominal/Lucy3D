#include "Lucia\Graphics/Buffers/Mesh.h"
using namespace Lucia;
using namespace Graphics;
Mesh::Mesh()
{
    setShaderVars(_Shaders::DefaultSceneShader->getVars());
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
        LOG << "Fatal" << "Out of Texture Memory!!" << std::endl;
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
    if (ShaderVars.get() == nullptr){ShaderVars = Graphics::_Shaders::DefaultSceneShader->getVars();};
    Buff* b = new Buff(ShaderVars);
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
void Mesh::render(Maths::Matrix<4>* model,DRAW draw_mode)
{
    ShaderVars->sendMatrix("model_view",4,4,false,model->unpack());
    int mech = getMechanicalTarget(draw_mode);
    for (unsigned int i=0; i < Images.size(); i++)
    {
        Images[i]->bind(i);
    };
    if (Cores.size() != 0)
    {
        int last_id = 0;
        Cores[0]->attach();
        ShaderVars->apply();
        for (auto v: Cores)
        {
            int nid = v->getShaderVars()->getID();
            if (nid != last_id)
            {
                v->getShaderVars()->use();
                SwapShaders(nid,model);
                last_id = nid;
            };
            v->attach();
            v->render(mech);
        };
        Cores[0]->detach();
    };
    for (auto v: Children)
    {
        //add local transformation here in the future maybe.
        v->render(model,draw_mode);
    }
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
