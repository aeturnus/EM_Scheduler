//
// Created by Brandon on 7/1/2015.
//

#ifndef EM_SCHEDULER_DATE_H
#define EM_SCHEDULER_DATE_H

#include <string>
#include <ostream>
#include <istream>
class Date
{
public:
    //Constants
    static const unsigned int JAN = 0;
    static const unsigned int FEB = 1;
    static const unsigned int MAR = 2;
    static const unsigned int APR = 3;
    static const unsigned int MAY = 4;
    static const unsigned int JUN = 5;
    static const unsigned int JUL = 6;
    static const unsigned int AUG = 7;
    static const unsigned int SEP = 8;
    static const unsigned int OCT = 9;
    static const unsigned int NOV = 10;
    static const unsigned int DEC = 11;

    static const unsigned int DAYS_JAN = 31;
    static const unsigned int DAYS_FEB = 28;
    static const unsigned int DAYS_MAR = 31;
    static const unsigned int DAYS_APR = 30;
    static const unsigned int DAYS_MAY = 31;
    static const unsigned int DAYS_JUN = 30;
    static const unsigned int DAYS_JUL = 31;
    static const unsigned int DAYS_AUG = 31;
    static const unsigned int DAYS_SEP = 30;
    static const unsigned int DAYS_OCT = 31;
    static const unsigned int DAYS_NOV = 30;
    static const unsigned int DAYS_DEC = 31;

    static const std::string WEEKDAY_NAMES[7];
    static const std::string MONTH_NAMES[12];
    static const unsigned int MONTH_DAYS[12];

    static bool isLeapYear(int yearInput);      //Checks if a given year is a leap year
    Date();
    ~Date();
    Date& operator=(Date& other);
    bool operator==(Date &rhs);
//    Date operator+(const Date& other);
//    Date operator-(const Date& other);

    //Function to set the date
    void calcDayOfWeek(void);

    //Getters
    std::string getYear(void);
    int getNumYear(void);

    std::string getDayOfWeek(void); //Returns the weekday's name
    unsigned int getNumDayOfWeek(void);      //Returns the weekday's number

    std::string getMonth(void);     //Returns the month's name
    unsigned int getNumMonth(void);          //Returns the month's number

    unsigned int getNumDay(void);            //Returns the day's number

    static unsigned int getMonthDays(unsigned int inMonth, int inYear);
    //Modifiers
    void addTime(int days, int months=0, int years=0);   //Adds a number of time to the current date. Will update.
    void setDate(unsigned int newDay, unsigned int newMonth = 0, int newYear = 0); //Will set it to the new date. If day is too large for the month, it will increment the provided month

    std::string toString(void);


//    bool parseDate(std::string& datestring); //False for a failed parse
    bool parseDateSet(std::string& datestring); //False for a failed parse

    static unsigned int daysBetween(Date& date1, Date& date2, bool includeEnd); //Calculates the difference in days between two dates. includeEnd means that it includes the last date in the count
    static int daysFromEpoch(Date& date);
    static std::string timeString(unsigned int hour, bool hour24 = false);
    //Returns number of weekday in the month e.g. 3rd wednesday of the month
    //Returns 0 if failed
    unsigned int weekdayOfMonth(void);


    //File
    void streamOutBinary(std::ostream& stream);
    void streamInBinary(std::istream& stream);
protected:


private:
    int year;               //Current year. 0 is 0 CE
    unsigned int month;     //Current month
    unsigned int day;       //Current day
    unsigned int weekday;   //Weekday
//    std::uint8_t hour;      //Current hour



};

#endif //EM_SCHEDULER_DATE_H
