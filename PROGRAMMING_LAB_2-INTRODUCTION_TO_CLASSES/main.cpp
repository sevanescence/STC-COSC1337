#include <iostream>
#include <string>
#include <stdio.h>
#include <limits>
#include <stdexcept>

template <typename ... _Any>
void throw_out_of_range_fmt(char const *fmt, _Any ... args) {
    char err[2048];
    sprintf(err, fmt, args...);
    throw std::out_of_range(err);
}

class Movie {
    protected:
        double score;
    public:
        std::string title;
        std::string genre;
        int runtime;

        Movie() : title(), genre(), runtime(0), score(0) {}
        Movie(std::string _title) : title(_title), genre(), runtime(0), score(0) {}
        Movie(std::string _title, std::string _genre, int _runtime, double _score) 
        : title(_title), genre(_genre), runtime(_runtime) {
            setScore(_score);
        }

        inline double getScore() const {
            return score;
        }

        void setScore(const double _score) {
            if (_score >= 0 && _score <= 100)
                score = _score;
            else
                throw_out_of_range_fmt("value of score (which is %lf) must be"
                " between 0.0 and 100.", _score);
        }
};

struct _Movie_serializer {
    const Movie &_M_movie;
    _Movie_serializer(const Movie &_movie) : _M_movie(_movie) {}
    friend std::ostream &operator<<(std::ostream &, const _Movie_serializer &);
};

std::ostream &operator<<(std::ostream &os, const _Movie_serializer &_movie_s) {
    const Movie &movie = _movie_s._M_movie;
    os << "******************************\n";
    os << "*\n";
    os << "*   Title: " << movie.title << '\n';
    os << "*   Genre: " << movie.genre << '\n';
    os << "*   Running Time (in minutes): " << movie.runtime << '\n';
    os << "*   Rotten Tomatoes Review: " << movie.getScore() << "%\n";
    os << "*\n";
    os << "******************************\n";
    return os;
}

int main() {
    Movie movie1, movie2("Equilibrium"), movie3("Trick'r'treat", "Thriller", 100, 84);

    puts("Movie 1 - Default Constructor\n");
    std::cout << "Enter a title for movie 1: ";
    std::getline(std::cin, movie1.title);
    std::cout << "Enter " << movie1.title << "'s genre: ";
    std::getline(std::cin, movie1.genre);
    std::cout << "Enter " << movie1.title << "'s runtime (in minutes): ";
    std::cin >> movie1.runtime;
    std::cout << "Enter " << movie1.title << "'s Rotten Tomatoes review: ";
    double score; std::cin >> score; movie1.setScore(score);
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << _Movie_serializer(movie1) << '\n\n';

    puts("Movie 2 - Title Only Constructor\n");
    std::cout << "Enter " << movie2.title << "'s genre: ";
    std::getline(std::cin, movie2.genre);
    std::cout << "Enter " << movie2.title << "'s runtime (in minutes): ";
    std::cin >> movie1.runtime;
    std::cout << "Enter " << movie2.title << "'s Rotten Tomatoes review: ";
    std::cin >> score; movie2.setScore(score);
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << _Movie_serializer(movie2) << '\n\n';

    puts("Movie 3 - All data set in constructor.\n");
    std::cout << _Movie_serializer(movie3) << '\n';

    std::cout << "Press any key to continue . . .";
    getchar(); // true platform independence
}
