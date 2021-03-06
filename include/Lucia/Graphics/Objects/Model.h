#ifndef MIKUS_LUCIA_GRAPHICS_MODEL_H
#define MIKUS_LUCIA_GRAPHICS_MODEL_H

#include <Lucia/Graphics/Scene/Object.h>
#include <Lucia/Graphics/Image.h>
#include <Lucia/Graphics/Shaders.h>

#include <Lucia/Utils/OpenGL.h>
#include <Lucia/Utils/log.h>

#include <Lucia/Controll/Safety/Handler.h>
#include <Lucia/Maths/Matrix.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


namespace Lucia{
namespace Graphics{
namespace Object{
    
    typedef Utils::OpenGL::Buffer Buff;
    using namespace Utils::OpenGL;

    class Model: public Graphics::Scene::Object, public Controll::Safety::Handler
    {
        public:
            Model();
            void draw();
            void generate();
            void render(Graphics::Scene::Container* c,DRAW mode);
            void fromFile(std::string path);
            std::vector<Vertex> getPoints();
            
            virtual ~Model();
        private:
            std::vector<Vector> VertData;
            
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
}}}

#endif // MIKUS_GRAPHICS_MODEL_H
