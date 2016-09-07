#include "Lucia/Graphics/Text/Text.h"
// FONT
namespace Lucia {
namespace Graphics
{
    std::unique_ptr<Shader> textShader;
    FT_Library Text::ft = NULL;
    FT_Library Text::Font::ft = NULL;

    Text::Font::subfont::subfont(FT_Library ft,string path,float bsize)
    {
        face = FT_Face();
        location = path;
		baseSize = bsize;
    }
    int Text::Font::subfont::getWidth(char c)
    {
        auto v = Chars[c];
        return (v->advance >> 6);
    }
    int Text::Font::subfont::getHeight(char c)
    {
        auto v = Chars[c];
        return v->size.y;
    }
    bool Text::Font::subfont::generate()
    {
        Finalized = false;
        if (FT_New_Face(ft,location.c_str(),0,&face)){return false;};
        glPixelStorei(GL_UNPACK_ALIGNMENT,1);
        FT_Set_Pixel_Sizes(face, 0, baseSize);

        // clean up
        Chars.clear();

        for (GLubyte  c=0;c < 128;c++)
        {
            if (!FT_Load_Char(face,c,FT_LOAD_RENDER))
            {
                GLuint texture;
                glGenTextures(1,&texture);
                glBindTexture(GL_TEXTURE_2D, texture);
                glTexImage2D(
                    GL_TEXTURE_2D,
                    0,
                    GL_ALPHA,
                    face->glyph->bitmap.width,
                    face->glyph->bitmap.rows,
                    0,
                    GL_ALPHA,
                    GL_UNSIGNED_BYTE,
                    face->glyph->bitmap.buffer
                );

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

                Character* cn = new Character{
                    texture,
                    Vertex(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                    Vertex(face->glyph->bitmap_left, face->glyph->bitmap_top),
                    face->glyph->advance.x
                };
                Chars.insert({c,cn});
                maxheight = std::max(maxheight,float(cn->size.y));
            }
        }
        Finalized = true;
    }
    Text::Font::subfont::~subfont()
    {
        for (auto v: Chars)
        {
            delete v.second;
        }
        Chars.clear();
        FT_Done_Face(face);
    }



    Text::Font::Font(FT_Library f,float bsize)
    {
        ft = f;
        baseSize = bsize;
    };
    bool Text::Font::add(Type t,string path)
    {
        auto sf = std::make_shared<subfont>(ft,path,baseSize);
        bool f = sf->generate();
        Family.insert({t,sf});
        return f;
    };
    Text::Font::subfont* Text::Font::find(Type t)
    {
        auto sf = Family.find(t);
        if (sf == Family.end())
        {
            return nullptr;
        }else
        {
            return sf->second.get();
        }
    };
    int Text::Font::getWidth(string s,int size,Text::Type t)
    {
        float w=0;
        auto sf = find(t);
        if (sf == nullptr)
        {return 0;};

        for (auto v: s)
        {
            float nw = sf->getWidth(v)/((float)baseSize); // size of size 1 font
            w = w + (nw*size);
        }
        return w;
    }
    int Text::Font::getHeight(string s,int size,Text::Type t)
    {
        float h=0;
        auto sf = find(t);

        if (sf == nullptr)
        {return 0;};

        for (auto v: s)
        {
            float nh = sf->getHeight(v)/((float)baseSize); // size of size 1 font
            h = std::max(nh,nh*size);
        }
        return h;
    }
    Text::Font::~Font()
    {
        Family.clear();
    }
    // EDN
    Text::Text(string path)
    {
        // producing buffers:
        if (Graphics::textShader.get() == nullptr)
        {
            string Vertex =
            #if !(defined LUCIA_USE_GLES3 || defined LUCIA_USE_GLES2)
            "#version 330 core \n"
            "layout (location = 0) in vec4 vertex;\n"
            "out vec2 TexCoords;\n"
            #else
            "#version 100 \n"
            "attribute vec4 vertex;\n"
            "varying vec2 TexCoords;\n"
            #endif
            "uniform mat4 projection;\n"
            "uniform mat4 model;\n"
            "void main()\n"
            "{\n"
            "   gl_Position = model * projection * vec4(vertex.xy, 0.0, 1.0);\n"
            "   TexCoords = vertex.zw;\n"
            "}; \n";

            string Fragment =
            #if !(defined LUCIA_USE_GLES3 || defined LUCIA_USE_GLES2)
            "#version 330 core \n"
            "in vec2 TexCoords;\n"
            "out vec4 color;\n"
            #else
            "#version 100 \n"
            "precision highp float;"
            "varying vec2 TexCoords;\n"
            "vec4 color\n;"
            "vec4 texture(sampler2D s,vec2 pos)"
            "{return texture2D(s,pos);};"
            #endif // LUCIA_USE_GLES2
            "uniform sampler2D text;\n"
            "uniform vec4 textColor;\n"
            "void main()\n"
            "{\n"
            "   vec4 sampled = vec4(1.0, 1.0, 1.0,texture(text, TexCoords).a);\n"
            "   color = textColor * sampled;\n"
            #if defined LUCIA_USE_GLES3 || defined LUCIA_USE_GLES2
            "gl_FragColor = color;"
            #endif // LUCIA_USE_GLES2
            "};\n";
            Graphics::textShader.reset(new Shader());
            Graphics::textShader->build(Vertex,Fragment,"textShader");
            Graphics::textShader->addVar("vertex",0,4);
        };
        if (FT_Init_FreeType(&ft))
        {
            std::cout << "Error loading freetype!" << std::endl;
        };
        activeFont = std::make_shared<Font>(ft,baseSize);
        activeFont->add(Type::Regular,path);
        Fonts.insert({path,activeFont});
        generate();
        //ctor
    }
    void Text::setProjection(Maths::Matrix<4>* p)
    {
        projection = p;
    }
    void newLine(Text::Font::subfont* sub,float size,float &offsetx,float &offsety,float oldoffsetx)
    {
        offsetx = oldoffsetx;
        offsety = offsety - (sub->getMaxHeight()*size);
    }
    std::pair<float,float> Text::renderString(string text,float offsetx,float offsety,float gapsize,bool fixedgap,Font::subfont* sub)
    {
        float s = size;
        float pyy = 0;
        float oldoffsetx = offsetx;
        for (unsigned int i=0;i < text.size();i++)
        {
            char v = text[i];
            if (v == '\n')
            {
                newLine(sub,s,offsetx,offsety,oldoffsetx);
            }else
            {
                if (wrapWidth != -1 and (offsetx) > wrapWidth)
                {
                    newLine(sub,s,offsetx,offsety,oldoffsetx);
                };
                auto ch = sub->Chars[v];
                float w = (float)ch->size.x * s;
                float h = (float)ch->size.y * s;

                float px = (offsetx + ch->bearing.x * s);
                // 1.0f for a slight offset preventing a loss of the bottom of the text
                float py = (offsety - (ch->size.y - ch->bearing.y) * s) + 0.2f*s;

                GLfloat verts[24] =
                {
                    px, py + h, 0.0,0.0,
                    px, py,     0.0,1.0,
                    px + w,py,  1.0,1.0,

                    px,py + h,  0.0,0.0,
                    px + w,py,  1.0,1.0,
                    px + w,py + h,  1.0,0.0
                };
                //fail safe:
                if (Buffer.get() == nullptr){generate();};
                // Update content of VBO memory
                Buffer->sendData(Buffer->convertToData(verts,0,6),0);
                glBindTexture(GL_TEXTURE_2D,ch->ID);

                Buffer->draw(GL_TRIANGLES);
                int bearing = ch->advance;

                if (fixedgap)
                {
                    // 1.2 because we want to cut down on the larger characters in favour of letters!
                    bearing = ((baseSize*100)/1.2)*s;
                }
                offsetx += ((bearing >> 6) * s) + gapsize;
                pyy = std::max(py,pyy);
                glBindTexture(GL_TEXTURE_2D,0);
            }
        }
        return std::pair<float,float>(offsetx,pyy);
    }
    std::pair<float,float> Text::calculateSize(string text, int size2)
    {
        float nsize = (size2/baseSize);
        float width=0;
        float height=0;
        auto sf = activeFont->find(activeType);
        float x=0;
        float newLines=1;
        for (auto i=0;i < text.size();i++)
        {
            auto v = text[i];
            if( v == '\n')
            {
                x = 0;
                newLines++;
            }else
            {
                auto ch = sf->Chars[v];
                int bearing = ch->advance;
                if (fixedGap)
                {
                    // 1.2 because we want to cut down on the larger characters in favour of letters!
                    bearing = ((baseSize*100)/1.2)*nsize;
                }
                x += ((bearing >> 6) * nsize) + gapSize;
                width = std::max(width,x);
            }
        };
        height = (sf->getMaxHeight()*nsize)*newLines;

        //safety check
        if (height < 0.0f){height = 0.0f;};
        if (width < 0.0f){width = 0.0f;};
        return std::pair<int,int>(width,height);
    }
    float Text::getFontHeight(int size2)
    {
        float nsize = size2/baseSize;
        auto sf = activeFont->find(activeType);
        return sf->getMaxHeight()*nsize;
    };
    std::shared_ptr<Graphics::Canvas> Text::render(string text,bool sendmatrix)
    {
        auto sf = activeFont->find(activeType);
        auto pair = calculateSize(text,baseSize);
        float height =  pair.second;
        auto canvas = std::make_shared<Graphics::Canvas>();
        canvas->generate(std::max(pair.first,1.0f),height);

        glUseProgram(Graphics::textShader->programID);
        glActiveTexture(GL_TEXTURE0);
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glUniform4f(glGetUniformLocation(Graphics::textShader->programID, "textColor"), 1.0f,1.0f,1.0f,1.0f);
        if (projection == nullptr)
        {
            GLint dim[4];
            glGetIntegerv(GL_VIEWPORT,&dim[0]);
            auto d = Ortho(0.0f,(float)dim[2],0.0f,(float)dim[3],-100.0f,100.0f);
            glUniformMatrix4fv(glGetUniformLocation(Graphics::textShader->programID, "projection"),
            1,GL_FALSE,d.unpack());
        }else
        {
            glUniformMatrix4fv(glGetUniformLocation(Graphics::textShader->programID, "projection"),
            1,GL_FALSE,projection->unpack());
        }
        Maths::Matrix<4> mat = Maths::Matrix<4>();
        glUniformMatrix4fv(glGetUniformLocation(Graphics::textShader->programID, "model"),1,GL_TRUE,mat.unpack());

        canvas->attach(true,true);
        renderString(text,0.0f,pair.second - sf->getMaxHeight()*size,gapSize,fixedGap,sf);
        canvas->detach();

        glDisable(GL_BLEND);
        glBindTexture(GL_TEXTURE_2D, 0);
        glUseProgram(0);

        return canvas;
    };
    float Text::getWidth(string s,int size,Text::Type t)
    {
        return activeFont->getWidth(s,size,t);
    }
    float Text::getHeight(string s,int size,Text::Type t)
    {
        return activeFont->getHeight(s,size,t);
    }

    bool Text::is(Type t)
    {
        return (activeType == t);
    }
    bool Text::has(string name,Type ty)
    {
        std::shared_ptr<Graphics::Text::Font> t;
        auto entry = Fonts.find(name);
        if (entry != Fonts.end()){t = entry->second;}
        else{t = activeFont;};
        return (t->find(ty) != nullptr);
    }
    bool Text::has(std::shared_ptr<Font> font,Type ty)
    {
        return (font->find(ty) != nullptr);
    }
    Text::Font::subfont* Text::get(std::shared_ptr<Text::Font> f,Type ty)
    {
        return f->find(ty);
    }
    std::shared_ptr<Text::Font> Text::add(string path)
    {
        auto f = std::make_shared<Text::Font>(ft,baseSize);
        f->add(Text::Type::Regular,path);
        Fonts.insert({path,f});
        return f;
    }
    std::shared_ptr<Text::Font> Text::setFont(string path)
    {
        std::shared_ptr<Text::Font> t;
        auto entry = Fonts.find(path);
        if (entry != Fonts.end())
        {
            t = entry->second;
        }else
        {
            t = add(path);
        }
        activeFont = t;
        activeType = Type::Regular;
        return t;
    }
    void Text::generate()
    {
        Buffer.reset(new Utils::OpenGL::Buffer(Graphics::textShader->getVars()));
        Buffer->allocateData(6);
    }
    void Text::remove()
    {
        Buffer->destroy();
        Fonts.clear();
        FT_Done_FreeType(ft);
    }
    Text::~Text()
    {
        //dtor
    }

}
}
