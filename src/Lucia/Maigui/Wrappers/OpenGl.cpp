#include <Lucia/Maigui/Wrappers/OpenGl.h>
#include <Lucia/Maigui/Skin.h>
#include <Lucia/Utils/OpenGL.h>
#include <Lucia/Graphics/Text/Text.h>
namespace Lucia {
namespace Maigui
{
    namespace OpenGL
    {
        struct Color
        {
            Color(float cr,float cg,float cb,float ca)
            {
                set(cr,cg,cb,ca);
            };
            void set(float cr,float cg,float cb,float ca)
            {
                r = cr;g=cg;b=cb;a=ca;
            };
            float r=0.0f;
            float g=0.0f;
            float b=0.0f;
            float a=1.0f;
        };
        Matrix<4> translation;
        std::unique_ptr<Color> currentColor(new Color(255,255,255,255));
        GLuint programID =0;
        GLuint VaoID = 0;
        GLuint VboID = 0;
        GLuint EboID = 0;
        GLuint tProgramID = 0;

        Shader activeShader = Shader::NIL;

        Utils::OpenGL::Buffer* Quad2D;
        std::unique_ptr<Graphics::Text> tx;
        std::map<int,std::shared_ptr<Graphics::Buffer::Canvas>> BufferedTexts;

        void init()
        {
            std::cout << "Compiling wrapper Maigui text shader" << std::endl;

            string Vertex =
            "#version 100 \n"
            "attribute vec3 vertex;"
            "attribute vec2 texCord;"
            "varying vec2 TexCord;"
            "varying float usecolor;"
            "varying vec4 Color;"
            "uniform mat4 model;"
            "uniform mat4 projection;"
            "uniform mat4 view;"
            "uniform float useColor;"
            "uniform vec4 color;"
            "void main()"
            "{"
            "   TexCord = texCord;"
            "   gl_Position = projection * view * model * vec4(vertex, 1.0f);"
            "   usecolor = useColor;"
            "   Color = color;"
            "}";
            string Fragment =
            "#version 100 \n"
            "precision highp float;"
            "varying vec2 TexCord;"
            "varying float usecolor;"
            "varying vec4 Color;"
            "uniform sampler2D Texture;"
            "void main()"
            "{"
            "   if (usecolor==1.0f)"
            "   {"
            "       gl_FragColor = Color;"
            "   }else{gl_FragColor = Color*texture2D(Texture,TexCord);};"
            "}";
            tProgramID = Collider_OpenGL::LoadShaderSource(Vertex.c_str(),Fragment.c_str());
            auto Vars = std::make_shared<Utils::OpenGL::Shader_Vars>();
            Vars->setProgram(tProgramID);
            Vars->add("vertex",0,3);
            Vars->add("texCord",3,5);

            Quad2D = new Utils::OpenGL::Buffer(Vars);
            GLfloat Quad[] =
            {
                -0.5f, -0.5f, 0.0f,     0.0f, 0.0f,
                 0.5f, -0.5f, 0.0f,     1.0f, 0.0f,
                 0.5f,  0.5f, 0.0f,     1.0f, 1.0f,

                 0.5f,  0.5f, 0.0f,     1.0f, 1.0f,
                -0.5f,  0.5f, 0.0f,     0.0f, 1.0f,
                -0.5f, -0.5f, 0.0f,     0.0f, 0.0f
            };
            Quad2D->setData(Quad,6);

            programID = Collider_OpenGL::LoadShaders("assets/Maigui/Shaders/Shader.vert","assets/Maigui/Shaders/Shader.frag");
        }


