#include <iostream>
#include <cstdio>
#include <tuple>
#include <bits/functexcept.h>

template <typename _Tp, size_t _Size> struct array {
    private:
    _Tp _M_array[_Size];
    
    public:
    array() {}

    template <typename ... _Ts>
    array(_Ts ... __elements) {
        if (sizeof...(_Ts) > _Size) {
            static const char *fmt = "array::array: __elements (which is %zu) > _Size (which is %zu)";
            char msg[256]; sprintf(msg, fmt, sizeof...(_Ts), _Size);
            std::__throw_length_error(msg);
        }

        _Tp __array[sizeof...(_Ts)] = { __elements... };
        memcpy(_M_array, __array, sizeof...(_Ts) * sizeof(_Tp));
    }

    _Tp &at(size_t idx) {
        if (idx >= _Size)
            std::__throw_out_of_range_fmt("array::at: idx (which is %zu)"
                                " >= _Size (which is %zu)", idx, _Size);
        
        return _M_array[idx];
    }
};

int main() {
    array<int, 3> __array{5, 2, 3};

    std::cout << __array.at(0) << '\n'; // prints 5
    std::cout << __array.at(3) << '\n'; // throws error
}
