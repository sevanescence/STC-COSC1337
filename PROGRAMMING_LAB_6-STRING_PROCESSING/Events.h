#pragma once
#define __EVENTS__
#define __I_DONT_LIKE_DOING_THESE__ "HELP ME"

#include <unordered_map>
#include <iostream>
#include <sstream>
#include <string>

#include "App.h"
#include "Crypto.h"

/// <summary>
/// Example of an event callback. Returns whether
/// the program should exit (don't invoke this function).
/// </summary>
bool event_cb(std::string &);

bool help_cb(std::string &) {
	std::cout << "************************************************\n"
	          << "help - shows this.\n"
	          << "set <str> - reassign the message to str.\n"
	          << "get - prints the current message.\n"
	          << "length - prints length of the current message.\n"
	          << "sanitize - pedantically reformats your message,\n"
	          << "in case you didn't already feel bad enough about\n"
	          << "your ability to write.\n"
	          << "encrypt <N> - prints positive caesar offset of N.\n"
	          << "decrypt <N> - prints negative caesar offset of N.\n"
			  << "exit - closes the program (alternatively, CTRL+C,\n"
			  << "not like I spent time on the event system. No,\n"
			  << "not at all. Go ahead. I don't mind. Really).\n"
			  << "clear - clears the screen.\n"
	          << "************************************************\n";
	return false;
}

bool set_cb(std::string &s) {
	std::stringstream ss(s); ss >> s;
	std::string str; while (ss.good()) {
		std::string buf; ss >> buf;
		str.append(buf).append(1, ' ');
	}
	global_context::message = str;

	return false;
}

bool get_cb(std::string &) {
	std::cout << global_context::message << '\n';
	return false;
}

bool length_cb(std::string &) {
	std::cout << global_context::message.get().size() << '\n';
	return false;
}

bool sanitize_cb(std::string &) {
	global_context::message.sanitize();
	std::cout << "Message sanitized. Use 'get' to see the message.\n";
	return false;
}

bool encrypt_cb(std::string &s) {
	std::stringstream ss(s); ss >> s;
	int offset; ss >> offset;
	std::cout << global_context::message.encrypt(offset) << '\n';
	return false;
}

bool decrypt_cb(std::string &s) {
	std::stringstream ss(s); ss >> s;
	int offset; ss >> offset;
	std::cout << global_context::message.decrypt(offset) << '\n';
	return false;
}

bool exit_cb(std::string &) {
	return true;
}

bool clear_cb(std::string &) {
	std::system("cls");
	return false;
}

bool init_cb(std::string &) {
	std::cout << "Enter a string: ";
	std::string str;
	std::getline(std::cin, str);
	global_context::message.set(str);
	std::cout << "\nAwesome. Type \"help\" for a list of commands.\n";
	return false;
}

namespace events {
	std::unordered_map<std::string, bool(*)(std::string &)> callbacks {
	{ "help", help_cb },
	{ "set", set_cb },
	{ "get", get_cb },
	{ "length", length_cb },
	{ "sanitize", sanitize_cb },
	{ "encrypt", encrypt_cb },
	{ "decrypt", decrypt_cb },
	{ "exit", exit_cb },
	{ "clear", clear_cb },
	{ "init", init_cb }
	};
}
