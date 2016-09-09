#include <Lucia/Collider/Opengl.h>
using namespace Lucia;
using namespace Collider;
using namespace Utils;
using namespace OpenGL;
namespace Lucia {
namespace Collider_OpenGL
{
    typedef Utils::OpenGL::Buffer Buffer;
    typedef Utils::OpenGL::Shader_Vars Vars;
    bool clean = true;

    int draw_mode = GL_STATIC_DRAW;
    // vector here is {VAO,VBO,EBO};
    std::map<Shape::Type,Buffer*> BufferIDs;
    std::map<unsigned int,Buffer*> polygonBufferIDs;
    std::shared_ptr<Vars> SVars;
    std::shared_ptr<Vars> PVars;
    GLuint programID=0;
    GLuint polygonID=0;


    Buffer* makeSquare()
    {
        if (!clean)
        {
            return BufferIDs[Shape::Type::box];
        }
        // 7-------------4
        // |##           ##
        // |   ##          ##
        // |     ##          ##
        // 6        3-----------0
        //  ##      |     5#    |
        //    ##    |       ##  |
        //      ##  |         ##|
        //        ##2-----------1

        float data[24] =
        {
            0.5f,  0.5f, 0.5f,
            0.5f, -0.5f, 0.5f,
            -0.5f, -0.5f, 0.5f,
            -0.5f,  0.5f, 0.5f,

            0.5f,  0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f,  0.5f, -0.5f
        };
        std::vector<int> indicies{

            //front
            1,2,3,0,
            1,5,4,0,
            1,5,6,2,
            2,6,7,3,
            7,4,0,3,7

        };
        Buffer* Buff = new Buffer(SVars);
        Buff->setData(Buff->convertToData(data,0,8));
        Buff->setIndices(indicies);
        BufferIDs.insert({Shape::Type::box,Buff});
        return Buff;
    };
    int makePolygon(Collider::Manager *M, std::vector<Collider::Vertex> Points)
    {
        struct Triangle
        {
            Triangle(){};
            void add(Vertex v,int index)
            {
                Core.push_back(v);
                Index.push_back(index);
            };
            void add(Triangle v){Adjacent.push_back(v);};
            std::vector<Triangle> Adjacent;
            std::vector<Vertex> Core;
            std::vector<int> Index;
            Vertex* operator [](unsigned int i)
            {
                return &Core[i];
            }
        };
        
        std::vector<float> data;
        std::vector<int> indicies;
        
        std::vector<Triangle> Triangles;
        
        for (unsigned int i = 0; i < Points.size(); i=i+3)
        {
            auto triangle = Triangle();
            triangle.add(Points[i],i);
            triangle.add(Points[i+1],i+1);
            triangle.add(Points[i+2],i+2);
            
            Triangles.push_back(triangle);
        };
        for (auto v: Points)
        {
            data.push_back(v.x);
            data.push_back(v.y);
            data.push_back(v.z);
        }
        for (unsigned int i = 0; i < Triangles.size(); i++)
        {
            auto me = Triangles[i];
            for (unsigned int j = 0; j < Triangles.size(); j++)
            {
                if (i != j)
                {
                    auto other = Triangles[j];
                    unsigned int common = 0;
                    for (auto v: other.Core)
                    {
                        for (auto n: me.Core)
                        {
                            if (v == n){common++;};
                            if (common == 2)
                            {
                                Triangles[i].add(other);
                                break;
                            }; 
                        }
                    }
                }
            }
        }
        
        for (auto v: Triangles)
        {
            indicies.push_back(v.Index[0]);
            indicies.push_back(v.Index[1]);
        };
        auto v = Triangles[Triangles.size() - 1];
        auto target = v.Core[2];
        for (auto w: v.Adjacent)
        {
            //auto 
            for (auto n: w.Core)
            {
                if (target == n)
                {
                    indicies.push_back(w.Index[1]);
                    indicies.push_back(w.Index[2]);
                }
            }
        }
        
         

        Buffer* Buff = new Buffer(SVars);
        Buff->setData(Buff->convertToData(&data));
        Buff->setIndices(indicies);
        
        auto id = polygonBufferIDs.size();
        polygonBufferIDs.insert({id,Buff});
        return id;
    };
    void removePolygon(int id)
    {
        if (id == -1){return;};
        polygonBufferIDs.erase(id);
    };
    Buffer* makeSphere(int subdiv_x,int subdiv_y)
    {

        if (!clean)
        {
            return BufferIDs[Shape::Type::sphere];
        };

        std::vector<float> data;
        std::vector<int> indicies;
        for (int i=0;i <= subdiv_y; i++)
        {
            float v = i/(float)subdiv_y;
            float pi = v*3.141592653589793238462643383279502884197;
            for (int j=0;j <= subdiv_x;j++)
            {
                float u = j/(float)subdiv_x;
                float theta = u * (3.14159265358979323846264338*2);
                Collider::Vertex v = Collider::Vertex(
                    cosf(theta)*sinf(pi),
                    cosf(pi),
                    sinf(theta)*sinf(pi)
                );
                v =v/2;
                data.push_back(v.x);
                data.push_back(v.y);
                data.push_back(v.z);
            }
        };
        for (int i=0; i < (subdiv_x*subdiv_y)+ subdiv_x;i++)
        {
            indicies.push_back(i);
            indicies.push_back(i + subdiv_x +1);
            indicies.push_back(i + subdiv_x);

            indicies.push_back(i + subdiv_x+1);
            indicies.push_back(i);
            indicies.push_back(i+1);
        }

        Buffer* Buff = new Buffer(SVars);
        Buff->setData(Buff->convertToData(&data));
        Buff->setIndices(indicies);
        BufferIDs.insert({Shape::Type::sphere,Buff});
        return Buff;
    };
    Buffer* makeRay()
    {
        if (!clean)
        {
            return BufferIDs[Shape::Type::ray];
        };
        Buffer* Buff = new Buffer(false);
        Buff->setShaderVars(SVars);
        #ifndef LUCIA_USE_GLES2
        glBindVertexArray(Buff->vao);
        #endif
        GLfloat data[6] = {0,0,0,0,0,0};

        glBindBuffer(GL_ARRAY_BUFFER,Buff->vbo);

        glBufferData(GL_ARRAY_BUFFER,6 * sizeof(GLfloat),NULL,GL_STREAM_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER,0,6 * sizeof(GLfloat),&data[0]);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3 * sizeof(GLfloat),(GLvoid*)0);

