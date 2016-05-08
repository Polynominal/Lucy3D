#include "utilities/OpenGL.h"
typedef unsigned int uint;
typedef Utils::OpenGL::Buffer Buffer;
typedef Utils::OpenGL::Vertex_Buffer Buffer_Vertex;
typedef Utils::OpenGL::Shader_Vars Shader_Vars;
typedef Utils::OpenGL::Shader_Var_Atribute Attribute;

namespace Utils
{
    namespace OpenGL
    {
        std::shared_ptr<Shader_Vars> shaderVars = std::make_shared<Shader_Vars>();
        void setGlobalShaderVars(std::shared_ptr<Shader_Vars> v){shaderVars = v;};
        std::shared_ptr<Shader_Vars> getGlobalShaderVars(){return shaderVars;};

        void printShaderLog( GLuint shader )
        {
            //Make sure name is shader
            if( glIsShader( shader ) )
            {
                //Shader log length
                int infoLogLength = 0;
                int maxLength = infoLogLength;

                //Get info string length
                glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &maxLength );

                //Allocate string
                char* infoLog = new char[ maxLength ];

                //Get info log
                glGetShaderInfoLog( shader, maxLength, &infoLogLength, infoLog );
                if( infoLogLength > 0 )
                {
                    //Print Log
                    printf( "%s\n", infoLog );
                }

                //Deallocate string
                delete[] infoLog;
            }
            else
            {
                printf( "Name %d is not a shader\n", shader );
            }
        }
        GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path)
        {
            // Create the shaders
            GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
            GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
            // Read the Vertex Shader code from the file
            std::string VertexShaderCode;
            std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
            if(VertexShaderStream.is_open()){
                std::string Line = "";
                while(getline(VertexShaderStream, Line))
                    VertexShaderCode += "\n" + Line;
                VertexShaderStream.close();
            }else{
                printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
                getchar();
                return 0;
            }
            // Read the Fragment Shader code from the file
            std::string FragmentShaderCode;
            std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
            if(FragmentShaderStream.is_open()){
                std::string Line = "";
                while(getline(FragmentShaderStream, Line))
                    FragmentShaderCode += "\n" + Line;
                FragmentShaderStream.close();
            }
            GLint Result = GL_FALSE;
            int InfoLogLength;
            // Compile Vertex Shader
            printf("Compiling shader : %s\n", vertex_file_path);
            char const * VertexSourcePointer = VertexShaderCode.c_str();
            glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
            glCompileShader(VertexShaderID);
            // Check Vertex Shader
            glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
            glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
            if ( InfoLogLength > 0 ){
                std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
                glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
                printf("%s\n", &VertexShaderErrorMessage[0]);
            }
            if (Result != GL_TRUE){
                    printf("Shader compilation error: ");
                    printShaderLog(VertexShaderID);
            }
            // Compile Fragment Shader
            printf("Compiling shader : %s\n", fragment_file_path);
            char const * FragmentSourcePointer = FragmentShaderCode.c_str();
            glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
            glCompileShader(FragmentShaderID);
            // Check Fragment Shader
            glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
            glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
            if ( InfoLogLength > 0 ){
                std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
                glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
                printf("%s\n", &FragmentShaderErrorMessage[0]);
            }
            // Link the program
            printf("Linking program\n");
            GLuint ProgramID = glCreateProgram();
            glAttachShader(ProgramID, VertexShaderID);
            glAttachShader(ProgramID, FragmentShaderID);
            glLinkProgram(ProgramID);
            // Check the program
            glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
            glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
            if ( InfoLogLength > 0 ){
                std::vector<char> ProgramErrorMessage(InfoLogLength+1);
                glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
                printf("%s\n", &ProgramErrorMessage[0]);
            }
            glDetachShader(ProgramID, VertexShaderID);
            glDetachShader(ProgramID, FragmentShaderID);
            glDeleteShader(VertexShaderID);
            glDeleteShader(FragmentShaderID);
            return ProgramID;
        }
        GLuint LoadShaderSource(const char * vertex_source,const char * fragment_source)
        {
            GLint Result = GL_FALSE;
            int InfoLogLength;

            GLuint VertexShaderID= glCreateShader(GL_VERTEX_SHADER);
            GLuint FragmentShaderID= glCreateShader(GL_FRAGMENT_SHADER);
            // Compile Vertex Shader
            printf("Compiling vertex shader\n");
            glShaderSource(VertexShaderID, 1, &vertex_source , NULL);
            glCompileShader(VertexShaderID);


            // Check Vertex Shader
            glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
            glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
            if ( InfoLogLength > 0 ){
                std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
                glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
                printf("%s\n", &VertexShaderErrorMessage[0]);
            }
            if (Result != GL_TRUE){
                    printf("Shader compilation error: \n");
                    printShaderLog(VertexShaderID);
            }
            // Compile Fragment Shader
            printf("Compiling fragment shader!\n");
            glShaderSource(FragmentShaderID, 1, &fragment_source , NULL);
            glCompileShader(FragmentShaderID);

            // Check Fragment Shader
            glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
            glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
            if ( InfoLogLength > 0 ){
                std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
                glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
                printf("%s\n", &FragmentShaderErrorMessage[0]);
            }
            // Link the program
            printf("Linking program\n");
            GLuint ProgramID = glCreateProgram();
            glAttachShader(ProgramID, VertexShaderID);
            glAttachShader(ProgramID, FragmentShaderID);
            glLinkProgram(ProgramID);

            // Check the program
            glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
            glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
            if ( InfoLogLength > 0 ){
                std::vector<char> ProgramErrorMessage(InfoLogLength+1);
                glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
                printf("%s\n", &ProgramErrorMessage[0]);
            }


            glDetachShader(ProgramID, VertexShaderID);
            glDetachShader(ProgramID, FragmentShaderID);

            glDeleteShader(VertexShaderID);
            glDeleteShader(FragmentShaderID);

            return ProgramID;
        }
    }
}


