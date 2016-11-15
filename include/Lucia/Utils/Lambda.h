#ifndef MIKUS_LUCIA_UTILS_LAMBDA_H
#define MIKUS_LUCIA_UTILS_LAMBDA_H
#include <functional>

namespace Lucia{
    namespace Utils{
        namespace Lambda{
            
            template <typename R, typename... Args>
            inline void extend(std::function<R(Args...)>& original,std::function<R(Args...)> additional)
            {
                auto old = original;
                original = [old,additional](Args... args)
                {
                    old(args...);
                    return additional(args...);
                };
            };
            //NOT SUITED FOR VOID!!
            template <typename R, typename... Args>
            inline void extendWithReturn(std::function<R(Args...)>& original,std::function<R(R,Args...)> additional)
            {
                auto old = original;
                original = [old,additional](Args... args)
                {
                    return additional(old(args...),args...);
                };
            };
            template <typename R, typename... Args>
            inline void extendAndOverwrite(std::function<R(Args...)>& original,std::function<R(Args...)> additional,std::function<void()> extension)
            {
                original = [additional](Args... args)
                {
                    auto ret = additional(args...);
                    additional();
                    return ret;
                };
            };  
        }
    }
}
#endif