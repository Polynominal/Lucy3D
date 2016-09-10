#ifndef MIKUS_LUCIA_GRAPHICS_BASES_COLORS_H
#define MIKUS_LUCIA_GRAPHICS_BASES_COLORS_H
#include <map>
#include <iostream>

#include <Lucia/Maths/General.h>
#include <Lucia/Utils/OpenGL.h>

namespace Lucia{
namespace Graphics{
namespace Base{
    class Color
    {
        public:
            enum COLOR
            {
                RED,
                DARKRED,
                LIGHTRED,

                GREEN,
                DARKGREEN,
                LIGHTGREEN,
                BLUE,
                DARKBLUE,
                LIGHTBLUE,
                CYAN,
                
                YELLOW,
                ORANGE,
                
                BLACK,
                DARKGRAY,
                GRAY,
                LIGHTGRAY,
                
                VIOLET,
                WHITE
            };
            
            Color(float r,float g,float b,float a=255.0f){setColor(r,g,b,a);};
            Color(COLOR c);
            Color(){};
            ~Color(){};
            
            //color
            struct mech
            {
                mech(){};
                mech(float r2,float g2,float b2,float a2=1.0f){r = r2;g = g2;b = b2;a = a2;};
                float r = 1.0f;
                float g = 1.0f;
                float b = 1.0f;
                float a = 1.0f;
            }; 
            struct rgba
            {
                rgba(){};
                rgba(float r2,float g2,float b2,float a2=255){r = r2;g = g2;b = b2;a = a2;};
                float r = 255;
                float g = 255;
                float b = 255;
                float a = 255;
            };
            struct hsv
            {
                hsv(){};
                hsv(float h2,float s2,float v2){h = h2;s = s2;v = v2;};
                float h = 360;
                float s = 100;
                float v = 100;
            };
            //SET
            void setColor(float r,float g,float b,float a=255);
            void setColor(rgba b);
            void setColor(mech m);
            void setColor(hsv h);
            void setColor(COLOR c);
            void setColorHSV(float h,float s,float v);
            void setColorMechanical(float r,float g,float b,float a=1.0f);
            
            //GET
            rgba getColor();
            hsv getHSVColor();
            mech getMechanicalColor();
            
            //
            bool applyColor(std::shared_ptr<Utils::OpenGL::Shader_Vars> vars,mech colors);
            bool applyColor(std::shared_ptr<Utils::OpenGL::Shader_Vars> vars){return applyColor(vars,Core);};
            bool applyColor(std::shared_ptr<Utils::OpenGL::Shader_Vars> vars,rgba color){return applyColor(vars,toMech(color));};
            bool applyColor(std::shared_ptr<Utils::OpenGL::Shader_Vars> vars,hsv color){return applyColor(vars,toMech(color));};
            
        private:
            mech Core = mech(1.0f,1.0f,1.0f);
            
            rgba toRGBA(hsv c);
            rgba toRGBA(mech c){return rgba(c.r*255,c.g*255,c.b*255,c.a*255);};
            
            hsv toHSV(rgba c);
            hsv toHSV(mech c){return toHSV(toRGBA(c));};
            
            mech toMech(rgba c){return mech(c.r/255.0f,c.g/255.0f,c.b/255.0f,c.a/255.0f);};
            mech toMech(hsv c){return toMech(toRGBA(c));};
    };
   inline std::ostream& operator<< (std::ostream& os,const Color::rgba &n){
        os << std::endl << "--RGBA Color--" << std::endl;
        os << "R: " << n.r << " G: "<< n.g << " B: " << n.b << " A: " << n.a << std::endl;
        os << "----------" << std::endl;
        return os; 
    };
    inline std::ostream& operator<< (std::ostream& os,const Color::mech& n){
        os << std::endl << "--Mechanical Color--" << std::endl;
        os << "R: " << n.r << " G: "<< n.g << " B: " << n.b << " A: " << n.a << std::endl;
        os << "----------" << std::endl;
        return os; 
    };
    inline std::ostream& operator<< (std::ostream& os,const Color::hsv &n){
        os << std::endl << "--HSV Color--" << std::endl;
        os << "H: " << n.h << " S: "<< n.s << " V: " << n.v << std::endl;
        os << "----------" << std::endl;
        return os; 
    };
}}}
#endif