void Attribute::apply(int vsize)
{
    glEnableVertexAttribArray(location);
    glVertexAttribPointer(location, finish - start, GL_FLOAT,GL_FALSE, vsize * sizeof(GLfloat), (GLvoid*)(start * sizeof(GLfloat)));
};

void Shader_Vars::add(const char* name, int start,int finish)
{
    int id = glGetAttribLocation(programID,name);
    if (id == -1)
    {
        std::cout << "VAR NOT FOUND: " << name << std::endl;
    }
    Attributes.push_back(Shader_Var_Atribute(id,start,finish));
    VertexSize = VertexSize + (finish - start);
};
int Shader_Vars::getVertexSize()
{
    return VertexSize;
}
void Shader_Vars::apply()
{
    for (auto v: Attributes)
    {
        v.apply(VertexSize);
    }
}

/*
*       +-----------+
*       |***********|
*       |*  BUFFER *|
*       |***********|
*       +-----------+
*
*/
void Buffer::generate(bool genEBO)
{
    #ifdef USE_OPENGL3
    glGenVertexArrays(1,&vao);
    #endif // USE_GLES2
    glGenBuffers(1,&vbo);
    if (genEBO)
    {
        glGenBuffers(1,&ebo);
    }
}
void Buffer::setShaderVars(std::shared_ptr<Shader_Vars> s){
    Vars = s;
    if (s.get() == nullptr)
    {
        std::cout << "WARNING ASSIGING UNINITALIZED POINTER TO VARS!!" << std::endl;
    }
};
std::shared_ptr<Shader_Vars> Buffer::getShaderVars()
{
    return Vars;
};
// notice that array ONE will be the one with the final count
void addArrays(GLfloat* one,uint size,std::vector<GLfloat> two,uint size2)
{
    for (uint i = size;i < size + size2; i++)
    {
        one[i] = two[i-size];
    }
}
void addArrays(GLfloat* one,uint size,GLfloat* two,uint size2)
{
    for (uint i = size;i < size + size2; i++)
    {
        one[i] = two[i-size];
    }
}
void getDataFromPtrs(std::vector<std::shared_ptr<Utils::OpenGL::Vertex_Buffer>> Data,uint VertexSize,GLfloat* compiled)
{
    uint no = 0;
    for (auto v: Data)
    {
        addArrays(compiled,no*VertexSize,v->Data,VertexSize);
        no++;
    }
}
void getDataFromPtrs(GLfloat *Data,uint size,uint VertexSize,GLfloat* compiled)
{
    addArrays(compiled,0,Data,VertexSize*size);
}
void Buffer::setData(std::vector<ptr<Vertex_Buffer>> Data)
{
    uint VertexSize = getShaderVars()->getVertexSize();
    uint size = Data.size();
    GLfloat compiled[VertexSize*size];
    getDataFromPtrs(Data,VertexSize,compiled);

    attach(true);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * VertexSize * size,&compiled[0],GL_STATIC_DRAW);
    //set attribs
    getShaderVars()->apply();
    setSize(Data.size());
}
void Buffer::setData(GLfloat *Data,uint size)
{
    uint VertexSize = getShaderVars()->getVertexSize();
    GLfloat compiled[VertexSize*size];
    getDataFromPtrs(Data,size,VertexSize,compiled);

    attach(true);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * VertexSize * size,&compiled[0],GL_STATIC_DRAW);
    //set attribs
    getShaderVars()->apply();
    setSize(size);
}
void Buffer::allocateData(uint size)
{
    uint VertexSize = getShaderVars()->getVertexSize();
    attach(true);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) *(size * VertexSize),NULL,GL_DYNAMIC_DRAW);
    getShaderVars()->apply();

    setSize(size);
}
void Buffer::sendData(std::vector<ptr<Vertex_Buffer>> Data,uint index)
{
    attach(true);
    uint VertexSize = getShaderVars()->getVertexSize();
    GLfloat compiled[VertexSize*Data.size()];
    getDataFromPtrs(Data,VertexSize,compiled);

    uint offset = index*VertexSize*sizeof(GLfloat);
    glBufferSubData(GL_ARRAY_BUFFER,offset,sizeof(GLfloat) *VertexSize*Data.size(),&compiled[0]);
    getShaderVars()->apply();
}
void Buffer::sendData(GLfloat *Data,uint size,uint index)
{
    attach(true);
    uint VertexSize = getShaderVars()->getVertexSize();
    GLfloat compiled[VertexSize*size];
    getDataFromPtrs(Data,size,VertexSize,compiled);

    uint offset = index*VertexSize*sizeof(GLfloat);
    glBufferSubData(GL_ARRAY_BUFFER,offset,sizeof(GLfloat) *VertexSize*size,&compiled[0]);
    getShaderVars()->apply();
}
void Buffer::allocateIndices(uint size)
{
    attach(true);
    if(ebo==0){glGenBuffers(1,&ebo);};

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLint) *size,NULL,GL_DYNAMIC_DRAW);

    setSize(size);
}
void Buffer::sendIndices(GLint *Data,uint size,uint index)
{
    attach(true);
    if(ebo==0){glGenBuffers(1,&ebo);};

    uint offset = index*sizeof(GLint);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,offset,sizeof(GLint),&Data[0]);
}
void Buffer::setIndices(GLint *Data,uint size)
{
    attach();

    if(ebo==0){glGenBuffers(1,&ebo);};
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(GLint),&Data[0], GL_STATIC_DRAW);

    setSize(size);
}
void Buffer::setIndices(std::vector<GLuint> indicies)
{
    attach();

    if(ebo==0){glGenBuffers(1,&ebo);};
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicies.size() * sizeof(GLint),&indicies.front(), GL_STATIC_DRAW);

    setSize(indicies.size());
}
void Buffer::setSize(uint s)
{
    size = s;
}
void Buffer::attach(bool t)
{
        #ifndef USE_GLES2
        glBindVertexArray(vao);
        #else
        glBindBuffer(GL_ARRAY_BUFFER,vbo);
        #endif // USE_GLES2
        if (ebo != 0 ){glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo);};
        if (t){glBindBuffer(GL_ARRAY_BUFFER,vbo);};
};
void Buffer::render(GLenum mode,uint start,uint lenght)
{
    if (ebo != 0)
    {
        glDrawElements(mode,lenght,GL_UNSIGNED_INT,0);
    }else
    {
        glDrawArrays(mode,start,lenght);
    };
}
void Buffer::render(GLenum mode)
{
    render(mode,0,size);
}
void Buffer::draw(GLenum mode,uint start,uint lenght)
{
    Attached = false;
    attach();
    #ifdef USE_GLES2
    Vars->apply();
    #endif // USE_GLES2
    render(mode,start,lenght);
    detach();
}
void Buffer::draw(GLenum mode){draw(mode,0,size);};
void Buffer::detach(bool t)
{
        #ifdef USE_OPENGL3
        glBindVertexArray(0);
        #else
        if (ebo != 0 ){glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo);};
        glBindBuffer(GL_ARRAY_BUFFER,0);
        #endif // USE_GLES2
};
void Buffer::destroy()
{
    #ifdef USE_OPENGL3
    glDeleteVertexArrays(1,&vao);
    #endif
    glDeleteBuffers(1,&vbo);
    glDeleteBuffers(1,&ebo);
};

//constructors
Buffer::Buffer(bool genEBO){generate(genEBO);};
Buffer::Buffer(uint size,bool genEBO)
{
    generate(genEBO);
    allocateData(size);
}
Buffer::Buffer(std::shared_ptr<Shader_Vars> s)
{
    setShaderVars(s);
    generate(false);
}
