#include <iostream>
#include <string>
#include <sstream>
#include <unordered_map>

#include "Crypto.h"
#include "Events.h"

// Sorry for the very nuanced design of this one.
// I got tired of the application part halfway
// through and i dont want to change the exit system.

int main() {
	bool should_exit = false;
	std::string _;
	events::callbacks.at("init")(_);

	while (!should_exit) {
		std::string cmd;
		
		std::cout << "~$ ";
		std::getline(std::cin, cmd);
		std::stringstream ss(cmd);
		std::string label; ss >> label;
		
		try {
			bool(*event)(std::string &) = events::callbacks.at(label);
			should_exit = event(cmd);
		}
		catch (std::exception _ignored) {
			std::cout << "Error: command \"" << label << "\" not found. Type \"help\""
				" for a list of commands.\n";
		}
	}
}
