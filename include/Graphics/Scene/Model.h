#ifndef MIKUS_GRAPHICS_MODEL_H
#define MIKUS_GRAPHICS_MODEL_H

#include <Graphics/Bases/Moveable.h>
#include <Graphics/Bases/DrawMode.h>
#include <Graphics/Bases/ShaderHolder.h>

#include <Graphics/Image.h>
#include <Graphics/Shaders.h>

#include <utilities/OpenGL.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


namespace Graphics
{
    typedef Utils::OpenGL::Buffer Buff;
    typedef Utils::OpenGL::Shader_Vars Shader_Vars;

    class Model: public Graphics::Base::Moveable,public Graphics::Base::DrawMode,
    public Graphics::Base::ShaderHolder
    {
        public:
            Model();
            void draw();
            void generate();
            void fromFile(std::string path);
            virtual ~Model();
        private:
            Assimp::Importer AssImporter;
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
