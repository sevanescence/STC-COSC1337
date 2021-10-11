#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

#include "AppContext.h"


/* ~~~ AMAZING CODE ~~~ */
// Should this all be in a namespace, and perhaps even in another header?
// Maybe. LOL

void ExitEventCallback(std::string) {
	Application::ShouldExit = true;
}
void HelpEventCallback(std::string);
void SizeGetEventCallback(std::string);
void SelectEventCallback(std::string);
void FindEventCallback(std::string);
void RowGetEventCallback(std::string);
void AddMovieEventCallback(std::string);

char const *border_max = "===============================================";
constexpr size_t ATTR_C = 4;
const char movie_attrs[ATTR_C][20] = { "title", "genre", "runtime", "rating" };
bool strInAttrs(std::string _str) {
	for (auto i = 0; i < ATTR_C; ++i) {
		if (_str == movie_attrs[i])
			return true;
	}
	return false;
}
class MovieAttribute {
public:
	enum _attr {
		TITLE = 0,
		GENRE,
		RUNTIME,
		RATING
	};
};

bool filterByMovieTitle(Movie &);
bool filterByMovieGenre(Movie &);
bool filterByMovieRuntime(Movie &);
bool filterByMovieRating(Movie &);

bool findByMovieTitle(const Movie &item, const Movie &query);
bool findByMovieGenre(const Movie &item, const Movie &query);
bool findByMovieRuntime(const Movie &item, const Movie &query);
bool findByMovieRating(const Movie &item, const Movie &query);

std::unordered_map<std::string, bool(*)(Movie &)> selectCallbacks{
	{ movie_attrs[MovieAttribute::TITLE],   filterByMovieTitle },
	{ movie_attrs[MovieAttribute::GENRE],   filterByMovieGenre },
	{ movie_attrs[MovieAttribute::RUNTIME], filterByMovieRuntime },
	{ movie_attrs[MovieAttribute::RATING],  filterByMovieRating }
};
std::unordered_map<std::string, bool(*)(const Movie &, const Movie &)> findCallbacks{
	{ movie_attrs[MovieAttribute::TITLE],   findByMovieTitle },
	{ movie_attrs[MovieAttribute::GENRE],   findByMovieGenre },
	{ movie_attrs[MovieAttribute::RUNTIME], findByMovieRuntime },
	{ movie_attrs[MovieAttribute::RATING],  findByMovieRating }
};

/* ~~~ END OF AMAZING CODE ~~~ */


// Note to professor: I'm not writing a const iterator
// TODO write const_iterator for _db_list
int main() {
	Application::AppEventHandler.registerEvent(Event("exit", ExitEventCallback));
	Application::AppEventHandler.registerEvent(Event("help", HelpEventCallback));
	Application::AppEventHandler.registerEvent(Event("size", SizeGetEventCallback));
	Application::AppEventHandler.registerEvent(Event("select", SelectEventCallback));
	Application::AppEventHandler.registerEvent(Event("find", FindEventCallback));
	Application::AppEventHandler.registerEvent(Event("get", RowGetEventCallback));
	Application::AppEventHandler.registerEvent(Event("add", AddMovieEventCallback));

	std::string arg;
	while (!Application::ShouldExit) {
		system("CLS");
		printf("%.*s\n", 20, border_max);

		puts("Welcome to MakoDB!");
		puts("v0.0.1");
		printf("Database size: %zu\n", Application::MovieDatabase.data().size());
		printf("%.*s\n", 20, border_max);

		puts("Enter a command, or help for a list of commands.\n");

		std::cout << "~$ ";
		std::getline(std::cin, arg);
		bool success = Application::AppEventHandler.fireEvents(arg);
		if (!success) {
			std::cout << "command \"" << arg << "\" not found.\n";
			system("PAUSE");
		}
	}
}

void HelpEventCallback(std::string) {
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

void SizeGetEventCallback(std::string) {
	std::cout << Application::MovieDatabase.data().size() << '\n';
	system("PAUSE");
}

void SelectEventCallback(std::string arg) {
	/*size_t first_delim = arg.find_first_of(' ');
	if (first_delim == std::string::npos) {
		puts("error: attr not provided.");
		system("PAUSE");
		return;
	}

	std::stringstream ss(arg);
	std::string label; ss >> label;
	std::string attr; ss >> attr;

	bool found = strInAttrs(attr);
	if (!found) {
		std::cout << "error: attr \"" << attr << "\" not found in row.\n";
		std::cout << "The correct attrs are: ";
		for (auto i = 0; i < ATTR_C; ++i) {
			std::cout << movie_attrs[i] << ' ';
		} std::cout << std::endl;

		system("PAUSE");
		return;
	}*/

	std::cout << "Feature not yet implemented.\n";

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
	//std::cout << value << '\n';

	bool found = strInAttrs(attr);
	if (!found) {
		std::cout << "error: attr \"" << attr << "\" not found in row.\n";
		std::cout << "The correct attrs are: ";
		for (auto i = 0; i < ATTR_C; ++i) {
			std::cout << movie_attrs[i] << ' ';
		} std::cout << std::endl;

		system("PAUSE");
		return;
	}

	// :(
	Movie movie; size_t ATTR;
	if (attr == movie_attrs[MovieAttribute::TITLE]) {
		movie = { value, "" };
		ATTR = MovieAttribute::TITLE;
	}
	else if (attr == movie_attrs[MovieAttribute::GENRE]) {
		movie = { "", value };
		ATTR = MovieAttribute::GENRE;
	}
	else if (attr == movie_attrs[MovieAttribute::RUNTIME]) {
		movie = { "", "", std::atoi(value.c_str()), 0 };
		ATTR = MovieAttribute::RUNTIME;
	}
	else {
		movie = { "", "", 0, std::atoi(value.c_str()) };
		ATTR = MovieAttribute::RATING;
	}

	auto match = Application::MovieDatabase.find(movie, findCallbacks.at(movie_attrs[ATTR]));
	if (match == Application::MovieDatabase.data().end()) {
		std::cout << "No match\n";
	}
	else {
		std::cout << *match << '\n';
	}

	system("PAUSE");
}

void RowGetEventCallback(std::string) {
	/*size_t first_delim = arg.find_first_of(' ');
	if (first_delim == std::string::npos) {
		puts("error: pos not provided.");
		system("PAUSE");
		return;
	}

	std::stringstream ss(arg);
	std::string label; ss >> label;
	size_t pos; ss >> pos;*/

	std::cout << "This feature is deprecated. Use Database::find() instead.\n";

	system("PAUSE");
}

void AddMovieEventCallback(std::string) {
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







/* ~~~ Now entering: The Forbidden Wastes ~~~ */

bool filterByMovieTitle(Movie &_m) {
	return false; // what does this even mean
}
bool filterByMovieGenre(Movie &) {
	return false; // i should probably revise the select function
}
bool filterByMovieRuntime(Movie &) {
	return false;
}
bool filterByMovieRating(Movie &) {
	return false;
}

bool findByMovieTitle(const Movie &item, const Movie &query) {
	return item.title == query.title;
}
bool findByMovieGenre(const Movie &item, const Movie &query) {
	return item.genre == query.genre;
}
bool findByMovieRuntime(const Movie &item, const Movie &query) {
	return item.runtime == query.runtime;
}
bool findByMovieRating(const Movie &item, const Movie &query) {
	return item.getRating() == query.getRating();
}
