//
// Created by Brandon on 7/1/2015.
//

#include <string>
#include <sstream>
#include <vector>
#include "Date.h"
#include "Parser.h"

#define GAUSS 1

const std::string Date::WEEKDAY_NAMES[7] = {"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};
const std::string Date::MONTH_NAMES[12] = {"January","February","March","April","May","June","July","August","September","October","November","December"};
const unsigned int Date::MONTH_DAYS[12] = {DAYS_JAN,DAYS_FEB,DAYS_MAR,DAYS_APR,DAYS_MAY,DAYS_JUN,DAYS_JUL,DAYS_AUG,DAYS_SEP,DAYS_OCT,DAYS_NOV,DAYS_DEC};
Date::Date()
{

}

Date::~Date()
{

}

Date& Date::operator=(Date &other)
{
    //setDate(other.getNumDay(),other.getNumMonth(),other.getNumYear());
    if(this == &other)
        return *this;
    day = other.getNumDay();
    month = other.getNumMonth();
    year = other.getNumYear();
    weekday = other.getNumDayOfWeek();
    return *this;
}

bool Date::operator==(Date &rhs)
{
    if(this == &rhs)
        return true;
    if(day != rhs.day)
        return false;
    if(month != rhs.month)
        return false;
    if(year != rhs.year)
        return false;
    //Weekday is assumed to be properly calculated
    return true;
}

std::string Date::toString(void)
{
    std::stringstream sstream;
    sstream<<getDayOfWeek()<<" "<<std::dec<<day<<" "<<getMonth()<<" "<<getYear();
    return sstream.str();
}


void Date::calcDayOfWeek()
{
    #ifdef BASIC

    #endif
    #ifdef ZELLER
    //Zeller's algorithm: comments on variables from the Wikipedia article on "Determination of the day of the week"
    double Y = year - (month == JAN || month == FEB?1:0);      //Y is the year minus 1 for January or February, and the year for any other month
    double y = (int)Y%100;                                      //y is the last 2 digits of Y
    double c = (int)(Y-y)%10000;                                //c is the first 2 digits of Y
    double d = day;                                        //d is simply the day of the month
    double m = (month + 1) + (month==JAN?1:(month==FEB?2:0));  //m is the shifted month (March = 3,...February=14) BRANDON: the ternary operators are to conditionally add 1 or 2 to the month, since Zeller's algo uses a 3-14 month system, with 3 being march and 14 being february. The +1 is because it's not zero indexed
    //Zeller's day of week calculation: (1=Sunday,...0=Saturday)
    unsigned int w = (unsigned int)(d + (13*(m+1)/5) + y + (y/4) + (c/4) - 2*c)%7;

    if(w == 0)
        weekday = 6;
    else
        weekday = w - 1;
    #endif

    #ifdef GAUSS
    //Yeah... Zeller's algorithm wasn't working out too well. I used Gauss's algorithm (on Wikipedia) then expanded on it to provide the rest of the year. Note the lack of floating point
    unsigned int firstday =(1 + 5*((year-1)%4) + 4*((year-1)%100) + 6*((year-1)%400) )%7; //Gauss's algorithm calculates the first day of the year
    unsigned int daysfromfirst = 0;
    //Compensate for the rest of the year
    for(int i = 0; i<month; i++)
    {
        daysfromfirst += getMonthDays(i,year);
    }
    daysfromfirst += (day-1);       //minus one to account for day 1 of January
    weekday = (firstday + (daysfromfirst)%7)%7;
    #endif

}

std::string Date::getMonth()
{
    return MONTH_NAMES[month%12];
}
std::string Date::getDayOfWeek()
{
    return WEEKDAY_NAMES[weekday%7];
}

std::string Date::getYear()
{
    std::stringstream sstream;
    if(year < 0)
    {
        sstream<<year*-1<<" BCE";
    }
    else
    {
        //sstream<<year<<" CE";
        sstream<<year;
    }
    return sstream.str();
}

unsigned int Date::getNumDay()
{
    return day;
}
unsigned int Date::getNumMonth()
{
    return month;
}
int Date::getNumYear()
{
    return year;
}
unsigned int Date::getNumDayOfWeek()
{
    return weekday;
}

bool Date::isLeapYear(int yearInput)
{
    bool output = false;
    if(yearInput%4 == 0)
    {
        output = true;
        if(yearInput%100 == 0)
        {
            output = false;
            if(yearInput%400 == 0)
            {
                output = true;
            }
        }
    }
    return output;
}

void Date::setDate(unsigned int newDay, unsigned int newMonth, int newYear)
{
    //normalize days and months by overflowing them
    while(newDay>getMonthDays(newMonth,newYear))
    {
        newDay -= getMonthDays(newMonth,newYear);
        newMonth++;
        if(newMonth >= 12)
        {
            newYear++;
            newMonth = newMonth%12;
        }
    }
    day = newDay;
    month = newMonth;
    year = newYear;
    calcDayOfWeek();
}

unsigned int Date::getMonthDays(unsigned int inMonth, int inYear)
{
    //Feb is 1
    if(inMonth == FEB)
    {
        if (isLeapYear(inYear))
        {
            return (MONTH_DAYS[inMonth]+1);
        }
    }
    return MONTH_DAYS[inMonth];
}

bool Date::parseDateSet(std::string& datestring)
{
    //<day> <month> <year>
    unsigned int newDay = 0;
    unsigned int newMonth = 0;
    int newYear = 0;
    datestring = Parser::stringToLower(datestring);
    std::vector<std::string> components = Parser::stringSplit(" ",datestring);  //Separate it into the day,month, and year components
    if(components.size() < 3)
        return false;   //Needs 3 components to not fail
    newDay = Parser::parseInt(components[0]);
    newYear = Parser::parseInt(components[2]);
    if(components.size() == 4)
    {
        if(components[3] == "bce")
            newYear *= -1;  //BCE date
    }
    std::string mon = components[1];    //if else chain of doom
    if(mon.find("jan")!=std::string::npos)
        newMonth = JAN;
    else if(mon.find("feb")!=std::string::npos)
        newMonth = FEB;
    else if(mon.find("mar")!=std::string::npos)
        newMonth = MAR;
    else if(mon.find("apr")!=std::string::npos)
        newMonth = APR;
    else if(mon.find("may")!=std::string::npos)
        newMonth = MAY;
    else if(mon.find("jun")!=std::string::npos)
        newMonth = JUN;
    else if(mon.find("jul")!=std::string::npos)
        newMonth = JUL;
    else if(mon.find("aug")!=std::string::npos)
        newMonth = AUG;
    else if(mon.find("sep")!=std::string::npos)
        newMonth = SEP;
    else if(mon.find("oct")!=std::string::npos)
        newMonth = OCT;
    else if(mon.find("nov")!=std::string::npos)
        newMonth = NOV;
    else if(mon.find("dec")!=std::string::npos)
        newMonth = DEC;
    else
        return false;
    setDate(newDay,newMonth,newYear);

    return true;
}

unsigned int Date::daysBetween(Date &date1, Date &date2, bool endIncluded)
{
    unsigned int output = 0;
    int d1 = daysFromEpoch(date1);
    int d2 = daysFromEpoch(date2);
    if(d1 < d2)
    {
        output = d2 - d1;
    }
    else
    {
        output = d1 - d2;
    }
    if(endIncluded)
        output++;
    return output;
}

int Date::daysFromEpoch(Date &date)
{
    int output = 0;
    if(date.getNumYear()>= 0)
    {
        output += date.getNumDay();                                 //Add remaining days in current month
        for(int j = date.getNumMonth()-1; j>=0; j--)                //Sum up the remaining days of the rest of the months
        {
            output += getMonthDays((unsigned int)j, date.getNumYear());
        }
        for(int i = date.getNumYear(); i > 0; i--)                  //Sum up the rest of the years
        {
            output += isLeapYear(i)?366:365;    //Adds days from years
        }
    }
    else
    {

    }
    return output;
}

unsigned int Date::weekdayOfMonth()
{
    if(day >= 1 && day <8)
        return 1;
    if(day >=8 && day < 15)
        return 2;
    if(day >=15 && day < 22)
        return 3;
    if(day >= 22 && day < 29)
        return 4;
    if(day >= 29)
        return 5;
    return 0;
}
