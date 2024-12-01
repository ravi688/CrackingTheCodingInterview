#include <iostream>
#include <cmath> // for std::signbit

typedef float (*UnivariateFunction)(float);
static constexpr float dx = 0.00003f;

static float derivative(UnivariateFunction f, float x) noexcept
{
	return (f(x + dx) - f(x)) / dx;
}

static float gradientDescend(UnivariateFunction f, float start, float step) noexcept
{
	float x = start;
	int dir = 0;
	int prevDir = 0;
	while(true)
	{
		float slope = derivative(f, x);
		dir = (slope > 0) ? 1 : -1;
		x -= dir * step;
		if((prevDir * dir) < 0)
			break;
		prevDir = dir;
	}
	return x;
}

static float function(float x) noexcept
{
	return x * x * x + 4 * (x - 2) * (x - 2) + x - 5;
}

int main()
{
	float x = gradientDescend(function, 5.0f, 0.01f);
	std::cout << "Minima found: " << x << std::endl;
	return 0;
};
