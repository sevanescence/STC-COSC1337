#include <iostream>
#include <string>

constexpr std::streamsize STREAMSIZE_MAX = std::numeric_limits<std::streamsize>::max();

template <typename _Tp>
void fillArray(_Tp *_begin, size_t _size);

template <typename _Tp>
void displayArray(const _Tp *_begin, size_t _size);

int main() {
	size_t len;
	std::string *array = nullptr;

	std::cout << "How many data elements are there? ";
	std::cin >> len;
	puts("");
	std::cin.ignore(STREAMSIZE_MAX, '\n');
	array = new std::string[len];

	fillArray(array, len);
	displayArray(array, len);

	delete[] array;
}

template <typename _Tp>
void fillArray(_Tp *_begin, size_t _size) {
	for (auto i = 0ULL; i < _size; ++i) {
		std::cout << "Enter item " << (i + 1) << ": ";
		std::getline(std::cin, _begin[i]);
	} puts("");
}

template <typename _Tp>
void displayArray(const _Tp *_begin, size_t _size) {
	for (auto it = _begin; it != _begin + _size; ++it) {
		std::cout << "Element #" << (it - _begin + 1) << ": " << *it << '\n';
	}
	system("PAUSE");
}
