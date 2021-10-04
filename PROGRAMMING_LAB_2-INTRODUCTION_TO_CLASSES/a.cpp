#include <iostream>

struct _score_traits {
    _score_traits(const double _value) : _M_score(_value) {}
    double &operator()() { return _M_score; }
    const double &operator()() const { return _M_score; }
    void operator()(const double &_value) { _M_score = _value; }

    protected:
        double _M_score;
};

int main() {
    _score_traits score = 5;
    score = 6;
}
