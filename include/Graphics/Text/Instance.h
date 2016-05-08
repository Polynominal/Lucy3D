#ifndef MIKUS_GRAPHICS_INSTANCE_H
#define MIKUS_GRAPHICS_INSTANCE_H

#include <Graphics/Text/Text.h>
#include <Graphics/Buffers/Canvas.h>
#include <Graphics/Bases/Moveable.h>

namespace Graphics
{
    class Text::Instance : public Graphics::Text, public  Graphics::Base::Moveable
    {
        friend class Text::Render;
        public:
            Instance(Graphics::Text* parent);
            virtual ~Instance();
            void setParent(Text* text){ parent = text;};
            Text::Task newTask(Text::Present);

            glm::mat4 Matrix;
            glm::vec3 Color;
        private:
            Text* parent=nullptr;
            bool costumRender = false;
            bool passive=false;
    };
    class Text::Render : public Graphics::Base::Moveable
    {
        public:
            friend Text::Task;
            // this is for the renderer to process start and finish the render
            Render(Text::Instance *parent);
            virtual ~Render();

            Render& operator << (const char* c){ return *this << std::string(c);};
            Render& operator << (string str);
            Render& operator << (const float a);
            // return child
            Render& operator << (const bool a);
            Render& operator << (Text::Type a);
            Task* operator << (Text::Present a);
            Render& operator << (std::shared_ptr<Font> f);
            Render& operator << (Font::subfont* a);
            void generate();
            void pre();
            void print(string str);
            void post();
            void setFixedGap(bool cond){fixedGap = cond;};
            void setGapSize(float gap){gapSize = gap;};

            void setColor(float r,float g,float b,float a);

            float getWidth(){return width;};
            float getHeight(){return height;};

        private:
            // this is used for keeping information inbetween frames but must be emptied each frame.
            std::shared_ptr<Graphics::Canvas> TextBuffer;
            std::shared_ptr<Graphics::Canvas> EffectsBuffer;
            std::unique_ptr<Utils::OpenGL::Buffer> Buffer;

            std::pair<float,float> renderString(string str,float offsetx,Font::subfont* sub);
            std::pair<float,float> calc(float offset,string t);
            // these also stand for intensity!!
            bool drawBase=true;


            glm::mat4 Matrix;
            glm::vec4 Color = glm::vec4(1.0f,1.0f,1.0f,1.0f);

            glm::vec4 Outline = glm::vec4(0.0f,0.0f,0.0f,1.0f);
            glm::vec4 Glow = glm::vec4(0.0f,0.0f,1.0f,1.0f);

            GLuint VAO;
            GLuint VBO;

            unsigned int used_size=0;
            float offset_x=0;

            float width = 0;
            float height = 0;

            bool fixedGap=false;
            bool matrixSwaped = false;
            float gapSize=0;

            Instance* parent;
            std::shared_ptr<Font> activeFont=nullptr;
            Font::subfont* activeSub=nullptr;
            std::vector<std::shared_ptr<Task>> Tasks;

    };
}


#endif // MIKUS_GRAPHICS_INSTANCE_H
