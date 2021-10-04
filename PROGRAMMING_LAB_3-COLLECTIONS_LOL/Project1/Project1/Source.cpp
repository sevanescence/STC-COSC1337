#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <sstream>

template <typename ... _Fmt_Args>
void throw_out_of_range_fmt(const char *fmt, _Fmt_Args ... args) {
	char err[2048];
	sprintf_s<2048>(err, fmt, args...);
	throw std::out_of_range(err);
}

class Movie {
public:
	Movie() {}
	Movie(std::string _title, std::string _genre)
		: title(_title), genre(_genre) {}
	Movie(std::string _title, std::string _genre, uint16_t _runtime, uint16_t _rating)
		: title(_title), genre(_genre), runtime(_runtime) {
		setRating(_rating);
	}

	void setRating(const uint16_t _rating) {
		if (_rating >= 0 && _rating <= 100) {
			_M_rating = _rating;
		}
		else {
			throw_out_of_range_fmt("Value of parameter uint16_t _rating (which is %hu)"
				" must be between 0 and 100.", _rating);
		}
	}

	uint16_t getRating() const {
		return _M_rating;
	}

	std::string title, genre;
	uint16_t runtime = 0;
private:
	uint16_t _M_rating = 0;
};

template <typename _Tp, size_t _Size>
struct _Selection_Buffer {
	_Selection_Buffer(const _Tp *_begin, size_t _len) : _M_len(_len) {
		//memcpy_s(_M_buffer, sizeof(_Tp) * _Size, _begin, sizeof(_Tp) * _len);
		for (size_t i = 0; i < _M_len; ++i) {
			_M_buffer[i] = _begin[i];
		}
	}

	size_t size() {
		return _M_len;
	}

	// pass the count, not the byte size, as _size
	void copy(_Tp *_dest, size_t _size) {
		if (_size > _M_len) {
			throw_out_of_range_fmt("Parameter _size (which is %zu) is greater than"
				" the maximum size of the buffer.", _size);
		}
		for (size_t i = 0; i < _size; ++i) {
			_dest[i] = _M_buffer[i];
		}
	}

	void copy(_Tp *_dest) {
		copy(_dest, _M_len);
	}

	void copy(_Tp *_dest, size_t *_size_buf) {
		copy(_dest, _M_len);
		*_size_buf = _M_len;
	}

private:
	_Tp _M_buffer[_Size];
	size_t _M_len = _Size;
};

template <typename _Tp, size_t _Size>
class Database {
public:
	Database() {}

	void add(const _Tp &_r) {
		if (_M_len < _Size) {
			_M_array[_M_len++] = _r;
		}
		else {
			throw_out_of_range_fmt("Attempt to append to Database<%s, %zu> failed;"
				" database length has reached its limit.", typeid(_Tp).name(), _Size);
		}
	}

	size_t size() const {
		return _M_len;
	}

	_Tp &at(size_t _pos) {
		return _M_array[_pos];
	}

	/// <summary>
	/// Matches the position of the first item in
	/// the database which satisfies the filter.
	/// </summary>
	template <typename _Query_t>
	size_t find(bool(*filter)(const _Tp &, const _Query_t), _Query_t match) {
		// based linear search O(n) ratio who needs data structures
		for (size_t i = 0; i < _M_len; ++i) {
			if (filter(_M_array[i], match))
				return i;
		}
		return SEARCH_FAILURE;
	}

	template <typename _Select_t>
	_Selection_Buffer<_Select_t, _Size> select(_Select_t (*get)(const _Tp &)) const {
		_Select_t buffer[_Size]; size_t ptr = 0;
		for (const _Tp *it = _M_array; it != _M_array + _M_len; ++it) {
			buffer[ptr++] = get(*it);
		}
		_Selection_Buffer<_Select_t, _Size> _sbuf(buffer, ptr);

		return _sbuf;
	}

	static constexpr size_t SEARCH_FAILURE = std::numeric_limits<std::size_t>::max();
private:
	_Tp _M_array[_Size];
	size_t _M_len = 0;
};

std::string getTitle(const Movie &movie) {
	return movie.title;
}

uint16_t getMovieRating(const Movie &movie) {
	return movie.getRating();
}

