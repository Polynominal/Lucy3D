#include "Lucia\Controll\Safety\Handler.h"
#include <iostream>
namespace Lucia
{
namespace Controll
{
    namespace Safety
    {
        std::function<void(std::string data)> OnFatalCrash = [](std::string data){std::cout << "CRASH DETECTED!" << std::endl;};
    }
}
}
