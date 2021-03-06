#ifndef LUCIA_UTILITIES_OPENGL_ANDBUFFER_H
#define LUCIA_UTILITIES_OPENGL_ANDBUFFER_H
#include <Lucia/Settings.h>
#include <Lucia/Utils/Log.h>
#include <vector>
#include <memory>
#include <fstream>
#include <iostream>
#include <cstdarg>
namespace Lucia {
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
            int find(std::string name);
            int findAttribute(std::string name);
            
            void send(std::string name,double v0){send(name,float(v0));};
            void send(std::string name,double v0,double v1){send(name,float(v0),float(v1));};
            void send(std::string name,double v0,double v1,double v2){send(name,float(v0),float(v1),float(v2));};
            void send(std::string name,double v0,double v1,double v2,double v3){send(name,float(v0),float(v1),float(v2),float(v3));};
            
            void send(std::string name,float v0);
            void send(std::string name,float v0,float v1);
            void send(std::string name,float v0,float v1,float v2);
            void send(std::string name,float v0,float v1,float v2,float v3);

            void send(std::string name,int v0);
            void send(std::string name,int v0,int v1);
            void send(std::string name,int v0,int v1,int v2);
            void send(std::string name,int v0,int v1,int v2,int v3);
            #ifndef LUCIA_USE_GLES2
            void send(std::string name,uint v0);
            void send(std::string name,uint v0,uint v1);
            void send(std::string name,uint v0,uint v1,uint v2);
            void send(std::string name,uint v0,uint v1,uint v2,uint v3);
            #endif
            void sendArray(std::string name,float* data,uint arraySize);
            void sendArray(std::string name,int* data,uint arraySize);
            void sendArray(std::string name,uint* data,uint arraySize);

            void sendMatrix(std::string name,int width,int height,bool transpose,float* value);

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
                #ifndef LUCIA_USE_GLES2
                GLuint ivbo=0; //instanced VBO
                #endif
                GLuint ebo=0;
                uint size;
                int boundInstanceSize = 0;
                std::vector<std::shared_ptr<Buffer>> Buffers;
                std::shared_ptr<Shader_Vars> Vars;
                bool Attached = false;

                void setShaderVars(std::shared_ptr<Shader_Vars> s);
                std::shared_ptr<Shader_Vars> getShaderVars();

                void generate(bool genEBO=false);
                void setData(std::vector<ptr<Vertex_Buffer>> Data);
                void setData(std::vector<Vertex_Buffer> Data);
                void setData(float *array,uint number_of_vertexes);

                void allocateData(uint number_of_vertexes);
                
                void applyData(uint VertexSize, uint size,GLfloat* array,uint datasize);
                void applyDynamicData(uint index,uint VertexSize, uint size,GLfloat* array,uint datasize);
                void sendData(std::vector<ptr<Vertex_Buffer>> Data,uint index=0);
                void sendData(std::vector<Vertex_Buffer> Data,uint index=0);
                void sendData(float *array,uint number_of_vertexes,uint index=0);
                
                #ifndef LUCIA_USE_GLES2
                void bindInstanced(uint componentNo,uint size,std::string where,uint nth);
                void setInstanced(std::vector<ptr<Buffer>> items,float* data,std::string where,uint componentNo,uint size,uint perNThInstance);
                void setInstanced(std::vector<ptr<Buffer>> items,std::vector<Vertex_Buffer> data,std::string where,int componentNo,uint perNThInstance);
                void addInstanced(float* data,std::string where,uint componentNo,uint size,uint perNthInstance);
                void addInstanced(std::vector<Vertex_Buffer> data,std::string where,uint componentNo,uint perNthInstance);
                void allocateInstanced(uint number_of_values,uint value_size);
                void addInstancedAssisted(int id,int componentNo,int size,int stride,int perNThInstance);
                #endif
                
                void setIndices(std::vector<GLint> indicies);
                void allocateIndices(uint size);
                void sendIndices(GLint* data, uint number,uint index=0);
                void setIndices(GLint* data, uint size);

                void setSize(uint s);

                void attach(bool attachVBO=false);
                void render(GLenum mode,uint start,uint lenght);
                void render(GLenum mode);
                
                #ifndef LUCIA_USE_GLES2
                void renderInstanced(GLenum mode);
                void renderInstanced(GLenum mode,uint totalitems);
                void renderInstanced(GLenum mode,uint totalitems,uint start,uint lenght);
                #endif
                void draw(GLenum mode);
                void draw(GLenum mode,Shader_Vars *Vars);
                void draw(GLenum mode,uint start,uint lenght,Shader_Vars *Vars);
                void detach(bool detachVBO=false);

                template <typename T>
                std::vector<ptr<Vertex_Buffer>> convertToData(const T* f,uint start,int finish=-1)
                {
                    if (finish == -1){start = 0; finish = start;};
                    uint VertexSize = getShaderVars()->getVertexSize();
                    std::vector<ptr<Vertex_Buffer>> output;
                    for (uint i = start; i < (uint)finish;i++)
                    {
                        auto buff = std::make_shared<Vertex_Buffer>();
                        for (uint j=0;j < VertexSize; j++)
                        {
                            auto x = VertexSize*i;
                            auto n = f[x+j];
                            buff->add(n);
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
                    return convertToData(data,0,siz/getShaderVars()->getVertexSize());
                };
                template <typename T>
                std::vector<int> converToIndicies(T f,uint start,uint finish)
                {
                    std::vector <int> Data;
                    for (uint i = start;i < finish;i++){Data.push_back((int)f[i]);};
                    return Data;
                };
                template <typename T>
                std::vector<int> converToIndicies(std::vector<T>s){return converToIndicies(s,0,s.size());};

                void destroy();
                //constructors
                Buffer(bool genEBO);
                Buffer(uint size,bool genEBO=false);
                Buffer(std::shared_ptr<Shader_Vars> s);
                Buffer(){};
                virtual ~Buffer(){destroy();};
        };
        void printShaderLog( GLuint shader );
        GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path);
        GLuint LoadShaderSource(const char * vertex_source,const char * fragment_source);
    }
}
}
#endif // UTILITIES_OPENGL_H
