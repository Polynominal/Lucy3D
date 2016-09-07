#include "Lucia\Graphics\Shaders.h"
using namespace Lucia;
using Utils::OpenGL::Shader_Vars;
void logError( GLuint shader )
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
            LOG << "Fatal" << infoLog << std::endl;
        }

        //Deallocate string
        delete[] infoLog;
    }
    else
    {
        LOG << "Fatal" << "Name %d is not a shader" << std::endl;
    }
}
GLuint LoadShaderSource(const char * vertex_source,const char * fragment_source)
{
    bool fail = false;
    GLint Result = GL_FALSE;
    int InfoLogLength;

    GLuint VertexShaderID= glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID= glCreateShader(GL_FRAGMENT_SHADER);
    // Compile Vertex Shader
    LOG << "Compiling vertex shader!" << std::endl;
    glShaderSource(VertexShaderID, 1, &vertex_source , NULL);
    glCompileShader(VertexShaderID);


    // Check Vertex Shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
        printf("%s\n", &VertexShaderErrorMessage[0]);
        fail = true;
    }
    if (Result != GL_TRUE){
        LOG << "Warning" << "[FROM SOURCE] GLSL compilation error:" << std::endl;
        logError(VertexShaderID);
        fail = true;
    }
    // Compile Fragment Shader
    LOG << "Building" <<"Compiling fragment shader!" << std::endl;
    glShaderSource(FragmentShaderID, 1, &fragment_source , NULL);
    glCompileShader(FragmentShaderID);

    // Check Fragment Shader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
        printf("%s\n", &FragmentShaderErrorMessage[0]);
        fail = true;
    }
    // Link the program
    LOG << "Linking program" << std::endl;
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
        LOG << "Fatal" << &ProgramErrorMessage[0] << std::endl;
        fail = true;
    }

    glDetachShader(ProgramID, VertexShaderID);
    glDetachShader(ProgramID, FragmentShaderID);

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);
    
    if (fail){return 0;};
    return ProgramID;
}
void checkBasicBuild(Shader_Vars* v)
{

    if (!Graphics::_Shaders::Basic2D->isBuilt())
    {
        std::string Vertex =
        "#version 100 \n"
        "attribute vec2 vertex;\n"
        "attribute vec2 uv;\n"
        "varying vec2 TexCoords;\n"
        "uniform vec2 translation;"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4((vertex + translation).xy, 0.0f, 1.0f);\n"
        "   TexCoords = uv;\n"
        "}; \n";


        std::string Fragment =
        "#version 100 \n"
        "precision highp float;"
        "varying vec2 TexCoords;\n"
        "uniform vec4 Color;\n"
        "uniform bool useColor;\n"
        "uniform sampler2D Texture;\n"
        "void main()"
        "{"
        "   if(useColor){gl_FragColor=Color;}else{gl_FragColor=texture(Texture,TexCoords);};"
        "};";


        Graphics::_Shaders::Basic2D->build(Vertex,Fragment,"Basic2D");
        Graphics::_Shaders::Basic2D->addVar("vertex",0,2);
        Graphics::_Shaders::Basic2D->addVar("uv",2,4);
    }
}
namespace Lucia {
namespace Graphics
{
    namespace _Shaders
    {
        std::unique_ptr<Graphics::Shader> Basic2D = std::unique_ptr<Graphics::Shader>(new Graphics::Shader);
    }
    Shader::Shader()
    {
        //ctor
    }
    void Shader::build(string Vertex,string Fragment,string name)
    {
        LOG << "Building" << "[----------Shader-------------]"  << std::endl;
        LOG << name << std::endl;
        LOG << "=================================="  << std::endl;
        built = true;
        if (SDL_WasInit(SDL_INIT_EVERYTHING) == 0) {SDL_Init(SDL_INIT_EVERYTHING);};
        programID = LoadShaderSource(Vertex.c_str(),Fragment.c_str());
        if (Vars.get() == nullptr){Vars = std::make_shared<Utils::OpenGL::Shader_Vars>();};
        Vars->setProgram(programID);
        checkBasicBuild(Vars.get());
        LOG << "[--------------------------------]" << std::endl;
    };

    void Shader::fromFile(string Vertex_path,string Fragment_path,string name)
    {
        const char* vertex_file_path   =Vertex_path.c_str();
        const char* fragment_file_path =Fragment_path.c_str();
        if (made)
        {
            glDeleteProgram(programID);
        }
        made = true;
        std::string VertexShaderCode;
        std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
        if(VertexShaderStream.is_open()){
            std::string Line = "";
            while(getline(VertexShaderStream, Line))
                VertexShaderCode += "\n" + Line;
            VertexShaderStream.close();
        }else{
            printf("Impossible to open %s.\n", vertex_file_path);
            getchar();
            return;
        }
        LOG << "Building" << "LOADING SHADER FROM SOURCE: " << "\n" << Vertex_path << "\n" << Fragment_path << std::endl;
        // Read the Fragment Shader code from the file
        std::string FragmentShaderCode;
        std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
        if(FragmentShaderStream.is_open()){
            std::string Line = "";
            while(getline(FragmentShaderStream, Line))
                FragmentShaderCode += "\n" + Line;
            FragmentShaderStream.close();
        }

        build(VertexShaderCode,FragmentShaderCode,name);
    };

    void Shader::use()
    {
        glUseProgram(programID);
    }
    void Shader::attach()
    {
        glGetIntegerv(GL_CURRENT_PROGRAM,&lastShader);
        glUseProgram(programID);
    }
    void Shader::detach()
    {

        glUseProgram(lastShader);
    }
    Shader::~Shader()
    {
        glDeleteProgram(programID);
        //dtor
    }
}
}
