#ifndef MIKUS_GRAPHICS_TEXT_FLAGPROGRESSIVE_H
#define MIKUS_GRAPHICS_TEXT_FLAGPROGRESSIVE_H
#include <Graphics/Text/Text.h>
#include <Graphics/Text/Instance.h>
#include <Graphics/Bases/Moveable.h>
#include <queque.h>

namespace Graphics
{
    class Text::Task: public Graphics::Base::Moveable
    {
        public:
            Task(Graphics::Text::Render*,Text::Present);
            virtual ~Task();

            Task& operator << (const char* b);
            Task& operator << (const float a);
            Task& operator << (const bool a);

            void refresh();
            void finish();
            string process();
            void draw();

            void setSpeed(float s=1);
            void setStart(int i=1); // where to start in the alphabet!

        private:
            Text::Present task;
            bool done = false;
            Render* Renderer = nullptr;

            std::queue<string> que;

            float speed=1.0f;
            float accumulator=0.0f;
            unsigned int textPosition=0;
            std::vector<float> segments;
            unsigned char alphabetStart=65; // start at the end
            unsigned char alphabetFinish=128;
            bool added = false;
    };
}


#endif // MIKUS_GRAPHICS_TEXT_FLAGPROGRESSIVE_H
