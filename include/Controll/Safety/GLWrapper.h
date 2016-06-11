#ifndef MIKUS_LUCIA_GLWRAPPER_H
#define MIKUS_LUCIA_GLWRAPPER_H

#include <Graphics/Text/Text.h>
#include <Graphics/Text/Instance.h>
#include <Graphics/Primitives/Quad.h>
#include <Window/Window.h>
#include <utilities/log.h>

void addLine(std::string &s,float fontSize,float width)
{
    for (unsigned int i =0;i < (width/(0.391304f*fontSize) -1);i++)
    {
        s = s + "-";
    };
    s += "\n";
}
void Lucia_GLWrapper(Lucia* lucy,Windower::Window* w)
{
    lucy->RenderCriticalErrors = [w](Lucia::Function* fn)
    {
        float width = 800.0f;
        float height = 600.0f;
        int fontSize =25;
        Graphics::Image Img("assets/Internal/Lucia/Lucia.png");
        Img.generate();

        Graphics::Text text("assets/Internal/Lucia/console.ttf");
        text.generate();
        auto Ortho = glm::ortho(0.0f, width,0.0f,height, 0.01f, 100.0f);
        text.setProjection(&Ortho);

        Graphics::Text::Instance inst(&text);
        inst.setSize(fontSize);
        inst.setWrapWidth(800.0f);

        Graphics::Text::Render Print(&inst);
        Print.moveTo(0.0f,2.0f - ((fontSize/height)),1.2f);
        Print.generate();

        float charHeight = (fontSize/height);
        std::string filename = LOG.write();
        while(w->isAlive())
        {
            SDL_Event event;
            while (SDL_PollEvent(&event))
            {
                switch(event.type)
                {
                case SDL_KEYUP:
                    {
                        switch(event.key.keysym.sym)
                        {
                            case ( SDLK_SPACE ):
                            {
                                // print the log file
                                filename = LOG.write();
                                break;
                            }
                            case ( SDLK_ESCAPE ):
                            {
                                // return
                                exit(-1);
                                break;
                            }
                        }
                        break;
                    }
                }
            }
            w->update();
            glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            glDisable(GL_DEPTH_TEST);
            Print.pre();

            std::string CurrentLine="";

            Print.moveTo(0.0f,2.0f - charHeight,1.2f);
            addLine(CurrentLine,fontSize,width);
            Print.print(CurrentLine);

            CurrentLine = "CRITICAL ERROR\n";
            Print.moveTo(1.0f - ((Print.getWidth(CurrentLine))/width),2.0f - charHeight*2.0f,1.2f);
            Print.print(CurrentLine);
            CurrentLine = "";

            // 1.2 to leave a bit more space.
            Print.moveTo(0.0f,Print.getPosition().y - charHeight*1.2f,1.2f);
            addLine(CurrentLine,fontSize,width);
            Print.print(CurrentLine);

            CurrentLine = "\n#" + fn->name + "\n";
            Print.move(1.0f - ((Print.getWidth(CurrentLine))/width),0.0f,0.0f);
            Print.print(CurrentLine);
            CurrentLine =
            "A fatal error/s has occurred: \n\n";
            for (auto v: fn->Errors)
            {
                CurrentLine += v.data + "\n\n";
            };
            CurrentLine += "\nLog file has been saved to:  "+filename+"\nPress the SPACE key to reprint LOG.\nPress the ESC key to terminate the program.\n";
            Print.moveTo(0.0f,Print.getPosition().y -charHeight*3.0f,1.2f);
            Print.print(CurrentLine);
            Print.post();
        }
    };
}

#endif // MIKUS_LUCIA_GLWRAPPER_H