class Event {
public:
	Event(std::string _name, void (*_callback)(std::string)) 
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

namespace Application {
	bool ShouldExit = false;
	EventHandler AppEventHandler;

	constexpr size_t DB_LEN = 100U;
	Database<Movie, DB_LEN> MovieDatabase;
	
	constexpr size_t ATTR_C = 4;
	const char movie_attrs[ATTR_C][20] = { "title", "genre", "runtime", "rating" };

	bool strInAttrs(std::string _str) {
		for (auto i = 0; i < ATTR_C; ++i) {
			if (_str == movie_attrs[i])
				return true;
		}
		return false;
	}

	// getters
	std::string GetTitle(const Movie &movie) {
		return movie.title;
	}

	// filters
	bool TitleFilter(const Movie &movie, const std::string title) {
		return movie.title == title;
	}

}
char const *border_max = "===============================================";

void ExitEventCallback(std::string _) {
	Application::ShouldExit = true;
}
void HelpEventCallback(std::string);
void SizeGetEventCallback(std::string);
void SelectEventCallback(std::string);
void FindEventCallback(std::string);
void RowGetEventCallback(std::string);
void AddMovieEventCallback(std::string);

int main() {
	Application::AppEventHandler.registerEvent(Event("exit", ExitEventCallback));
	Application::AppEventHandler.registerEvent(Event("help", HelpEventCallback));
	Application::AppEventHandler.registerEvent(Event("size", SizeGetEventCallback));
	Application::AppEventHandler.registerEvent(Event("select", SelectEventCallback));
	Application::AppEventHandler.registerEvent(Event("find", FindEventCallback));
	Application::AppEventHandler.registerEvent(Event("get", RowGetEventCallback));
	Application::AppEventHandler.registerEvent(Event("add", AddMovieEventCallback));

	Application::MovieDatabase.add({ "The Matrix", "Sci-fi", 180, 98 });
	Application::MovieDatabase.add({ "Your Name (Kimi no na wa.)", "Romance", 112, 99 });
	Application::MovieDatabase.add({ "Weathering with You (Tenki no ko)", "Romance", 114, 99 });

	std::string arg;
	while (! Application::ShouldExit) {
		system("CLS");
		printf("%.*s\n", 20, border_max);

		puts("Welcome to MakoDB!");
		puts("v0.0.1");
		puts("db length: 100");
		printf("%.*s\n", 20, border_max);

		printf("db size: %zu\n\n", Application::MovieDatabase.size());
		puts("Enter a command, or help for a list of commands.\n");
		
		std::cout << "~$ ";
		std::getline(std::cin, arg);
		bool success = Application::AppEventHandler.fireEvents(arg);
		if (!success) {
			std::cout << "command \"" << arg << "\" not found.\n";
			system("PAUSE");
		}
	}

//	movieDatabase.add({ "The Matrix", "Sci-fi", 180, 98 });
//	movieDatabase.add({ "Your Name (Kimi no na wa.)", "Romance", 112, 99 });
//	movieDatabase.add({ "Weathering with You (Tenki no ko)", "Romance", 114, 99 });
//	
//	std::string movieTitles[3];
//	movieDatabase.select(getTitle).copy(movieTitles);
//	for (int i = 0; i < 3; ++i) {
//		std::cout << movieTitles[i] << '\n';
//	}

}

void HelpEventCallback(std::string _) {
	printf("%.*s\n", 48, border_max);
	puts("help - displays a list of commands. Enter \"help\" for more info.");
	puts("exit - exit the program.");
	puts("size - gets the size of the database -- that is, the number\n  of items currently stored.");
	puts("select <attr> - returns column of <attr> from database, if the\n  function exists.");
	puts("find <attr> <value> - finds first match of row by <value> of <attr> in movie.");
	puts("get <pos> - gets the queried row at <pos>.");
	puts("add - open movie addition prompt. (what, did you expect me to handle arguments for this?)");
	puts("\n*btw these are case-sensitive lol");
	printf("%.*s\n", 48, border_max);
	system("PAUSE");
}

void SizeGetEventCallback(std::string _) {
	std::cout << Application::MovieDatabase.size() << '\n';
	system("PAUSE");
}

void SelectEventCallback(std::string arg) {
	size_t first_delim = arg.find_first_of(' ');
	if (first_delim == std::string::npos) {
		puts("error: attr not provided.");
		system("PAUSE");
		return;
	}

	std::stringstream ss(arg);
	std::string label; ss >> label;
	std::string attr; ss >> attr;

	bool found = Application::strInAttrs(attr);
	if (!found) {
		std::cout << "error: attr \"" << attr << "\" not found in row.\n";
		std::cout << "The correct attrs are: ";
		for (auto i = 0; i < Application::ATTR_C; ++i) {
			std::cout << Application::movie_attrs[i] << ' ';
		} std::cout << std::endl;

		system("PAUSE");
		return;
	}

	// hardcode go brrrr
	// I sort of realized (way too late) that i implemented
	// absolutely no way of evaluating different attribute
	// types at runtime, and since this database isnt
	// type-ambiguous, this is really my only choice. sucks
	// to suck, honestly. ¯\_(ツ)_/¯
	if (attr == "title") {
		std::string movieTitles[Application::DB_LEN]; size_t fetch_size;
		Application::MovieDatabase.select(Application::GetTitle).copy(movieTitles, &fetch_size);
		std::cout << "Movie titles\n";
		for (auto i = 0ULL; i < fetch_size; ++i) {
			std::cout << movieTitles[i] << '\n';
		}
	}
	else {
		std::cout << "I wasnt told I had to get any other columns. I really need to turn this in xd\n";
	}

	system("PAUSE");
}

void FindEventCallback(std::string arg) {
	size_t first_delim = arg.find_first_of(' ');
	if (first_delim == std::string::npos) {
		puts("error: attr not provided.");
		system("PAUSE");
		return;
	}

	std::stringstream ss(arg);
	std::string label; ss >> label;
	std::string attr; ss >> attr; ss.ignore(1);
	std::string value; std::getline(ss, value);
	std::cout << value << '\n';

	bool found = Application::strInAttrs(attr);
	if (!found) {
		std::cout << "error: attr \"" << attr << "\" not found in row.\n";
		std::cout << "The correct attrs are: ";
		for (auto i = 0; i < Application::ATTR_C; ++i) {
			std::cout << Application::movie_attrs[i] << ' ';
		} std::cout << std::endl;

		system("PAUSE");
		return;
	}

	// lol ill put this in a map later or smth
	if (attr == "title") {
		size_t row_pos = Application::MovieDatabase.find(Application::TitleFilter, value);
		if (row_pos == Application::MovieDatabase.SEARCH_FAILURE) {
			std::cout << "Movie with title \"" << value << "\" could not be found.\n";
		}
		else {
			std::cout << "Movie with title \"" << value << "\" found at row " << row_pos << '\n';
		}
	}
	else {
		std::cout << ":) todo\n";
	}

	system("PAUSE");
}

void RowGetEventCallback(std::string arg) {
	size_t first_delim = arg.find_first_of(' ');
	if (first_delim == std::string::npos) {
		puts("error: pos not provided.");
		system("PAUSE");
		return;
	}

	std::stringstream ss(arg);
	std::string label; ss >> label;
	size_t pos; ss >> pos;

	if (pos+1 > Application::MovieDatabase.size()) {
		std::cout << "error: database size is less than " << pos+1
			<< " (" << Application::MovieDatabase.size() << ")\n";
	}
	else {
		Movie movie = Application::MovieDatabase.at(pos);
		std::cout << "Title of movie at pos " << pos << ": " << movie.title << '\n';
	}

	system("PAUSE");
}

void AddMovieEventCallback(std::string _) {
	Movie movie;
	
	std::cout << "Enter movie title: ";
	std::getline(std::cin, movie.title);
	std::cout << "Enter movie genre: ";
	std::getline(std::cin, movie.genre);
	std::cout << "Enter movie runtime: ";
	std::cin >> movie.runtime;
	std::cout << "Enter movie rating (0-100. it will throw if you do not do this. deal with it.): ";
	int r; std::cin >> r; movie.setRating(r); 
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	Application::MovieDatabase.add(movie);
	std::cout << "Movie \"" << movie.title << "\" added to database.\n";
	system("PAUSE");
}