        void createFromUV(float topx,float topy,float bottomx,float bottomy)
        {

        }
        Vec2 toMechanical(float nx,float ny,float w,float h)
        {
            float x = nx/w;
            float y = (ny/h);
            return Vec2(x,y);
        };
        void setTextShader(Matrix<4> translation,Maths::Matrix<4> *view,Maths::Matrix<4> *projection)
        {
                GLuint var=0;
                glUseProgram(tProgramID);
                activeShader = Shader::TEXT;

                var = glGetUniformLocation(tProgramID,"model");
                glUniformMatrix4fv(var,1,GL_TRUE,translation.unpack());
                var = glGetUniformLocation(tProgramID,"view");
                glUniformMatrix4fv(var,1,GL_TRUE,view->unpack());
                var = glGetUniformLocation(tProgramID,"projection");
                glUniformMatrix4fv(var,1,GL_FALSE,projection->unpack());
                glUniform1f(glGetUniformLocation(programID,"useColor"),false);
                var = glGetUniformLocation(tProgramID,"color");
                glUniform4f(var,currentColor->r,currentColor->g,currentColor->b,currentColor->a);

                glDisable(GL_DEPTH_TEST);

                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

                glActiveTexture(GL_TEXTURE0);
        }
        void setTextShader(Matrix<4> translation)
        {
            glUseProgram(tProgramID);

            auto var = glGetUniformLocation(tProgramID,"model");
            glUniformMatrix4fv(var,1,GL_TRUE,translation.unpack());

            var = glGetUniformLocation(tProgramID,"color");
            glUniform4f(var,currentColor->r,currentColor->g,currentColor->b,currentColor->a);

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glActiveTexture(GL_TEXTURE0);
        }
        void drawColoredRectangle(float r,float g,float b,float a,Matrix<4> translation)
        {
            glUniform1f(glGetUniformLocation(tProgramID,"useColor"),1);
            auto var = glGetUniformLocation(tProgramID,"color");
            glUniform4f(var,r,g,b,a);
            Quad2D->draw(GL_TRIANGLES);
            glUniform1f(glGetUniformLocation(tProgramID,"useColor"),0);
            glUniform4f(var,0.0f,0.0f,0.0f,1.0f);
        };
        void attachGraphicalShader(Skin *s,Maths::Matrix<4> *view,Maths::Matrix<4> *projection)
        {
                auto Image = s->getData();
                Image->setFilter(Graphics::FILTER::LINEAR);
                glEnable(GL_DEPTH_TEST);
                glDepthFunc(GL_LEQUAL);
                glEnable(GL_BLEND);
                glBlendFunc(GL_ONE,GL_ONE_MINUS_SRC_ALPHA);

                glUseProgram(programID);
                activeShader = Shader::ITEM;

                GLuint var=0;
                var = glGetUniformLocation(programID,"view");
                glUniformMatrix4fv(var,1,GL_TRUE,view->unpack());
                var = glGetUniformLocation(programID,"projection");
                glUniformMatrix4fv(var,1,GL_FALSE,projection->unpack());

                Image->bind();
                Image->forceApplyFilters();
        }
        void attachWrappers(Skin *s,Maths::Matrix<4> *view,Maths::Matrix<4> *projection)
        {
            init();
            s->PreDraw = [view,projection,s]()
            {
                glClear(GL_DEPTH_BUFFER_BIT);
                attachGraphicalShader(s,view,projection);
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            
            };
            s->PostDraw = []()
            {
                activeShader = Shader::NIL;
                glDisable(GL_BLEND);
                glUseProgram(0);
            };
            s->setTextColor = [](float r,float g,float b,float a)
            {
                currentColor->set(r,g,b,a);
            };
            s->setFont = [](string path)
            {
                if (tx.get() == nullptr)
                {
                    tx.reset(new Graphics::Text(path));
                }else
                {
                    tx->setFont(path);
                }
            };
            s->bufferText = [](string text)
            {
                auto c = tx->render(text);
                int id = BufferedTexts.size();
                BufferedTexts.insert({id,c});
                return id;
            };
            s->delBuffer = [](int id)
            {
                BufferedTexts.erase(id);
            };
            s->getFontHeight = [](int size){return tx->getFontHeight(size);};
            s->getTextWidth = [](string text,int size){return tx->getWidth(text,size);};
            s->printText = [view,projection](string text,Matrix<4> Translation)
            {
                auto canvas = tx->render(text);
                setTextShader(Translation,view,projection);
                glEnable(GL_BLEND);
                glBlendFunc(GL_ONE,GL_ONE_MINUS_SRC_ALPHA);
                canvas->bindTexture();
                Quad2D->draw(GL_TRIANGLES);
            };
            s->printBufferedText = [view,projection](int id,Matrix<4> Translation)
            {
                if (activeShader != Shader::TEXT){setTextShader(Translation,view,projection);}
                else{setTextShader(Translation);};
                auto canvas = BufferedTexts.find(id);
                if (canvas != BufferedTexts.end())
                {
                    canvas->second->bindTexture();
                    Quad2D->draw(GL_TRIANGLES);
                }
                else
                {
                    std::cout << "[MAIGUI] Buffered text ID is not recognized!" << std::endl;
                }
            };
            s->setMatrix = [](Matrix<4> Mat){translation = Mat;};
            s->drawImageItem=[s,view,projection](Graphics::Image* image,Maths::Sprite* sprite,float r,float g,float b,float a)
            {
                auto Image = image;
                if(Image == nullptr)
                {
                    Image = s->getData().get();
                };
                Image->bind();
                
                Maths::Sprite* d;
                if (sprite == nullptr)
                {
                    d = new Maths::Sprite(0,0,Image->getWidth(),Image->getHeight(),Image->getWidth(),Image->getHeight());
                }else
                {
                    d = sprite;
                };
                if (activeShader != Shader::ITEM){attachGraphicalShader(s,view,projection);};
                Vertex scale = translation.getScale();
                Vec2 extension = toMechanical((scale.x - d->w),(scale.y - d->h),scale.x,scale.y);
                extension = Vec2((d->bottom.x - d->top.x)*extension.x,(d->bottom.y - d->top.y)*extension.y);
                // position data
                
                GLuint var=0;
                var = glGetUniformLocation(programID,"Color");
                glUniform4f(var,r,g,b,a);

                var = glGetUniformLocation(programID,"model");
                glUniformMatrix4fv(var,1,GL_TRUE,translation.unpack());

                var = glGetUniformLocation(programID,"Range");
                glUniform4f(var,d->top.x,d->top.y,d->bottom.x,d->bottom.y);

                var = glGetUniformLocation(programID,"Extension");
                glUniform2f(var,extension.x,extension.y);
                
                var = glGetUniformLocation(programID,"Aspect");
                glUniform2f(var,scale.x/Image->getWidth(),scale.y/Image->getHeight());

                var = glGetUniformLocation(programID,"UV");
                glUniform4f(var,d->top.x,d->top.y,d->bottom.x - d->top.x,d->bottom.y - d->top.y);
                
                #ifndef LUCIA_USE_GLES2
                    glBindVertexArray(Quad2D->vao);
                #else
                    glBindBuffer(GL_ARRAY_BUFFER,Quad2D->vbo);

                    GLuint pos = glGetAttribLocation(programID,"position");
                    GLuint tex = glGetAttribLocation(programID,"texCoord");
                    
                    glEnableVertexAttribArray(pos);
                    glVertexAttribPointer(pos,3,GL_FLOAT,GL_FALSE,5*sizeof(GLfloat),(GLvoid*)0);
                    glEnableVertexAttribArray(tex);
                    glVertexAttribPointer(tex, 2, GL_FLOAT,GL_FALSE,5*sizeof(GLfloat), (GLvoid*)(sizeof(GLfloat)*3));
                
                #endif
                        glDrawArrays(GL_TRIANGLES,0,6);
                
                #ifndef LUCIA_USE_GLES2
                    glBindVertexArray(0);
                #else
                    glBindBuffer(GL_ARRAY_BUFFER,0);
                #endif
                
                if (sprite == nullptr)
                {
                    delete d;
                };
            };
            
            s->drawColoredItem = [s,view,projection](Skin::Drawable *d,float r,float g,float b,float a)
            {
                    switch(d->type)
                    {
                        case Skin::Shapes::Rectangle:
                        {
                            setTextShader(translation,view,projection);
                            drawColoredRectangle(d->x,d->y,d->w,d->h,translation);
                            break;
                        }
                        case Skin::Shapes::None:
                        {

                            if (activeShader != Shader::ITEM){attachGraphicalShader(s,view,projection);};
                            auto Image = s->getData();
                            Vertex scale = translation.getScale();
                            Vec2 extension = toMechanical((scale.x - d->w),(scale.y - d->h),scale.x,scale.y);
                            extension = Vec2((d->bottom.x - d->top.x)*extension.x,(d->bottom.y - d->top.y)*extension.y);

                            // position data
                            GLuint var=0;
                            var = glGetUniformLocation(programID,"Color");
                            glUniform4f(var,r,g,b,a);

                            var = glGetUniformLocation(programID,"model");
                            glUniformMatrix4fv(var,1,GL_TRUE,translation.unpack());

                            var = glGetUniformLocation(programID,"Range");
                            glUniform4f(var,d->top.x,d->top.y,d->bottom.x,d->bottom.y);

                            var = glGetUniformLocation(programID,"UV");
                            glUniform4f(var,d->top.x,d->top.y,d->bottom.x - d->top.x,d->bottom.y - d->top.y);
                            
                            var = glGetUniformLocation(programID,"Extension");
                            glUniform2f(var,extension.x,extension.y);
                            
                            var = glGetUniformLocation(programID,"Aspect");
                            glUniform2f(var,scale.x/Image->getWidth(),scale.y/Image->getHeight());
                            
                            #ifndef LUCIA_USE_GLES2
                                glBindVertexArray(Quad2D->vao);
                            #else
                                glBindBuffer(GL_ARRAY_BUFFER,Quad2D->vbo);

                                GLuint pos = glGetAttribLocation(programID,"position");
                                GLuint tex = glGetAttribLocation(programID,"texCoord");
                                
                                glEnableVertexAttribArray(pos);
                                glVertexAttribPointer(pos,3,GL_FLOAT,GL_FALSE,5*sizeof(GLfloat),(GLvoid*)0);
                                glEnableVertexAttribArray(tex);
                                glVertexAttribPointer(tex, 2, GL_FLOAT,GL_FALSE,5*sizeof(GLfloat), (GLvoid*)(sizeof(GLfloat)*3));
                            
                            #endif
                                    glDrawArrays(GL_TRIANGLES,0,6);
                            
                            #ifndef LUCIA_USE_GLES2
                                glBindVertexArray(0);
                            #else
                                glBindBuffer(GL_ARRAY_BUFFER,0);
                            #endif
                            
                            break;
                        }
                    }
            };
            s->drawItem = [s,view,projection](Skin::Drawable *d)
            {
                s->drawColoredItem(d,1.0f,1.0f,1.0f,1.0f);
            };
            s->drawColoredRectangle = [view,projection](float r,float g,float b,float a)
            {
                
                setTextShader(translation,view,projection);
                drawColoredRectangle(r,g,b,a,translation);
            };
        };
        void cleanUp()
        {
            delete Quad2D;
            #ifndef LUCIA_USE_GLES2
            glDeleteVertexArrays(1,&VaoID);
            #endif
            glDeleteBuffers(1,&VboID);
        };
    }
}
}
