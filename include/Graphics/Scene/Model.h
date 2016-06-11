#ifndef MIKUS_GRAPHICS_MODEL_H
#define MIKUS_GRAPHICS_MODEL_H

#include <Graphics/Scene/Object.h>
#include <Graphics/Image.h>
#include <Graphics/Shaders.h>

#include <utilities/OpenGL.h>
#include <utilities/log.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <Controll/Safety/Handler.h>

namespace Graphics
{
    typedef Utils::OpenGL::Buffer Buff;
    typedef Utils::OpenGL::Shader_Vars Shader_Vars;

    class Model: public Graphics::Scene::Object, public Controll::Safety::Handler
    {
        public:
            Model();
            void draw();
            void generate();
            void render(glm::mat4* model,DRAW mode);
            void fromFile(std::string path);
            virtual ~Model();
        private:
            Assimp::Importer AssImporter;
            bool fail = false;
            const aiScene* AssScene;
            std::string Directory = "";

            struct TData
            {
                std::shared_ptr<Graphics::Image> ImageData=nullptr;
                std::string type;
            };
            struct DataHolder
            {
                std::vector<std::shared_ptr<Utils::OpenGL::Buffer>> Buffers;
                std::vector<TData> Textures;
            };
            std::vector<DataHolder> DataSegments;

            void processNode(aiNode*);
            DataHolder processMesh(aiMesh*);
            void processMaterial(aiMaterial* mat,std::vector<TData> *v,aiTextureType type,const char* name);
            void MaterialToTexture();
            void render(int mech_ref);
    };
    namespace _Shaders
    {
        extern std::shared_ptr<Graphics::Shader> StandardModelShader;
    }

}


#endif // MIKUS_GRAPHICS_MODEL_H
