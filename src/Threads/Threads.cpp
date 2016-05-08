#include "Threads/Threads.h"
Threads::Threads()
{
    //ctor
}
int Threads::exec(std::function<void(int, std::atomic<bool> &Exit)> fn)
{
    int id = children.size();
    std::shared_ptr<Thread> n(new Thread(fn,id));
    children.insert(std::pair<int, std::shared_ptr<Thread>>(id,n));
    return id;
};
std::shared_ptr<Threads::Thread> Threads::find(int id)
{
    return children.find(id)->second;
};
bool Threads::shutdown()
{
    for(auto const &v : children) {
        v.second->abort();
    };
    return true;
};
Threads::~Threads()
{
    this->shutdown();
};
Threads::Thread::Thread(std::function<void(int,std::atomic<bool> &Exit)>fn,int id)
{
    function = std::make_shared<std::function<void(int,std::atomic<bool> &Exit)>>(fn);
    thread = std::thread(&Threads::Thread::Thread::main,this,id);

};
int Threads::Thread::main(int id)
{
    std::function<void(int,std::atomic<bool> &Exit)>fn = *function;
    fn(id,active);
    return 0;
}
void Threads::Thread::abort()
{
    // doesn't work for detached threads use own terminator!
    active.store(false);
    if (thread.joinable())
    {
        thread.join();
    }
}
void Threads::Thread::restart()
{
    this->abort();
    active.store(true);
    thread = std::thread(&Threads::Thread::Thread::main,this,id);
}
Threads::Thread::~Thread()
{
    this->abort();
}
