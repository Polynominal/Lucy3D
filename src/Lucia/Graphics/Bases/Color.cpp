#include "Lucia\Graphics\Bases\Color.h"

using namespace Lucia;
using namespace Maths;
using namespace Graphics;
using namespace Bases;

Color::Color(Color::COLOR c)
{
    setColor(c);
};
Color::rgba Color::toRGBA(Color::hsv c)
{
    float r = 0, g = 0, b = 0;
    float h = c.h, s = Maths::max(c.s,1.0f)/100, v = c.v/100;
    h = Maths::fmod(h,360.0f);
    if (s==0){return Color::rgba(v,v,v);};
    h=h/60.0f;
    
    float i=Maths::floor(h);
    float f=h-i;
    float p=v*(1-s);
    float q=v*(1-s*f);
    float t=v*(1-s*(1-f));
    
    if (i==0){r=v; g=t; b=p;}
    else if (i==1){ r=q; g=v; b=p;}
    else if (i==2){ r=p; g=v; b=t;}
    else if (i==3){ r=p; g=q; b=v;}
    else if (i==4){ r=t; g=p; b=v;}
    else {r=v; g=p; b=q;}
    
    r=r*255;
    g=g*255;
    b=b*255;
    return Color::rgba(r,g,b);  
};
Color::hsv Color::toHSV(Color::rgba c)
{
    float epsilon = 0.0000001f;
    float r = c.r, g = c.g, b = c.b;
    float h = 0, s = 0, v = 0;
    float min = Maths::min(r,Maths::min(g,b));
    float max = Maths::max(r,Maths::max(g,b));
    float delta = max - min;
    
    s = delta/(max + epsilon);
    v = max/255;
    
    if (r == max)
    {
        h = (g - b)/(delta + epsilon);
    }
    else if (g == max)
    {
        h = 2 + (b - r)/(delta + epsilon);  
    } 
    else
    {
        h  = 4 + (r - g)/(delta + epsilon);
    };
    if (h < 0){ h = h + 6; };
    
    h = h * (1.0f/6.0f);
    
    return Color::hsv(Maths::max(h*360,1.0f), Maths::max(s*100,1.0f), Maths::max(v*100,1.0f));
};

void Color::setColor(float r,float g,float b,float a)
{
    setColor(Color::rgba(r,g,b,a));
}
void Color::setColor(Color::mech m)
{
    Core = m;
}
void Color::setColor(Color::rgba b)
{
    setColor(toMech(b));
}

void Color::setColor(Color::hsv h)
{
    Core = toMech(h);
}
void Color::setColor(COLOR c)
{
    rgba r;
    switch(c)
    {
        case RED: r = rgba(255,0,0); break;
        case DARKRED: r = rgba(127,0,0); break;
        case LIGHTRED: r = rgba(255,132,132); break;

        case GREEN: r = rgba(0,255,0); break;
        case DARKGREEN: r = rgba(38,127,0); break;
        case LIGHTGREEN: r = rgba(182,255,0); break;
        
        case BLUE: r = rgba(0,0,255); break;
        case DARKBLUE: r = rgba(0,19,127); break;
        case LIGHTBLUE: r = rgba(173,216,230); break;
        case CYAN: r = rgba(0,255,255); break;
        
        case YELLOW: r = rgba(255,216,0); break;
        case ORANGE: r = rgba(255,106,0); break;
        
        case VIOLET: r = rgba(255,0,255); break;
        
        case WHITE: r = rgba(255,0,0); break;
        case BLACK: r = rgba(0,0,0); break;
        
        case DARKGRAY: r = rgba(128,128,128); break;
        case GRAY: r = rgba(28,28,28); break;
        case LIGHTGRAY: r = rgba(228,228,228); break;
        
    };
    Core = toMech(r);
}
void Color::setColorHSV(float h,float s,float v)
{
    setColor(Color::hsv(h,s,v));
}
void Color::setColorMechanical(float r,float g,float b,float a)
{
    setColor(Color::mech(r,g,b,a));
}

//GET
Color::rgba Color::getColor()
{
    return toRGBA(Core);
}
Color::hsv Color::getColorHSV()
{
   return toHSV(Core); 
}
Color::mech Color::getColorMechanical()
{
    return Core;
}