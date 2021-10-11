#pragma once
#include <iostream>
#include <string>
#include <stdexcept>
#include <memory>
#include <list>
#include <vector>

template <typename ... _Args>
void throw_out_of_range_fmt(const char *fmt, _Args ... pfargs) {
	char err[2048];
	sprintf_s(err, fmt, pfargs...);
	throw std::out_of_range(err);
}

template <typename _Tp, size_t _Size> struct _db_list_iterator;

// array node
template <typename _Tp, size_t _Size>
struct _db_node {
	typedef _db_node<_Tp, _Size> node_t;

	~_db_node() {
		if (next) delete next;
	}

	void add(const _Tp &_val) {
		// will segfault if the operation is invalid
		// i.e. size is already greater than the array
		// offset maximum
		array[_M_size++] = _val;
	}

	size_t size() {
		return _M_size;
	}

	_Tp array[_Size];
	node_t *next = nullptr;
private:
	size_t _M_size = 0;
};

constexpr size_t _M_cpos_start = 0ULL;

// array list
template <typename _Tp, size_t _Size>
struct _db_list {
	typedef _db_list_iterator<_Tp, _Size> iterator;
	typedef _db_node<_Tp, _Size>          node_t;

	_db_list() {
		head = new node_t();
	}

	~_db_list() {
		delete head;
	}

	size_t nodes() {
		node_t *list = head;
		size_t nodec = 1;
		while ((list = list->next) != nullptr)
			++nodec;
		return nodec;
	}

	// gets the first node with a null next pointer
	node_t *last() {
		node_t *list = head;
		while (list->next) list = list->next;
		return list;
	}

	// returns the first node with an available bucket
	node_t *last_open() {
		node_t *list = head;
		while (list->next && list->size() == _Size) list = list->next;
		return list;
	}

	size_t size() {
		return _M_size;
	}

	void push_back(const _Tp &_val) {
		node_t *list = last_open();
		if (list->size() == _Size && !list->next) {
			list->next = new node_t();
			list = list->next;
		}
		list->add(_val);

		++_M_size;
	}

	iterator begin() {
		// §23.2.1 [container.requirements.general] p6
		if (head->size() == _M_cpos_start) {
			return end();
		}
		else {
			return iterator(head);
		}
	}

	iterator end() {
		return iterator(static_cast<node_t *>(nullptr));
	}

	node_t *head = nullptr;
private:
	size_t _M_size = 0;
};

template <typename _Tp, size_t _Size>
struct _db_list_iterator {
	typedef _db_list_iterator<_Tp, _Size> iterator;
	typedef std::forward_iterator_tag     iterator_category;
	typedef _db_node<_Tp, _Size>          node_t;
	typedef _Tp                           value_type;
	typedef _Tp*                          pointer;
	typedef _Tp&                          reference;

	_db_list_iterator(node_t *_node) : _M_current(_node) {}

	_db_list_iterator(node_t *_node, size_t _deep) : _M_current(_node), _M_times_nested(_deep) {}

	size_t pos() {
		return _M_times_nested * _Size + _M_cpos;
	}

	iterator &operator++() {
		if (_M_cpos + 1 >= _M_current->size()) {
			_M_current = _M_current->next;
			_M_cpos = _M_cpos_start;
			++_M_times_nested;

			// checks that the last used node has been iterated
			if (_M_current && _M_current->size() == _M_cpos) {
				_M_current = static_cast<node_t *>(nullptr);
				return *this;
			}
		}
		else {
			++_M_cpos;
		}
		return *this;
	}

	iterator operator++(int) {
		iterator it = *this;
		++*this;
		return it;
	}

	reference operator*() const {
		return _M_current->array[_M_cpos];
	}

	pointer operator->() {
		return _M_current;
	}

	friend bool operator==(const iterator &a, const iterator &b) {
		return a._M_current == b._M_current;
	}

