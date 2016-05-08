#ifndef MIKUS_LUCIA_GLWRAPPER_H
#define MIKUS_LUCIA_GLWRAPPER_H

#include <Graphics/Text/Text.h>
#include <Graphics/Text/Instance.h>
#include <Graphics/Primitives/Quad.h>
#include <Window/Window.h>

void Lucia_GLWrapper(Lucia* lucy,Windower::Window* w)
{
    lucy->RenderCriticalErrors = [w](Lucia::Function* fn)
    {
        Graphics::Image Img("assets/Internal/Lucia/Lucia.png");
        Img.generate();

        Graphics::Text text("assets/Internal/Lucia/Libel.ttf");
        text.generate();
        auto Ortho = glm::ortho(0.0f, 800.0f,0.0f,600.0f, -0.01f, 100.0f);
        text.setProjection(&Ortho);
        Graphics::Text::Instance inst(&text);
        Graphics::Text::Render Print(&inst);
        Print.moveTo(0.0f,1.8f,0.1f);
//
        Print.generate();



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

                                break;
                            }
                            case ( SDLK_RETURN ):
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
            Print.print("CRITICAL ERROR AT: \n" + fn->name);
            Print.post();


        }
    };
}

#endif // MIKUS_LUCIA_GLWRAPPER_H
