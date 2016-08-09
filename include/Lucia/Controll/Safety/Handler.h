#ifndef MIKUS_LUCIA_CONTROLL_SAFETY_HANDLER_H
#define MIKUS_LUCIA_CONTROLL_SAFETY_HANDLER_H
#include <string>
#include <csignal>
#include <functional>
namespace Lucia {
namespace Controll
{
    namespace Safety
    {
        extern std::function<void(std::string data)> OnFatalCrash;
        class Handler
        {
            public:
                void silenceErrors(bool t = true){silentErrors = t;};
                std::function<void(std::string)> OnSilentFailiure = [](std::string data){};
                std::function<void(std::string)> OnFatalFailiure = [](std::string data){};

            protected:
                void sendErrorSignal(std::string data,bool fatal=true)
                {
                    if (fatal)
                    {
                        OnFatalFailiure(data);
                        Controll::Safety::OnFatalCrash(data);

                    }else
                    {
                        OnSilentFailiure(data);
                    };
                };

            private:
                bool silentErrors = false;
        };
    }
}
}


#endif // MIKUS_CONTROLL_SAFETY_HANDLER_H
