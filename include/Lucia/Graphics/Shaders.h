#ifndef MIKUS_LUCIA_GRAPHICS_SHADERS_H
#define MIKUS_LUCIA_GRAPHICS_SHADERS_H
#include <Lucia/Settings.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <SDL.h>
#include <memory>
#include <Lucia/Graphics/Graphics.h>
#include <Lucia/Utils/Utils.h>
#include <Lucia/Utils/OpenGL.h>
#include <Lucia/Utils/log.h>
namespace Lucia {
namespace Graphics
{
    using std::string;
    class Shader
    {
        public:
            Shader();
            void fromFile(const char* Vertex_path,const char* Fragment_path,const char* name)
            {fromFile(std::string(Vertex_path),std::string(Fragment_path),std::string(name));};
            void fromFile(string Vertex_path,string Fragment_path,string name);
            void build(string Vertex,string Fragment,string name);
            void addVar(const char* name, int start,int finish){Vars->add(name,start,finish);};
            std::shared_ptr<Utils::OpenGL::Shader_Vars> getVars(){return Vars;};
            void use();
            void attach();
            void detach();
            bool isBuilt(){return built;};
            virtual ~Shader();

            bool made=false;
            GLuint programID;
        private:
            std::shared_ptr<Utils::OpenGL::Shader_Vars> Vars;
            bool built = false;
            GLint lastShader;
    };
    namespace _Shaders
    {
        extern std::unique_ptr<Graphics::Shader> Basic2D;
    }
};}


#endif // MIKUS_GRAPHICS_SHADERS_H
