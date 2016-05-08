#ifndef QUEQUE_H
#define QUEQUE_H
#include <mutex>
#include <atomic>
#include <queue>
#include <iostream>

template<typename T>
class queque
{
    public:
        queque(){};
        void send(T const& data)
        {
            std::unique_lock<std::mutex> Lock(mutex);
            std::cout << "Sending " << data << std::endl;
            Que.push(data);
            Lock.unlock();
        };
        bool get(T& data){
            std::unique_lock<std::mutex> Lock(mutex);
            if (!Que.empty())
            {
                data = Que.front();
                Que.pop();
                return true;
            }else{return false;};
        };
        bool isEmpty(){
            return Que.empty();
        };

        virtual ~queque(){
            std::queue<T> empty;
            std::swap(Que,empty);
        };
    protected:
    private:
        std::mutex mutex;
        std::queue<T> Que;
};

#endif // QUEQUE_H
