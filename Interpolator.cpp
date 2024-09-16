#include <iostream>

template<typename T, typename FactorType = float>
struct GetLerpFuncProto
{
	typedef T (*type)(T start, T end, FactorType t);
};

template<typename T, typename FactorType = float>
T Lerp(T a, T b, FactorType t)
{
    return a * (1 - t) + b * t;
}

template<typename T, typename GetLerpFuncProto<T>::type LerpFunc = Lerp<T>>
class Interpolator
{
    public:
    
    T getValue(T a, T b, float value) const noexcept { return LerpFunc(a, b, value); }
};

int main()
{
    Interpolator<float> it;
    float value = it.getValue(0.0f, 5.0f, 0.5f);
    std::cout << value << std::endl;

    return 0;
}