#ifndef THREADS_H
#define THREADS_H

#include <functional>
#include <thread>
#include <map>
#include <mutex>
#include <atomic>
#include <vector>
#include <memory>
#include <queue>

class Threads
{
    public:
        Threads();

        int exec(std::function<void(int,std::atomic<bool> &Running)>fn);

        bool shutdown();

        virtual ~Threads();
        class Thread
        {
            public:

                Thread(std::function<void(int,std::atomic<bool> &Running)>fn,int id);

                void restart();
                void abort();

                ~Thread();

            private:
                int main(int id);
                std::thread thread;
                std::shared_ptr<std::function<void(int,std::atomic<bool> &Running)>> function = nullptr;
                int id=0;
                std::atomic<bool> active{true};

        };
        std::shared_ptr<Thread> find(int id);
    protected:

    private:
        Threads* System;
        int Active=0;
        int Max = std::thread::hardware_concurrency();
        std::map<int,std::shared_ptr<Thread>> children;
};

#endif // THREADS_H
