#include <iostream>
#include <string>
#include <list>

// C++11+

// create list node and list iterator

template <typename ... _Ts> constexpr std::size_t length = sizeof...(_Ts);

template <typename _Tp> struct _list_node {
    typedef _list_node<_Tp>    _Self;
    typedef _Tp                value_type;
    typedef _Tp*               pointer;
    typedef _Tp&               reference;

    _Self *next = static_cast<_Self*>(nullptr);
    _Self *prev = static_cast<_Self*>(nullptr);
    _Tp value;
    
    _list_node(_Tp _value) : value{_value} {}
    template <typename ... _Tpa> 
    _list_node(_Tp _v, _Tpa ... __next_args) : value{_v}, next{new _Self(this, __next_args...)} {}

    _list_node(_Self *_prev, _Tp _value) : value{_value}, prev{_prev} {}
    template <typename ... _Tpa>
    _list_node(_Self *_p, _Tp _v, _Tpa ... __next_args) : value{_v}, prev{_p}, next{new _Self(this, __next_args...)} {}
};

template <typename _Tp> struct _list_iterator {
    typedef _list_iterator<_Tp>    _Self;
    typedef _list_node<_Tp>        _Node;

    typedef _Tp                    value_type;
    typedef _Tp*                   pointer;
    typedef _Tp&                   reference;

    _Node *node;

    _list_iterator(_Node *_node) : node(_node) {}

    _Node &operator*() const {
        return *node;
    }

    _Node *operator->() const {
        return node;
    }

    _Self &operator++() {
        node = node->next;
        return *this;
    }

    _Self operator++(int) {
        _Self tmp = *this;
        node = node->next;
        return tmp;
    }

    _Self &operator--() {
        node = node->prev;
        return *this;
    }

    _Self operator--(int) {
        _Self tmp = *this;
        node = node->prev;
        return tmp;
    }

    bool operator==(const _Self &__x) const {
        return node == __x.node;
    }

    bool operator!=(const _Self &__x) const {
        return node != __x.node; 
    }

    _Self end() {
        static const _Self _end(static_cast<_Node*>(nullptr));
        return _end;
    }
};

template <typename _Tp> class linked_list {
    public:
        typedef _list_node<_Tp>   _Node;
        typedef _Tp               value_type;
        typedef _Tp*              pointer;
        typedef _Tp&              reference;

        linked_list() : head(static_cast<_Node*>(nullptr)) {}

        template <typename ... _Tpa>
        linked_list(_Tpa ... __node_args) : head(new _Node(__node_args...)) {}

        // TODO write member functions
    private:
        _Node *head;
};

int main() {
    _list_node<int> node {5, 2, 3, 7, 8};
    
    _list_iterator<int> it(&node);
    for (; it != it.end(); ++it) {
        std::cout << it->value << ' ' << it->next << '\n';
    }

    std::cout << sizeof(_list_iterator<char>::value_type) << '\n';
}
