#ifndef MIKUS_GRAPHICS_TEXT_H
#define MIKUS_GRAPHICS_TEXT_H
#include <Graphics/Graphics.h>

#include <SDL.h>
#include <ft2build.h>
#include <memory.h>
#include <gtc/matrix_transform.hpp>
#include FT_FREETYPE_H

#include <Graphics/Shaders.h>
#include <Graphics/Shaders/Outline.h>
#include <Graphics/Shaders/Bloom.h> // for glow!

namespace Graphics
{
    extern std::unique_ptr<Shader> textShader;
}
class Graphics::Text
{
    public:
        // enums and nested items
        enum Type
        {
            Regular,
            Bold,
            Underlined,
            Italic,
        };
        enum Present
        {
            TypeOut,
            MatchOut,
            MatchType,
            TypeMatch
        };
        enum State
        {
            Pre,
            Post,
        };
        class Font //a better name for this should be called font family however Font is more intuitive.
            {
                public:
                    class subfont
                    {
                        public:
                            string location;
                            bool Finalized=false;

                            subfont(FT_Library ft,string path,float bsize);
                            bool generate();
                            float getMaxHeight(){return maxheight;};
                            int getWidth(char c);
                            int getHeight(char c);

                            ~subfont();

                            struct Character
                            {
                                 GLuint ID;
                                 glm::vec2 size;
                                 glm::vec2 bearing;
                                 GLuint advance;
                            };
                            float maxheight=0;
							float baseSize;
                            std::map<GLchar,Character*> Chars;
                            FT_Face face;

                    };

                    //
                    Font(FT_Library ft,float bSize);

                    bool add(Type t,string path);
                    int getWidth(string s,int size=128,Text::Type t=Text::Regular);
                    int getHeight(string s,int size=128,Text::Type t=Text::Regular);
                    subfont* find(Type t);

                    virtual ~Font();
                    //
                    static FT_Library ft;
                private:
                    float baseSize = 0;
                    std::map<Type,std::shared_ptr<subfont>> Family;
            };
        class Instance;
        class Render;
        class Task;

        friend class Render;

        Text(string font);
        Text(){};
        // non virtuals
        void setProjection(glm::mat4* projection);
        std::pair<float,float> renderString(string str,float offsetx,float offsety,float gapsize,bool fixedgap,Font::subfont* sub);
        std::pair<float,float> calculateSize(string text,int size);
        //virtuals

        virtual std::shared_ptr<Graphics::Canvas> render(string text,bool sendmatrix=true);
        // sets
        virtual void setWrapWidth(float width=-1){wrapWidth = width;};
        virtual void setSize(float s){size = s/baseSize;};
        virtual void setFixedGap(bool cond){fixedGap = cond;};
        virtual void setGapSize(float gap){gapSize = gap;};
        virtual std::shared_ptr<Text::Font> setFont(string path);
        virtual void setColor(float r,float g,float b,float a=255){cr = r/255;cg = g/255;cb = b/255;ca = 255/255;};
        virtual void set(Type t){activeType = t;}; // sets the current type
        //gets
        virtual Font::subfont* get(std::shared_ptr<Font>,Type);
        virtual Font::subfont* get(Type t){return get(t);};

        std::shared_ptr<Font> getFont(){return activeFont;};
        virtual float getSize(){return size/baseSize;};

        virtual std::pair<float,float> getDimensions(string s,int size2){return calculateSize(s,size2);}
        virtual float getWidth(string s, int size2){return getDimensions(s,size2).first;};
        virtual float getHeight(string s,int size2){return getDimensions(s,size2).second;};

        virtual float getWidth(string s){return getDimensions(s,size*baseSize).first;};
        virtual float getHeight(string s){return getDimensions(s,size*baseSize).second;};

        virtual float getWidth(string s,int size,Text::Type t);
        virtual float getHeight(string s,int size,Text::Type t);

        virtual float getFontHeight(int size);
        // misc
        virtual bool is(Type);
        virtual bool has(std::shared_ptr<Font>,Type);
        virtual bool has(string,Type);
        virtual bool has(Type t){return has(activeFont,t);};

        std::shared_ptr<Font> add(string path);
        void generate();
        void remove();

        virtual ~Text();

        //
    private:
        std::unique_ptr<Utils::OpenGL::Buffer> Buffer;
        std::unique_ptr<Graphics::Canvas> lastCanvas;

        static FT_Library ft;
        std::shared_ptr<Font> activeFont;
        Type   activeType=Type::Regular;

        std::map<string,std::shared_ptr<Font>> Fonts;

        glm::mat4* projection=nullptr;
        bool fixedGap=false;
        float wrapWidth = -1;
        float gapSize=0;

        float baseSize = 48;
        float size=1.0f;
        float width=0;
        float height=0;

        // color
        float cr=1.0f;
        float cg=1.0f;
        float cb=1.0f;
        float ca=1.0f;
};

#endif // MIKUS_GRAPHICS_TEXT_H
