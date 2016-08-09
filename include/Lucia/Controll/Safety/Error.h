#ifndef MIKUS_LUCIA_DAMAGECONTROLL_H
#define MIKUS_LUCIA_DAMAGECONTROLL_H
#include <signal.h>

#include <Lucia/Threads/Threads.h>
#include <Lucia/Threads/queque.h>

#include <Lucia/Utils/Log.h>

#include <exception>
#include <stdexcept>
// ---------------------
// DESIGN QUALIFICATIONS
// ---------------------

// Purpose:
//----------------
// Safety framework for encountering a large host of errors in a
// contained enviroment to prevent crashes.

// Method:
//----------------
// Isolation of interface into different threads for processing.

// Benefits:
//----------------
// Provides fast and reliable error reporting. Not only in log format but in graphical view.
// Gives a safety mechanism for the user to deal with allowing for user intervention.
// Thread protected sensetive data that prevents the use of data from other threads.
//
namespace Lucia
{
    class Error
    {
        public:
            // INTERNAL STRUCTS
            template<typename T>
            class SensitiveData
            {
                std::map<int,T> chunks;

                int lastCheckup=-1;
                SensitiveData(T data)
                {
                    chunks.push_back(chunks);
                };
                int add(T chunk)
                {
                    chunks.insert({chunks.size(),chunk});
                    return chunks.size();
                };
                bool has(int id)
                {
                    auto iter = chunks.find(id);
                    if (iter != chunks.end()){lastCheckup = id;return true;}else{lastCheckup = -1;return false;};
                };
                T get()
                {
                    if (lastCheckup != -1)
                    {
                        return chunks[lastCheckup];
                    };
                };
            };
            struct Report
            {
                std::string data;
                int id;
            };
            struct Function
            {
                Function(std::string Name){name = Name;};
                void setProgress(int percentage){progress = percentage;};

                std::string name;
                int progress;
                bool critical=false;
                bool silent=false;
                std::exception_ptr excPtr;
                void* Data;
                std::vector<Report> Errors;

                std::function<void()> Core = [](){};
                // End User's functions
                std::function<void()> OnStart    = [](){};
                std::function<void()> OnFinish   = [](){};
                std::function<void()> OnRecovery = [](){};

                std::function<void()> OnAbort= [](){};
                std::function<void()> OnInvalid= [](){};
                std::function<void()> OnInteraction= [](){};
                std::function<void()> OnSegment= [](){};
                std::function<void()> OnTermination= [](){};
            };
            // seperate storage of sensitive data.

            //misc
            void reportError(std::string data="Undefined error",bool crash = false);
            Function* newFunction(std::string name,std::function<void()> fn);
            bool handleException(Function* fn);
            void reportException(Function* fn);
            void presentErrors(Function* fn);
            void registerSignals(Function* fn);

            std::shared_ptr<Log>  LOGTHIS();

            // WRAPPERS
            std::function<void(Function* fn)> RenderCriticalErrors;
            std::function<void(Function* fn)> RenderSilentErrors;
        private:
            std::mutex mutex;
            std::vector<std::shared_ptr<Threading::Threads::Thread>> ThreadsUsed;
    };
}

#endif // MIKUS_DAMAGECONTROLL_LUCIA_H