	friend bool operator!=(const iterator &a, const iterator &b) {
		return a._M_current != b._M_current;
	}

	// mmm yummy arithmetic
	iterator &operator+=(int y) {
		// second condition avoids segfault when trying to access
		// inexistent nodes
		// patch note: initial condition was _M_current != nullptr,
		// however the patch compares the iterator position to
		// the actual size of the current list, rather than comparing
		// nodes.
		for (int x = y; x > 0 && _M_current != nullptr; --x) {
			++*this;
		}
		return *this;
	}

private:
	node_t *_M_current;
	size_t  _M_cpos = 0;
	size_t  _M_times_nested = 0;
};

template <typename _Tp, size_t _Size>
_db_list_iterator<_Tp, _Size> operator+(const _db_list_iterator<_Tp, _Size> &iterator, int y) {
	if (y < 0)
		throw_out_of_range_fmt("operator+(_db_list_iterator<%s, %zu>, int y): y (which is %d)"
			" cannot be less than 0.", typeid(_Tp).name(), _Size, y);
	_db_list_iterator<_Tp, _Size> _it(iterator);
	for (int x = y; x > 0 && _it != nullptr; --x) {
		++_it;
	}
	return _it;
}

class Movie {
public:
	Movie() {
		
	}
	Movie(std::string _title, std::string _genre) : title(_title), genre(_genre) { }
	Movie(std::string _title, std::string _genre, int _runtime, int _rating)
		: title(_title), genre(_genre), runtime(_runtime) {
		setRating(_rating);
	}

	int getRating() const {
		return rating;
	}

	void setRating(int _rating) {
		if (_rating <= 100 && _rating >= 0) {
			rating = _rating;
		}
		else {
			throw_out_of_range_fmt("_rating (which is %d) must be between 0 and 100", _rating);
		}
	}

public:
	std::string title;
	std::string genre;
	int runtime = 0;
private:
	int rating = 0;
};

std::ostream &operator<<(std::ostream &os, const Movie &_m) {
	os << _m.title << '\n' << _m.genre << '\n' << _m.runtime << '\n' << _m.getRating() << '\n';
	return os;
}

template <typename _Tp, size_t _BucketSize, size_t _Multiplier>
class Database {
	typedef _db_node<_Tp, _BucketSize> node_t;
	typedef _db_list<_Tp, _BucketSize> list_t;

public:
	typename list_t::iterator find(const _Tp match_val, bool comp(const _Tp &item, const _Tp &query)) {
		for (auto it = _M_data.begin(); it != _M_data.end(); ++it) {
			if (comp(*it, match_val)) {
				return it;
			}
		}
		return _M_data.end();
	}

	std::vector<_Tp> select(bool predicate(_Tp &filter)) {
		std::vector<_Tp> vec;
		for (auto &i : _M_data) {
			if (predicate(i))
				vec.push_back(i);
		}
		return vec;
	}

	void add(const _Tp &_val) {
		node_t *last_node = _M_data.last();
		_M_data.push_back(_val);
		if (_M_data.last() != last_node) {
			///std::cout << "Node added, current length: " << _M_data.size() << '\n';
			size_t _tmp = _M_data.size() - 1; // 15, 5, 2 : 15 / 5 = 3 + 1  = 4
											  // 35 / 5            = 7 + 1  = 8
											  // 75 / 5            = 15 + 1 = 16
			size_t mult = (_M_data.size() - 1) / _BucketSize;
			///std::cout << "Dynamic allocation multiplier: " << mult << '\n';
			last_node = _M_data.last();
			for (auto i = mult; i > 0; --i) {
				last_node->next = new node_t();
				last_node = last_node->next;
			}
		}
	}

	// returning by reference because doing so by
	// value causes it to go out of scope. ask
	// professor about it, lol
	_db_list<_Tp, _BucketSize> &data() {
		return _M_data;
	}
private:
	_db_list<_Tp, _BucketSize> _M_data;
};
