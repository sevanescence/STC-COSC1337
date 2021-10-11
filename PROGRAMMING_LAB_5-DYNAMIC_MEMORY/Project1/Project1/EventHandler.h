#pragma once
#include <string>
#include <vector>

class Event {
public:
	Event(std::string _name, void(*_callback)(std::string))
		: name(_name), callback(_callback) {}

	std::string name;
	void(*callback)(std::string args);
};

class EventHandler {
public:
	EventHandler() {}
	void registerEvent(const Event &_event) {
		_M_events.push_back(_event);
	}
	bool fireEvents(std::string _str) {
		//std::string _name = _str.substr(0, _str.find(' '));
		size_t _dm_pos = _str.find_first_of(' ');
		std::string _name = _dm_pos == _name.npos ? _str : _str.substr(0, _dm_pos);

		bool found = false;
		for (auto it = _M_events.begin(); it != _M_events.end(); ++it) {
			if (_name == it->name) {
				it->callback(_str);
				found = true;
			}
		}

		return found;
	}
private:
	std::vector<Event> _M_events;
};
