#include <iostream>
#include "template.h"

// C++98

int main() {
    std::cout << Clazz<int>::echo(5) << '\n';
    std::cout << Clazz<std::string>::echo("Hello, world!\n");
}
