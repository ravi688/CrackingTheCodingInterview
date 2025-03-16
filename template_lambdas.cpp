#include <span>
#include <vector>

void divide_fast(std::span<int> i, int d) {
    auto f = [&i]<int divisor>() {
      for (auto& value : i) {
        value /= divisor;
      }
    };
    if(d == 2) {
        return f.template operator()<2>();
    }
    if(d == 3) {
        return f.template operator()<3>();
    }
    if(d == 4) {
        return f.template operator()<4>();
    }
    if(d == 5) {
        return f.template operator()<5>();
    }
    if(d == 6) {
        return f.template operator()<6>();
    }

    for (auto& value : i) {
        value /= d;
    }
}

int main()
{
	std::vector<int> values { 1, 2, 3, 4, 5, 6, 7 };
	std::span<int> mySpan(values);
	divide_fast(mySpan, 3);
}