        BufferIDs.insert({Shape::Type::ray,Buff});
        #ifndef LUCIA_USE_GLES2
        glBindVertexArray(0);
        #else
        glBindBuffer(GL_ARRAY_BUFFER,0);
        #endif
        Buff->setSize(2);
        return Buff;
    };
    Buffer* makePoint()
    {
        if (!clean)
        {
            return BufferIDs[Shape::Type::point];
        };
        float data[3] =
        {
            0.5f,0.5f,0.5f
        };
        Buffer* Buff = new Buffer(false);
        Buff->setShaderVars(SVars);
        Buff->setData(Buff->convertToData(data,1));

        BufferIDs.insert({Shape::Type::point,Buff});
        return Buff;
    };
    void drawRay(Buffer* Buff,Collider::Vertex A,Collider::Vertex B)
    {

        GLfloat data[6] =
        {A.x,A.y,A.z,
        B.x,B.y,B.z};
        glBindBuffer(GL_ARRAY_BUFFER,Buff->vbo);
        glBufferSubData(GL_ARRAY_BUFFER,0,6*sizeof(GLfloat),&data[0]);
        #ifndef LUCIA_USE_GLES2
        glBindBuffer(GL_ARRAY_BUFFER,0);
        glBindVertexArray(Buff->vao);
            glDrawArrays(GL_LINES,0,6);
        glBindVertexArray(0);
        #else
            glDrawArrays(GL_LINES,0,6);
        glBindBuffer(GL_ARRAY_BUFFER,0);
        #endif

    };
    void createWrappers(Collider::Manager *M,Maths::Matrix<4> *view,Maths::Matrix<4> *projection,bool setLine)
    {
            if (clean)
            {
                std::string vertex =
                #if !(defined LUCIA_USE_GLES3 || defined LUCIA_USE_GLES2)
                "#version 330 core \n"
                "layout (location = 0) in vec3 position; \n"
                "uniform vec3 color; \n"
                "out vec3 Color; \n"
                #else
                "#version 100 \n"
                "attribute vec3 position; \n"
                "uniform vec3 color; \n"
                "varying vec3 Color; \n"
                #endif
                "uniform mat4 view; \n"
                "uniform mat4 projection; \n"
                "uniform mat4 model;\n"
                "void main() \n"
                "{\n"
                "gl_Position = projection * view * model *vec4(position, 1.0f); \n"
                "Color = color; \n"
                "};";

                std::string fragment =
                #if !(defined LUCIA_USE_GLES3 || defined LUCIA_USE_GLES2)
                "#version 330 core\n"
                "in vec3 Color;\n"
                "out vec4 color;\n"
                #else
                "#version 100 \n"
                "precision highp float;"
                "varying vec3 Color;"
                "vec4 color;"
                #endif
                "void main()\n"
                "{\n"
                    "color = vec4(Color,1.0f); \n"
                #if defined LUCIA_USE_GLES3 || defined LUCIA_USE_GLES2
                    "gl_FragColor = color;"
                #endif
                "};";
                LOG << "Building" << "Collider Wrapper Shader" << std::endl;

                programID = LoadShaderSource(vertex.c_str(),fragment.c_str());
                SVars = std::make_shared<Vars>();
                SVars->setProgram(programID);
                SVars->add("position",0,3);
            };



            M->CreatePolygon = [M](std::vector<Collider::Vertex> Points){
                return makePolygon(M,Points);
            };
            M->DeletePolygon = [](int id)
            {
                removePolygon(id);
            };
            // pre draw functions use them to encompass the other draw calls.

            M->PreDraw = [setLine,programID,view,projection](){
                if (setLine)
                {
                    glEnable(GL_DEPTH_TEST);
                }

                glUseProgram(programID);

                GLuint var=0;
                var = glGetUniformLocation(programID,"view");
                glUniformMatrix4fv(var,1,GL_TRUE,view->unpack());
                var = glGetUniformLocation(programID,"projection");
                glUniformMatrix4fv(var,1,GL_FALSE,projection->unpack());
                
                glDisable(GL_POLYGON_OFFSET_FILL);
            };
            M->PostDraw = [](){
                glEnable(GL_POLYGON_OFFSET_FILL);
                glPolygonOffset(0.49f,0.49f);
                
                glUseProgram(0);
            };
            M->DrawPolygon = [M,view,projection](Matrix<4> Data,int id,Vertex Color){

                auto d = polygonBufferIDs[id];
                
                GLuint var = glGetUniformLocation(programID,"color");
                glUniform3f(var,Color.x,Color.y,Color.z);
                
                // position data
                var = glGetUniformLocation(programID,"model");
                glUniformMatrix4fv(var,1,GL_TRUE,Data.unpack());
                
                d->draw(GL_LINES);
                    
            };
            auto Buff = makePoint();
            M->DrawPoint = [Buff,programID](Matrix<4> Data,Vertex Color){

                GLint var = glGetUniformLocation(programID,"color");
                glUniform3f(var,Color.x,Color.y,Color.z);
                // position data
                var = glGetUniformLocation(programID,"model");
                glUniformMatrix4fv(var,1,GL_TRUE,Data.unpack());

                Buff->draw(GL_LINE_STRIP);
            };
            Buff = makeSphere(10,10);
            M->DrawSphere = [Buff,programID](Matrix<4> Data,Vertex Color){
                GLint var = glGetUniformLocation(programID,"color");
                glUniform3f(var,Color.x,Color.y,Color.z);
                // position data
                var = glGetUniformLocation(programID,"model");
                glUniformMatrix4fv(var,1,GL_TRUE,Data.unpack());
                Buff->draw(GL_LINE_STRIP);
            };

            Buff = makeSquare();
            M->DrawBox = [Buff,programID](Matrix<4> Data,Vertex Color){
                //color data
                GLint var = glGetUniformLocation(programID,"color");
                glUniform3f(var,Color.x,Color.y,Color.z);
                // position data
                var = glGetUniformLocation(programID,"model");
                glUniformMatrix4fv(var,1,GL_TRUE,Data.unpack());

                Buff->draw(GL_LINE_STRIP);
            };
            auto p2 = makeRay();
            M->DrawRay = [p2,programID](Vertex A,Vertex B,Vertex Color)
            {
                GLint var = glGetUniformLocation(programID,"color");
                glUniform3f(var,Color.x,Color.y,Color.z);
                // position data
                var = glGetUniformLocation(programID,"model");
                // empty matrix here.
                auto m = Matrix<4>();
                glUniformMatrix4fv(var,1,GL_TRUE,m.unpack());
                drawRay(p2,A,B);
            };
            clean = false;
    };
    void cleanup()
    {
        clean = true;
        //clear polygonIDs
        for (auto v: BufferIDs)
        {
            v.second->destroy();
            delete v.second;
        }
        //clear buffer ids
        for (auto v: polygonBufferIDs)
        {
            v.second->destroy();
            delete v.second;
        };
        BufferIDs.clear();
        polygonBufferIDs.clear();
        glDeleteShader(programID);
    };
};
}