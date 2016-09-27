#include <functional>
namespace Lucia
{
namespace Controll 
{
namespace Base
{
    class KeyboardUser
    {
        public:
            KeyboardUser(){};
            virtual ~KeyboardUser(){};
            virtual bool keyIsDown(std::string key){return keyboard.isDown(key);};
            virtual void keyIsDown(std::initializer_list<std::string> keys,std::function<void(std::string)>fn)
            {
                for (auto v: keys)
                {
                    if (keyIsDown(v)){fn(v);};
                }
            };
        private: 
            Lucia::Controll::Keyboard keyboard = Lucia::Controll::Keyboard();
    };
}
}
}