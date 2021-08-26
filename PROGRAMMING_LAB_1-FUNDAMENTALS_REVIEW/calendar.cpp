#include <iostream>
#include <iomanip>
#include <cstdio>

// Compiled on GNU from MinGW64

// Makoto Miyamoto
// Lucian Silcox COSC 1337

void dispatch(char const *win_cmd, char const *nix_cmd) {
    int res = 0;
    #ifdef _WIN32
        res = std::system(win_cmd);
    #else
        res = std::system(nix_cmd);
    #endif
}

void pause() {
    dispatch("pause", "read -p \"Press [Enter] to continue...\"");
}

void clear() {
    dispatch("cls", "clear");
}

// extern ensures that the constants are not copied
// in the compilation unit. this is one correct
// way to define a constant, even if it isn't
// necessarily imperative to this particular instance.
extern int const MONTHS = 12, WEEKS = 6, DAYS = 7;
extern int const SUNDAY =    1,
                 MONDAY =    2,
                 TUESDAY =   3,
                 WEDNESDAY = 4,
                 THURSDAY =  5,
                 FRIDAY =    6,
                 SATURDAY =  7;

extern int const JANUARY =   0,
                 FEBRUARY =  1,
                 MARCH =     2,
                 APRIL =     3,
                 MAY =       4,
                 JUNE =      5,
                 JULY =      6,
                 AUGUST =    7,
                 SEPTEMBER = 8,
                 OCTOBER =   9,
                 NOVEMBER =  10,
                 DECEMBER =  11;
extern int const EMPTY_CALENDAR_FIELD = 0;
extern int const BEGINNING_MONTH_PADDING = -1;

void GetYear(int *year);
bool IsLeapYear(int year);
void GetFirstDayOfYear(int *day);
void PopulateCalendar(int calendar[MONTHS][WEEKS][DAYS], int year, int firstDay);

// Exit code for QueryCalenderMonth()
extern size_t const EXIT = 13;
// Asks the user to select a month to view, or 13 to exit.
int QueryCalenderMonth();

struct MonthCalendarDisplay {
    int (*m)[DAYS];
    int year;
    int firstDay;
    MonthCalendarDisplay(int (*m)[DAYS], int year, int firstDay) {
        this->m = m;
        this->year = year;
        this->firstDay = firstDay;
    }
};

std::ostream &operator<<(std::ostream &os, MonthCalendarDisplay const& month) {
    os << "| S  | M  | T  | W  | Th | F  | Sa |\n"
       << "|----|----|----|----|----|----|----|\n";
    for (auto week = 0; month.m[week][0] != 0 && week < WEEKS; ++week) {
        os << '|';
        for (auto day = 0; day < DAYS; ++day) {
            if (month.m[week][day] == BEGINNING_MONTH_PADDING 
                || month.m[week][day] == EMPTY_CALENDAR_FIELD) os << "    |";
            else os << std::setw(3) << month.m[week][day] << " |";
        } puts("");
        os << "|----|----|----|----|----|----|----|\n";
    }
    return os;
}

int main() {
    int calendar[MONTHS][WEEKS][DAYS];
    int year = 0;
    int firstDay = 0;

    clear();
    GetYear(&year);
    GetFirstDayOfYear(&firstDay);
    PopulateCalendar(calendar, year, firstDay);
    clear();

    int choice;
    while ((choice = QueryCalenderMonth()) != EXIT) {
        clear();
        std::cout << MonthCalendarDisplay(calendar[choice-1], year, firstDay);
        pause();
        clear();
    }

    return EXIT_SUCCESS;
}

void GetYear(int *year) {
    puts("What year would you like to build a calendar for?");
    printf("Please enter the four-digit year: ");
    std::cin >> *year; 
}

bool IsLeapYear(int year) {
    if (year % 4 == 0) {
        return (year % 100 == 0 && year % 400 == 0) || year % 100 != 0;
    } else {
        return false;
    }
}

void GetFirstDayOfYear(int *day) {
    printf("January 1st of %d was a...\n", *day);
    puts("1) Sunday");
    puts("2) Monday");
    puts("3) Tuesday");
    puts("4) Wednesday");
    puts("5) Thursday");
    puts("6) Friday");
    puts("7) Saturday");
    printf(">> ");
    std::cin >> *day;
}

int GetDaysInMonth(size_t month, int year) {
    switch (month) {
        case APRIL:
        case JUNE:
        case SEPTEMBER:
        case NOVEMBER:
            return 30;
        case FEBRUARY:
            return IsLeapYear(year) ? 29 : 28;
        default:
            return 31;
    }
}

void PopulateCalendar(int calendar[MONTHS][WEEKS][DAYS], int year, int firstDay) {
    if (firstDay < SUNDAY || firstDay > SATURDAY) {
        throw "Invalid first day.";
    }

    // first populate array with all zeroes
    for (auto i = 0; i < MONTHS; ++i) {
        for (auto j = 0; j < WEEKS; ++j) {
            for (auto k = 0; k < DAYS; ++k) {
                calendar[i][j][k] = EMPTY_CALENDAR_FIELD;
            }
        }
    }

    int weekStart = firstDay - 1, nextMonthDayStart = 0;
    for (auto month = 0; month < MONTHS; ++month) {
        int week = 0;
        int daysInMonth = GetDaysInMonth(month, year);
        for (auto day = 1; day <= daysInMonth;) {
            for (auto week_it = weekStart; week_it < DAYS && day <= daysInMonth; ++week_it) {
                calendar[month][week][week_it] = day;
                ++day;
                nextMonthDayStart = week_it + 1;
            }
            weekStart = 0;
            ++week;
        }
        weekStart = nextMonthDayStart;

        for (auto pad = 0; calendar[month][0][pad] == 0; ++pad) {
            calendar[month][0][pad] = BEGINNING_MONTH_PADDING;
        }
    }
}

int QueryCalenderMonth() {
    puts("View Monthly Calendars");
    puts("1) January");
    puts("2) February");
    puts("3) March");
    puts("4) April");
    puts("5) May");
    puts("6) June");
    puts("7) July");
    puts("8) August");
    puts("9) September");
    puts("10) October");
    puts("11) November");
    puts("12) December");
    puts("13) Exit");
    int buf;
    if (! (std::cin >> buf)) return EXIT;
    return buf;
}
