#include "Lucia\Utils\Log.h"
using namespace Lucia;
namespace Lucia {
    Log LOG;
}
#ifdef __WIN32
#include <windows.h>
#include <tchar.h>
#include <conio.h>
#include <strsafe.h>

void Log::setConsoleTitle(const char* title)
{
    SetConsoleTitle(title);
};
void Log::setColor(Log::Colors color)
{
    HANDLE Console = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(Console, color);
};


#else

#endif
Log::Log()
{
    //ctor
}
Log::Log(const char* name)
{
    id = name;
    //ctor
}
void Log::addSeverity(int no,const char* name,Log::Colors color,std::function<void()>fn)
{
    severityMap.insert(std::pair<int,Severity>(no,Severity(name,color,fn)));
}
const char* Log::getSeverity()
{
    std::string name = "undefined";
    auto it = severityMap.find(severity);
    if (it != severityMap.end())
    {
        Severity s = it->second;
        name = s.name;
    };
    return name.c_str();
}
void Log::setSeverity(int no)
{
    severity = no;
}
int Log::lookUp(std::string name)
{
    for (auto Severe: severityMap )
    {
        if (Severe.second.name == name){return Severe.first;};
    }
    return 0;
}
void Log::setSeverity(std::string name)
{
    severity = Log::lookUp(name);
}
void Log::clear()
{
    cache.clear();
}
std::string Log::write(const char* filename)
{
    int edition = 0;
    std::string formated = id;
    if (id !="") {formated = "[" + id + "]";};
    std::string fname = std::string(filename) + formated;
    if (!overwrite){
        while (Utils::fileExists((fname + ".txt").c_str()))
        {
            edition = edition + 1;
            fname = filename + formated +  "[" + std::to_string(edition) + "]";
        }
    };
    std::ofstream File;
    File.open(fname + ".txt");
    File << std::accumulate(cache.begin(),cache.end(),std::string(""));
    File.close();
    return fname;
}
void Log::send(int severity_no,std::string msg,bool end_line,bool print)
{
    if (current_line == "")
    {
        current_line = std::string("[") + Utils::getTime() + "]";
        Log::Colors color = Colors::GRAY;
        std::string name = "undefined";
        auto it = severityMap.find(severity_no);
        if (it != severityMap.end())
        {
            Severity sev = it->second;
            name = sev.name;
            color = sev.color;
            last_call = sev.fn;
        };
        *this << Log::WHITE;
        std::cout << current_line;
        *this << color;
        std::cout << "[" + name + "] ";
        if (id != ""){std::cout << "[" + id + "] ";};
        *this << Log::GRAY;
        current_line += "[" + name + "] " + msg;
    }
    else
    {
        current_line += msg;
    };
    if (print)
    {
        std::cout << msg;
    }
    if (end_line)
    {
        *this << Log::GRAY;
        cache.push_back(current_line + "\n\r");
        current_line = "";
        std::cout << std::endl;
        last_call();
        last_call = [](){};
    }
}
void Log::send(int severity_no,const char* msg,bool end,bool p)
{
    this->send(severity_no,std::string(msg),end,p);
}
void Log::send(std::string severity_name,const char* msg,bool end,bool p){
    this->send(Log::lookUp(severity_name),msg,p);
};
void Log::load(int precentage,const char* msg)
{
    int prc = precentage/10;
    std::string message = std::string(" ") + msg;
    std::string flush = "";
    std::string cond_flush = "\b";
    std::string a = "[";
    for (unsigned int i=0; i < message.size(); i++)
    {
        cond_flush += "\b";
    }
    for (unsigned int i=0; i < 10; i++)
    {
        if (i < prc)
        {
            a += full;
            for (unsigned int i=0; i < full.size(); i++)
            {
                flush += "\b";
            }
        }
        else
        {
            a += empty;
            for (unsigned int i=0; i < empty.size(); i++)
            {
                flush += "\b";
            }
        };
    }
    a += "]";
    if (precentage == 100){
        std::cout << a << message << flush << cond_flush << std::flush;
        cache.push_back(a + message + "\n\r");
        std::cout << std::endl;
    }else{
        if (message != " "){cache.push_back(a + message + "\n\r");};
        flush += "\b\b";
        // need about 10 to over write the lines unless
        std::cout << a << message << flush << cond_flush << std::flush;
        lastFlush = cond_flush;
    };
}
void Log::buffer()
{
    std::cout << "\b\b"<< std::flush;
    Utils::sleep(150);
    std::cout << "|" <<"\b"<< std::flush;
    Utils::sleep(150);
    std::cout << "/" <<"\b"<< std::flush;
    Utils::sleep(170);
    std::cout << "-" <<"\b"<< std::flush;
    Utils::sleep(150);
    std::cout << "|" <<"\b"<< std::flush;
    Utils::sleep(150);
    std::cout << "/" <<"\b"<< std::flush;
    Utils::sleep(150);
    std::cout << "-" <<"\b"<< std::flush;
}
const char* Log::dump()
{
    std::string output;
    for (std::string var: cache){
        output += var;
    };
    return output.c_str();
}
void Log::parse(std::string msg,bool end_line,bool print)
{
    if (current_line == "" && !appliedSeverity)
    {
        auto s = lookUp(msg);
        if (s != 0)
        {
            severity = s;
            appliedSeverity = true;
            return;
        };
    };
    if (end_line){appliedSeverity = false;};
    send(severity,msg,end_line,print);
}
Log& Log::operator<<(std::string msg){
    // do not end the line here and wait for unique notifier! so we pass FALSE to send
    parse(msg,false);
    return *this;
};
Log& Log::operator<<(const char* msg)
{

    parse(std::string(msg),false,false);
    if (current_line != "")
    {
        std::cout << msg;
    }
    return *this;
};
Log& Log::operator<<(std::ostream& (*os)(std::ostream&)){
    // end line notifier found pass TRUE for end line
    parse(std::string(""),true,false);
    return *this;
};
Log& Log::operator<<(Colors color){
    // an empty string is passed for init otherwise the color won't register.
    if (current_line == ""){*this << "" << color;} else{
            this->setColor(color);
    }
    return *this;
};
Log::~Log()
{
    //dtor
}
