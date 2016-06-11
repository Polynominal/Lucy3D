#ifndef MIKUS_LOGISTICS_H_INCLUDED
#define MIKUS_LOGISTICS_H_INCLUDED
namespace Utils
{
    namespace Logic
    {
        template<typename T>
        T lessThanZeroIsDefault(T value,T def)
        {
            if (value < 0){return def;};
            return value;
        };
    };
};
#endif // MIKUS_LOGISTICS_H_INCLUDED
