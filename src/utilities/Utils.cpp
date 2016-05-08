#include "utilities/Utils.h"
namespace Utils
{
    // [WARNING] based on current machine time.
    std::string getDateTime()
    {
        char buffer[80];
        std::strftime(buffer,sizeof(buffer),"%Y-%m-%d:%X",updateTime());
        return std::string(buffer);
    }
    std::string getTime()
    {
        char buffer[80];
        std::strftime(buffer,sizeof(buffer),"%H:%M:%S",updateTime());
        return std::string(buffer);
    }
    std::string getDate()
    {
        char buffer[80];
        std::strftime(buffer,sizeof(buffer),"%d/%m/%y",updateTime());
        return std::string(buffer);
    }
    std::string getCleanDate()
    {
        char buffer[80];
        std::strftime(buffer,sizeof(buffer),"%d_%m_%y",updateTime());
        return std::string(buffer);
    }
    const tm* updateTime()
    {
        std::time_t t = std::time(0);
        return std::localtime(&t);
    }
    bool accumulate(int ticks)
    {
        int accumulator = 0;
        while(accumulator < ticks){accumulator++;};
        return true;
    }
    void sleep(int milliseconds)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
    }
    // [WARNING] may return true if exists but has no access.
    bool fileExists(const char* name)
    {
        std::ifstream file(name);
        return file.good();
    }
}

