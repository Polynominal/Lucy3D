#include "Controll\Safety\Lucia.h"
static Lucia::Function* Current;
static std::function<void(Lucia::Function* fn)> Critical = [](Lucia::Function* fn){};
void Lucia::registerSignals(Function* fn)
{
    Current = fn;
    Critical = RenderCriticalErrors;

    signal(SIGABRT,[](int id)
    {
        Current->OnAbort();
        Current->critical = true;
    });
    signal(SIGILL,[](int id)
    {
        Current->OnInvalid();
        Current->critical = true;

        Error err = Error();
        err.data = "[!!] SIGILL: " + std::to_string(id) + " [!!]";
        err.id = id;
        Current->Errors.push_back(err);
    });
    signal(SIGINT,[](int id)
    {
        std::cout << "SIG INT RECIEVED" << std::endl;
        Current->OnInteraction();
        Current->critical = true;
    });
    signal(SIGSEGV,[](int id)
    {
        std::cout << "SIG SEV RECIEVED" << std::endl;
        Current->OnSegment();
        Current->critical = true;

        Error err = Error();
        err.data = "[!!!] SIGSEV: " + std::to_string(id) + " [!!!]";
        err.id = id;
        Current->Errors.push_back(err);

        Critical(Current);
    });
    signal(SIGTERM,[](int id)
    {
        Current->OnTermination();
        Current->critical = true;
    });
}
Lucia::Function* Lucia::newFunction(std::string name,std::function<void()> fn)
{
    Function* nFn = new Function(name);
    nFn->Core = fn;
    return nFn;
}
void Lucia::reportException(Function* fn) // passing by value is ok
{
    try {
        if (fn->excPtr) {
            std::rethrow_exception(fn->excPtr);
        }
    } catch(const std::exception& e)
    {
        Error err = Error();
        err.data = "[!] : " + std::string(e.what()) + " [!]";
        err.id = 0;
        fn->Errors.push_back(err);
        fn->silent = true;
    }
}
bool Lucia::handleException(Function* user)
{
    Threads Thread;
    Thread.exec([this,user](int,std::atomic<bool> &Running)
    {
        std::lock_guard<std::mutex> lock(mutex);
        registerSignals(user);
        try
        {
            user->OnStart();
            user->Core();
            user->OnFinish();
        }
        catch(...)
        {
            user->excPtr = std::current_exception();
        }
        reportException(user);
        Running = false;
    });
}
void Lucia::presentErrors(Function* fn)
{
    if (fn->critical or fn->silent)
    {
        std::cout << "SUPER TERRIBLE HACKS WOW" << std::endl;
    }
    while(true){};
    if (fn->critical){RenderCriticalErrors(fn);};
    if (fn->silent){RenderCriticalErrors(fn);};
}
