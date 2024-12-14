//
//  main.cpp
//  dates
//
//  Created by maya nachiappan on 12/13/24.
//

#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <iomanip>

// Helper function to determine if a year is a leap year
bool isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

// Helper function to get the number of days in a month
int daysInMonth(int year, int month) {
    std::vector<int> days = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month == 2 && isLeapYear(year)) {
        return 29;
    }
    return days[month - 1];
}

// Parent class DateTime
class DateTime {
protected:
    int Hour, Minute, Second; // Time variables
    int Year, Month, Day;     // Date variables

public:
    // Virtual methods to allow derived class-specific implementations
    virtual int GetHour() const = 0;
    virtual int GetMinute() const = 0;
    virtual int GetSecond() const = 0;
    virtual int GetYear() const = 0;
    virtual int GetMonth() const = 0;
    virtual int GetDay() const = 0;

    // Compute difference between two DateTime objects
    void Difference(const DateTime& other, int& totalDays, int& totalHours, int& totalMinutes, int& totalSeconds) const {
        // Convert both dates to total days since year 0
        auto daysSinceEpoch = [](int year, int month, int day) {
            int days = day;
            for (int y = 0; y < year; ++y) {
                days += isLeapYear(y) ? 366 : 365;
            }
            for (int m = 1; m < month; ++m) {
                days += daysInMonth(year, m);
            }
            return days;
        };

        int thisDays = daysSinceEpoch(Year, Month, Day);
        int otherDays = daysSinceEpoch(other.GetYear(), other.GetMonth(), other.GetDay());

        // Calculate time difference in seconds
        int thisTimeInSeconds = Hour * 3600 + Minute * 60 + Second;
        int otherTimeInSeconds = other.GetHour() * 3600 + other.GetMinute() * 60 + other.GetSecond();

        int dayDifference = thisDays - otherDays;
        int timeDifference = thisTimeInSeconds - otherTimeInSeconds;

        // Adjust for day boundaries
        if (timeDifference < 0) {
            --dayDifference;
            timeDifference += 86400; // Seconds in a day
        }

        totalDays = dayDifference;
        totalHours = timeDifference / 3600;
        totalMinutes = (timeDifference % 3600) / 60;
        totalSeconds = timeDifference % 60;
    }

    // Format method for displaying date and time
    std::string Format(const std::string& formatString) const {
        std::string result = formatString;

        auto replaceAll = [&result](const std::string& placeholder, const std::string& value) {
            size_t pos = 0;
            while ((pos = result.find(placeholder, pos)) != std::string::npos) {
                result.replace(pos, placeholder.length(), value);
                pos += value.length();
            }
        };

        replaceAll("YYYY", std::to_string(GetYear()));
        replaceAll("MM", (GetMonth() < 10 ? "0" : "") + std::to_string(GetMonth()));
        replaceAll("DD", (GetDay() < 10 ? "0" : "") + std::to_string(GetDay()));
        replaceAll("HH", (GetHour() < 10 ? "0" : "") + std::to_string(GetHour()));
        replaceAll("mm", (GetMinute() < 10 ? "0" : "") + std::to_string(GetMinute()));
        replaceAll("SS", (GetSecond() < 10 ? "0" : "") + std::to_string(GetSecond()));

        return result;
    }
};

// Derived class Time
class Time : public DateTime {
public:
    // Constructor for Time
    Time(int hour, int minute, int second) {
        Hour = hour;
        Minute = minute;
        Second = second;
    }

    // Getters
    int GetHour() const override {
        return Hour;
    }

    int GetMinute() const override {
        return Minute;
    }

    int GetSecond() const override {
        return Second;
    }

    int GetYear() const override {
        return 0; // Not applicable for Time
    }

    int GetMonth() const override {
        return 0; // Not applicable for Time
    }

    int GetDay() const override {
        return 0; // Not applicable for Time
    }

    // Add seconds, minutes, and hours
    void AddTime(int hours, int minutes, int seconds) {
        Second += seconds;
        Minute += Second / 60;
        Second %= 60;

        Minute += minutes;
        Hour += Minute / 60;
        Minute %= 60;

        Hour += hours;
        Hour %= 24; // Wrap around to stay within 24 hours
    }

    // Subtract seconds, minutes, and hours
    void SubtractTime(int hours, int minutes, int seconds) {
        AddTime(-hours, -minutes, -seconds);
        if (Second < 0) {
            Second += 60;
            Minute--;
        }
        if (Minute < 0) {
            Minute += 60;
            Hour--;
        }
        if (Hour < 0) {
            Hour += 24;
        }
    }
};

// Derived class Date
class Date : public DateTime {
public:
    // Constructor for Date
    Date(int year, int month, int day) {
        Year = year;
        Month = month;
        Day = day;
    }

    // Getters
    int GetYear() const override {
        return Year;
    }

    int GetMonth() const override {
        return Month;
    }

    int GetDay() const override {
        return Day;
    }

    int GetHour() const override {
        return 0; // Not applicable for Date
    }

    int GetMinute() const override {
        return 0; // Not applicable for Date
    }

    int GetSecond() const override {
        return 0; // Not applicable for Date
    }

    // Add days, months, and years
    void AddDate(int years, int months, int days) {
        Year += years;
        Month += months;
        while (Month > 12) {
            Year++;
            Month -= 12;
        }
        while (Month < 1) {
            Year--;
            Month += 12;
        }

        Day += days;
        while (Day > daysInMonth(Year, Month)) {
            Day -= daysInMonth(Year, Month);
            Month++;
            if (Month > 12) {
                Year++;
                Month = 1;
            }
        }
        while (Day < 1) {
            Month--;
            if (Month < 1) {
                Year--;
                Month = 12;
            }
            Day += daysInMonth(Year, Month);
        }
    }

    // Subtract days, months, and years
    void SubtractDate(int years, int months, int days) {
        AddDate(-years, -months, -days);
    }
};

int main() {
    int hour, minute, second;
    int year, month, day;

    // Input for Date 1
    std::cout << "Enter first date (year month day): ";
    std::cin >> year >> month >> day;
    Date d1(year, month, day);

    // Input for Time 1
    std::cout << "Enter first time (hour minute second): ";
    std::cin >> hour >> minute >> second;
    Time t1(hour, minute, second);

    // Input for Date 2
    std::cout << "Enter second date (year month day): ";
    std::cin >> year >> month >> day;
    Date d2(year, month, day);

    // Input for Time 2
    std::cout << "Enter second time (hour minute second): ";
    std::cin >> hour >> minute >> second;
    Time t2(hour, minute, second);

    // Calculate Difference
    int totalDays, totalHours, totalMinutes, totalSeconds;
    d1.Difference(d2, totalDays, totalHours, totalMinutes, totalSeconds);

    std::cout << "Difference between dates and times: " << totalDays << " days, " << totalHours << " hours, "
              << totalMinutes << " minutes, " << totalSeconds << " seconds." << std::endl;

    // Demonstrate Format method
    std::cout << "Formatted Date 1: " << d1.Format("YYYY-MM-DD") << std::endl;
    std::cout << "Formatted Time 1: " << t1.Format("HH:mm:SS") << std::endl;

    return 0;
}

