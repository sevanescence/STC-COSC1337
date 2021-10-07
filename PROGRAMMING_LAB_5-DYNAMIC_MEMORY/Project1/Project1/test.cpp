#include <iostream>

template <typename _Tp, size_t _Size>
struct dynamic {
	typedef dynamic<_Tp, _Size> node_t;

	dynamic(size_t _mult) : mult(_mult), bucket(new _Tp[_Size * mult]) {}
	~dynamic() {
		delete[] bucket;
		delete next;
	}

	size_t mult = 0;
	_Tp *bucket = nullptr;
	node_t *next = nullptr;
};

int main() {

}