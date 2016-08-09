#include "Lucia\Controll\Safety\Error.h"
using namespace Lucia;
using namespace Threading;
static Error::Function* Current;
static std::function<void(Error::Function* fn)> Critical = [](Error::Function* fn){};
void Error::registerSignals(Function* fn)
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

        Report err = Report();
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

        Report err = Report();
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
void Error::reportError(std::string data,bool crash)
{
        Current->OnInteraction();
        Report err = Report();
        err.data = data;
        err.id = 0;
        Current->Errors.push_back(err);

        if (crash)
        {
            Current->critical = true;
            Critical(Current);
        }
}
Error::Function* Error::newFunction(std::string name,std::function<void()> fn)
{
    Function* nFn = new Function(name);
    nFn->Core = fn;
    return nFn;
}
void Error::reportException(Function* fn) // passing by value is ok
{
    try {
        if (fn->excPtr) {
            std::rethrow_exception(fn->excPtr);
        }
    } catch(const std::exception& e)
    {
        Report err = Report();
        err.data = "[!] : " + std::string(e.what()) + " [!]";
        err.id = 0;
        fn->Errors.push_back(err);
        fn->silent = true;
    }
}
bool Error::handleException(Function* user)
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
void Error::presentErrors(Function* fn)
{
    if (fn->critical or fn->silent)
    {
        std::cout << "SUPER TERRIBLE HACKS WOW" << std::endl;
    }
    while(true){};
    if (fn->critical){RenderCriticalErrors(fn);};
    if (fn->silent){RenderCriticalErrors(fn);};
}
