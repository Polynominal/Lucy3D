#include "Lucia/Graphics/Text/Task.h"
using namespace Lucia;
typedef Graphics::Text::Task Task;
using namespace std;

Task::Task(Graphics::Text::Render* r,Text::Present p)
{
    Renderer = r;
    task = p;
    //ctor
}
Task& Task::operator << (const char* b)
{
    added = true;
    que.push(b);
    return *this;
}
Task& Task::operator << (const float a)
{
    return *this << std::to_string(a).c_str();
};
Task& Task::operator << (const bool a)
{
    if (a)
    {
        return *this << "True";
    }else
    {
        return *this << "False";
    }
};
void Task::finish()
{
    if (que.size() != 1) // if we are not at the top!
    {
        que.pop();
    }
    refresh();
}
void Task::refresh()
{
    textPosition = 0;
    accumulator = 0;
    segments.clear();
    if (que.size() == 0)
    {
        done=true;
    }else
    {
        if (task == Text::Present::MatchOut or task == Text::Present::MatchType)
        {
            for (auto v: que.front())
            {
                segments.push_back(alphabetStart);
            }
        }else
        {
            segments.push_back(alphabetStart);
        }
    }
    added = false;
}
string Task::process()
{
    accumulator = accumulator + speed;
    if (added)
    {
        refresh();
    }
    if (segments.size() == 0)
    {
        for (auto v: que.front())
        {
            segments.push_back(alphabetStart);
        }
    }
    string current = que.front().substr(0,textPosition);
    switch (task)
    {
        // match the characters
        case Text::Present::MatchOut:
        {
            current = que.front();
            string n="";
            bool match = true;
            for (unsigned int i=0;i < segments.size();i++)
            {
                string s = std::to_string((char)current[i]);
                char c =(char)current[i];
                if (c != 32)
                {
                    if ((char)segments[i] != (char)current[i])
                    {
                        if (segments[i] > (int)c)
                        {
                            segments[i] = segments[i] - 0.01f;
                        }else
                        {
                            segments[i] = segments[i] + 0.01f;
                        }
                        match = false;
                    }
                    n = n + (char)segments[i];
                }else
                {
                    n = n + (char)current[i];
                }
            }
            if (match)
            {

            }
            return n;
        }
        // type the characters
        case Text::Present::TypeOut:
        {
            int tpos = accumulator;
            if (que.front().size() == tpos){finish();};
            return que.front().substr(0,tpos);
        }
        // match and then match the next character reseting the old one
        case Text::Present::MatchType:
        {
            string n="";
            bool match = true;
            for (unsigned int i=0;i < segments.size();i++)
            {
                if (segments[i] != (char)current[i])
                {
                    if (alphabetStart > 0)
                    {segments[i] = segments[i] - speed;}else{segments[i] = segments[i] + speed;};
                    match = false;
                }
                if (segments[i] == alphabetFinish && textPosition >= i)
                {
                    segments[i] = alphabetStart;
                }else
                {
                    textPosition++;
                }
                n = n + std::to_string(segments[i]);
            }
            if (match and textPosition == que.front().size())
            {
                finish();
            }
            return n;
        }
        // type and then match the character
        case Text::Present::TypeMatch:
        {
            if (segments[0] == que.front()[textPosition])
            {
                if (textPosition == que.front().size() )
                {
                    finish();

                }else{textPosition++;};

                return que.front().substr(0,textPosition);
            }else
            {
                if (alphabetStart > 0)
                {segments[0] = segments[0] - speed;}else{segments[0] = segments[0] + speed;};
            }
            return que.front().substr(0,textPosition-1) + std::to_string(segments[0]);
        }
    }
    return que.front();
}
void Task::draw()
{
    *Renderer << process().c_str();
}
void Task::setSpeed(float s)
{
    speed = s;
}
void Task::setStart(int i) // where to start in the alphabet!
{
    alphabetStart = i;
}
Task::~Task()
{
    //dtor
}
