#include "Lucia\Graphics\Objects\Model.h"

using namespace Lucia;
using namespace Graphics;
using namespace Object;

namespace Lucia {
    namespace Graphics
    {
        namespace _Shaders
        {
            std::shared_ptr<Graphics::Shader> StandardModelShader;
        }
    }
}
Model::Model()
{
    if (Graphics::_Shaders::StandardModelShader.get() == nullptr)
    {
        Graphics::_Shaders::StandardModelShader = std::make_shared<Graphics::Shader>();
        Graphics::_Shaders::StandardModelShader->fromFile("assets/Shaders/StandardModelShader.vert",
                          "assets/Shaders/StandardModelShader.frag","StandardModelShader");

        Graphics::_Shaders::StandardModelShader->addVar("position",0,3);
        Graphics::_Shaders::StandardModelShader->addVar("normal",3,6);
        Graphics::_Shaders::StandardModelShader->addVar("texture",6,8);
        auto vars = Graphics::_Shaders::StandardModelShader->getVars();
        vars->use();
        vars->send("Color",1.0f,1.0f,1.0f,1.0f);
        
    }
    ShaderVars = Graphics::_Shaders::StandardModelShader->getVars();
    //ctor
}
void Model::fromFile(string path)
{
    AssImporter.FreeScene();
    AssScene = AssImporter.ReadFile(path,aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_GenUVCoords);
    LOG << "Building";
    LOG << "New Model: " << path << std::endl;
    if(!AssScene or AssScene->mFlags == AI_SCENE_FLAGS_INCOMPLETE or !AssScene->mRootNode)
    {
        LOG << "Fatal";
        LOG << "--- IMPORT OF : " << path << " FAILED----" << std::endl;
        LOG << "ERROR:" << AssImporter.GetErrorString() << std::endl;
        LOG << "-----------------------------------------" << std::endl;
        Controll::Safety::Handler::sendErrorSignal("Import of: " + path + " failed: " + AssImporter.GetErrorString());
        return;
    }
    Directory = path.substr(0, path.find_last_of('/'));
};
void Model::processNode(aiNode* node)
{
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = AssScene->mMeshes[node->mMeshes[i]];
        DataSegments.push_back(processMesh(mesh));
    }
    // Then do the same for each of its children
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i]);
    }
};
void Model::processMaterial(aiMaterial* mat,std::vector<Model::TData> *v,aiTextureType type,const char* name)
{
    for (unsigned int i=0; i < mat->GetTextureCount(type); i++)
    {
        Model::TData data;

        aiString path;
        mat->GetTexture(type,i,&path);

        data.ImageData = std::make_shared<Graphics::Image>(Directory + "/" + path.C_Str());
        data.ImageData->generate();
        data.type = name;
        v->push_back(data);
    }
};
Model::DataHolder Model::processMesh(aiMesh* mesh)
{
    auto Data = DataHolder();
    // p[3] t[2] uv[2], totaling 6

    VertData = std::vector<Vector>(mesh->mNumVertices);
    
    auto Buffer = std::make_shared<Utils::OpenGL::Buffer>(ShaderVars);
    Data.Buffers.push_back(Buffer);
    Buffer->allocateData(mesh->mNumVertices);

    for (unsigned int i=0; i < mesh->mNumVertices; i++)
    {
        float vertex[8];
        if (mesh->HasPositions())
        {
            vertex[0] = mesh->mVertices[i].x;
            vertex[1] = mesh->mVertices[i].y;
            vertex[2] = mesh->mVertices[i].z;
        };
        if (mesh->HasNormals())
        {
            vertex[3] = mesh->mNormals[i].x;
            vertex[4] = mesh->mNormals[i].y;
            vertex[5] = mesh->mNormals[i].z;
        }
        if (mesh->HasTextureCoords(0))
        {
            float x = mesh->mTextureCoords[0][i].x;
            if (x > 1.0f)
            {
                x = x - 1.0f;
            }
            vertex[6] = x;

            float y = mesh->mTextureCoords[0][i].y;
            if (y > 1.0f)
            {
                y = y - 1.0f;
            };
            vertex[7] = y;
        }
        auto v = Vector({vertex[0],vertex[1],vertex[2],vertex[3],vertex[4],vertex[5],vertex[6],vertex[7]});
        VertData[i] = v;
        
        Buffer->sendData(vertex,1,i);

    };
    // indicies
    std::vector<GLuint> indicies;
    for (unsigned int i=0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j=0;j < face.mNumIndices; j++)
        {
            indicies.push_back(int(face.mIndices[j]));
        };
    };
    Buffer->setIndices(indicies);
    // material
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = AssScene->mMaterials[mesh->mMaterialIndex];
        processMaterial(material,&Data.Textures,aiTextureType_DIFFUSE,"diffuse");
        processMaterial(material,&Data.Textures,aiTextureType_SPECULAR,"specular");
    };

    return Data;
}
std::vector<Vertex> Model::getPoints()
{
    auto points = std::vector<Vertex>(VertData.size());
    for (unsigned int i=0; i < VertData.size(); i++)
    {
        auto v = VertData[i];
        points[i] = Vertex(v[0],v[1],v[2]);
    };
    return points;
}
void Model::generate()
{
    processNode(AssScene->mRootNode);
    AssImporter.FreeScene();
};
void Model::render(Scene::Container* c,DRAW mode)
{
    auto model = c->getModelMatrix();
    ShaderVars->sendMatrix("model_view",4,4,true,model->unpack());
    render(getMechanicalTarget(mode));
}
void Model::draw()
{
    render(getDrawMode());
};
using namespace Lucia::Utils::OpenGL;
void Model::render(int mech_ref)
{
    for (auto v: DataSegments)
    {
        int diffuse_no = 0;
        int specular_no = 0;
        // TEXTURES
        for (auto texture: v.Textures)
        {
            string name = "Texture_" + texture.type + "_";
            int no = 0;
            if (texture.type == "diffuse")
            {
                diffuse_no++;
                no = diffuse_no;
            }else
            {
                specular_no++;
                no = specular_no;
            }
            name = name + std::to_string(no);
            texture.ImageData->setName(name);
            texture.ImageData->bind(no);
        }
        // BUFFERS
        //Vars->use();
        if (v.Buffers.size() > 0)
        {
            v.Buffers[0]->attach();
            for (auto Buffer: v.Buffers)
            {
                 // needed for GLES2
              //glUniformMatrix4fv(glGetUniformLocation(Vars->getID(),"local"),1,GL_FALSE,glm::value_ptr(Model_Matrix));
                Buffer->attach();
                #ifndef LUCIA_USE_OPENGL3
                ShaderVars->apply();
                #endif
                Buffer->render(mech_ref);
            }
            v.Buffers[0]->detach();
        }
    }

};
Model::~Model()
{
    //dtor
}
