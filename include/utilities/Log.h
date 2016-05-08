#ifndef LOG_H
#define LOG_H
#include <utilities/utils.h>
#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <type_traits>
#include <typeinfo>
#include <sstream>
#include <fstream>
#include <numeric>
class Log
{
    public:
        enum Colors
        {
            BLACK,
            BLUE,
            GREEN,
            GBLUE,
            RED,
            MAGENTA,
            BORING,
            GRAY,
            DGRAY,
            BBLUE,
            BGREEN,
            CYAN,
            BRED,
            VIOLET,
            YELLOW,
            WHITE,
        };
        Log();
        Log(const char* name);
        virtual ~Log();

        Log& operator<<(Colors color);
        Log& operator<<(std::string msg);
        Log& operator<<(const char* msg);
        Log& operator<<(std::ostream& (*os)(std::ostream&));
        void setConsoleTitle(const char* title);

        template <typename T>
        Log& operator<<(T *msg){
            // couldn't figure out how to convert directly to 0x... so instead used a string stream to do it for me.
            // type-casting to unsigned int didn't work as it caused an the numbers to be slightly off, perhaps do in std::hex?
            std::stringstream smsg;
            smsg << msg;
            this->send(this->severity,smsg.str(),false);
            return *this;
        };
        template <typename T>
        Log& operator<<(T msg){
            static_assert(std::is_fundamental<T>::value,"Bad type! Expected fundamental.");
            this->send(this->severity,std::to_string(msg),false);
            return *this;
        };
        void addSeverity(int no,const char* name,Colors color =Colors::GRAY,std::function<void()>fn=[](){});
        const char* getSeverity();
        void setSeverity(int no);
        void setSeverity(std::string severity_name);

        void setColor(Colors color);

        void send(int severity,std::string msg,bool end_line = true,bool print = true);
        void send(int severity,const char* msg,bool end_line = true,bool print = true);

        void send(std::string severity_name,const char* msg,bool end_line = true,bool print = true);

        void clear();
        void load(int precentage,const char* msg="");
        void buffer();

        void write(const char* filename=("LOG_" + Utils::getCleanDate()).c_str());
        const char* dump();

        // easily accessible cache with output of the messages as you send them via send()
        // useful for in-game command line when you want only draw the latest lines and view the past lines without reading file.
        // has a memory footprint as it has to store each line made, beware of this, I will make a new function to prevent this.
        // note that you may need to flip this if you intend to make a console as vector is a push back device
        std::ostream&(*endl)(std::ostream&);
        bool overwrite=false;
        std::string empty = "-";
        std::string full = "/";
        std::vector<std::string> cache;
    protected:
    private:
        struct Severity{
            std::string name;
            Colors color;
            std::function <void()>fn;
            Severity(const char* tname,Colors col,std::function<void()>tfn) // added empty lambda
                     :name(tname),color(col),fn(tfn){};
        };
        int lookUp(std::string name);
        std::string id="";
        std::string formated = "";
        std::string current_line = "";
        int severity = 0;
        std::map<int,Severity> severityMap;
        std::string filename = std::string("LOG [") + Utils::getDate() + "]";
        std::string lastFlush="";
        std::function<void()>last_call=[](){}; // empty lambda for end of the line execution.
};

extern Log LOG;

#endif // LOG_H
