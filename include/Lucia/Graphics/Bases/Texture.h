#ifndef MIKUS_LUCIA_GRAPHICS_BASES_TEXTURE_H
#define MIKUS_LUCIA_GRAPHICS_BASES_TEXTURE_H

#include <Lucia\Utils\OpenGL.h>
#include <Lucia/Graphics/Shaders.h>
#include <Lucia/Graphics/Primitives/Quad.h>
#include <Lucia/Graphics/Bases/Filter.h>

namespace Lucia
{
    namespace Graphics{
        struct ImageData
        {
            ImageData(std::string d,float w,float h)
            {
                height = w;
                height = h;
                data = d;
            };
            const char* getData(){return data.c_str();};
            float height=0;
            float width=0;
            std::string data="";
        };
    namespace Base{
        class Texture:
        public Graphics::Base::Filter
        {
            //misc
        public:
            Texture(){};
            virtual ~Texture(){};
            virtual void remove()
            {
                glDeleteTextures(1,&textureID);
            };
            
            //get 
            virtual unsigned int getID()
            {
                return textureID;
            };
            virtual float getWidth(){return width;};
            virtual float getHeight(){return height;};
            virtual void generateMipMap()
            {
                glBindTexture(GL_TEXTURE_2D, textureID);
                #if defined LUCIA_USE_GLES2 || defined LUCIA_USE_GLES3
                glGenerateMipmap(GL_TEXTURE_2D);
                #else
                glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
                #endif
                glBindTexture(GL_TEXTURE_2D, 0);
            };
            virtual ImageData getImageData()
            {
                
                #ifndef LUCIA_USE_GLES2
                    bindTexture();
                    GLubyte data;
                    glGetTexImage(GL_TEXTURE_2D,0,GL_RGBA8,GL_UNSIGNED_BYTE,&data);
                    return ImageData(std::string(reinterpret_cast<const char*>(data)),width,height);
                #else
                return ImageData("0",0,0);
                #endif
            };
            //SET
            virtual void setQuad(Graphics::Primitive::Quad* q){Quad = q;};
            virtual void setName(std::string new_name){name = new_name;}
            //
            virtual void bind()
            {
                bind(0);
            };
            virtual void preDrawTasks()
            {
                applyFilters();
            };
            virtual void bind(int number)
            {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, textureID);
                preDrawTasks();
            };
            virtual void bind(int number,int shaderID)
            {
                GLfloat no = number;

                glActiveTexture(GL_TEXTURE0 + number);
                glBindTexture(GL_TEXTURE_2D, textureID);
                preDrawTasks();
                
                GLint programID = 0;
                glGetIntegerv(GL_CURRENT_PROGRAM,&programID);
                if (name != "")
                {
                    glUniform1i(glGetUniformLocation(programID,name.c_str()),no);
                };
            };
            virtual GLuint getTextureID(){return textureID;};
            virtual void bindTexture()
            {
                glBindTexture(GL_TEXTURE_2D,textureID);
            };
            virtual void renderQuad(bool ownShader=true,bool resetShader=true)
            {
                GLint lastShader=-1.0f;
                GLboolean hasDepth = glIsEnabled(GL_DEPTH_TEST);
                if (ownShader){
                    if (resetShader){glGetIntegerv(GL_CURRENT_PROGRAM,&lastShader);};

                    glUseProgram(Graphics::_Shaders::Basic2D->programID);
                    // send necessary parameters such as that we want to use a texture.
                    glUniform1i(glGetUniformLocation(Graphics::_Shaders::Basic2D->programID, "useColor"),false);
                }
                GLint lastTexture;
                glGetIntegerv(GL_TEXTURE_BINDING_2D,&lastTexture);

                glDisable(GL_DEPTH_TEST);
                glActiveTexture(GL_TEXTURE0);

                glBindTexture(GL_TEXTURE_2D,textureID);
                if (Quad == nullptr)
                {
                    Graphics::Shared::Quad->draw();
                }else
                {
                    Quad->draw();
                }
                glBindTexture(GL_TEXTURE_2D,lastTexture);

                if (ownShader and resetShader){glUseProgram(lastShader);}
                if (hasDepth){glEnable(GL_DEPTH_TEST);};
            };
            
        protected:
            Graphics::Primitive::Quad* Quad=nullptr;
            std::string name="";
            GLenum format =
            #ifndef LUCIA_USE_GLES2
             GL_RGBA8;
            #else 
             GL_RGBA;
            #endif
            GLuint textureID=0;
            float width=0;
            float height=0;
        };
    }
}
}
#endif