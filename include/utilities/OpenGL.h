#ifndef UTILITIES_OPENGL_H
#define UTILITIES_OPENGL_H
#include <Settings.h>
#include <vector>
#include <memory>
#include <fstream>
#include <iostream>
#include <cstdarg>

namespace Utils
{
    namespace OpenGL
    {
        typedef unsigned int uint;
        template<typename T>
        using ptr = typename std::shared_ptr<T>;
        struct Vertex_Buffer
        {
            std::vector<GLfloat> Data;
            // the order is messed up with the constructor so I have to write two varidiac templates.
            // I originally had a diagnostic varidiac function but it would cause sigill via va_arg as expected

            template <typename T>
            void add(T var)
            {
                Data.push_back(var);
            };
            template <class T, class... T2>
            void add(T first,T2... rest)
            {
                add(first);
                add(rest...);
            }

            template <typename T>
            Vertex_Buffer(T var)
            {
                add(var);
            }
            template <class T, class... T2>
            Vertex_Buffer(T first, T2... rest)
            {
                add(first);
                add(rest...);
            };
            Vertex_Buffer(){};

        };
        struct Shader_Var_Atribute
        {
            int location;
            int start;
            int finish;

            Shader_Var_Atribute(int tlocation,int tstart,int tfinish):
            location(tlocation),start(tstart),finish(tfinish){};
            void apply(int vsize);
        };
        struct Shader_Vars
        {
            std::vector <Shader_Var_Atribute> Attributes;
            int VertexSize = 0;
            int programID;
            uint VBufferIter=0;

            Shader_Vars(){};
            void use(){glUseProgram(programID);};
            void clear(){Attributes.clear();VertexSize=0;};
            void setProgram(int ID){programID = ID;clear();};
            int getID(){return programID;};
            void add(const char* name, int start,int finish);
            int getVertexSize();
            void apply();
        };
        extern ptr<Shader_Vars>shaderVars;
        void setGlobalShaderVars(std::shared_ptr<Shader_Vars> v);
        std::shared_ptr<Shader_Vars> getGlobalShaderVars();

        class Buffer
        {
            public:
                GLuint vao=0;
                GLuint vbo=0;
                GLuint ebo=0;
                uint size;
                std::shared_ptr<Shader_Vars> Vars;
                bool Attached = false;

                void setShaderVars(std::shared_ptr<Shader_Vars> s);
                std::shared_ptr<Shader_Vars> getShaderVars();

                void generate(bool genEBO=false);
                void setData(std::vector<ptr<Vertex_Buffer>> Data);
                void setData(GLfloat *array,uint number_of_vertexes);

                void allocateData(uint number_of_vertexes);
                void sendData(std::vector<ptr<Vertex_Buffer>> Data,uint index=0);
                void sendData(GLfloat *array,uint number_of_vertexes,uint index=0);

                void setIndices(std::vector<GLuint> indicies);
                void allocateIndices(uint size);
                void sendIndices(GLint* data, uint number,uint index=0);
                void setIndices(GLint* data, uint size);

                void setSize(uint s);

                void attach(bool attachVBO=false);
                void render(GLenum mode,uint start,uint lenght);
                void render(GLenum mode);
                void draw(GLenum mode);
                void draw(GLenum mode,uint start,uint lenght);
                void detach(bool detachVBO=false);

                template <typename T>
                std::vector<ptr<Vertex_Buffer>> convertToData(T* f,uint start,uint finish=-1)
                {
                    if (finish == -1){start = 0; finish = start;};
                    uint VertexSize = getShaderVars()->getVertexSize();
                    std::vector<ptr<Vertex_Buffer>> output;
                    for (uint i = start; i < finish;i++)
                    {
                        auto buff = std::make_shared<Vertex_Buffer>();
                        for (uint j=0;j < VertexSize; j++)
                        {
                            auto x = VertexSize*i;
                            buff->add(f[x+j]);
                        };
                        output.push_back(buff);
                    }
                    return output;
                };
                template <typename T>
                void convertToArray(T* f,GLfloat* data)
                {
                    int no = 0;
                    for (auto v: *f)
                    {
                        data[no]=v;
                        no++;
                    };
                };
                template <typename T>
                std::vector<ptr<Vertex_Buffer>> convertToData(T* s){
                    int siz = s->size();
                    GLfloat data[siz];
                    convertToArray(s,data);
                    return convertToData(data,0,siz);
                };
                template <typename T>
                std::vector<GLfloat> converToIndicies(T f,uint start,uint finish)
                {
                    std::vector <GLfloat> Data;
                    for (uint i = start;i < finish;i++){Data.push_back((GLfloat)f[i]);};
                    return Data;
                };
                template <typename T>
                std::vector<GLfloat> converToIndicies(std::vector<T>s){return converToIndicies(s,0,s.size());};

                void destroy();
                //constructors
                Buffer(bool genEBO);
                Buffer(uint size,bool genEBO=false);
                Buffer(std::shared_ptr<Shader_Vars> s);
                Buffer(){};
        };
        void printShaderLog( GLuint shader );
        GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path);
        GLuint LoadShaderSource(const char * vertex_source,const char * fragment_source);
    }
}
#endif // UTILITIES_OPENGL_H
