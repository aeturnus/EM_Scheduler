//
// Created by Brandon on 7/1/2015.
//

#include <string>
#include <iostream>
#include <sstream>
#include "Shift.h"
#include "Date.h"
#include "Student.h"

Shift::Shift()
{

}

Shift::~Shift()
{

}

void Shift::init(int shiftID,std::string shiftName, Date* datePtr,unsigned int start, unsigned int end)
{
    id = shiftID;
    assocDate = datePtr;
    setName(shiftName);
    setTime(start,end);
    unassign();
    unblock();
    manual = false;

}

void Shift::setName(std::string input)
{
    name = input;
}

bool Shift::assign(Student *taker)
{
    if(manual)
    {
        return false;
    }
    if(taker->assign())
    {
        assocStudent = taker;
        return true;
    }
    else
    {
        std::cout << taker->getName() << " already has " << std::dec << Student::MAX_SHIFTS << " shifts" << std::endl;
        return false;
    }
}

void Shift::unassign()
{
    assocStudent->unassign();
    assocStudent = nullptr;
}

void Shift::block(std::string reason)
{
    blockReason = reason;
    blocked = true;
    unassign();
}

void Shift::unblock()
{
    blocked = false;
}

bool Shift::isManual()
{
    return manual;
}
void Shift::setManual(bool input)
{
    manual = input;
}

Student* Shift::student(void)
{
    return assocStudent;
}

Date* Shift::date(void)
{
    return assocDate;
}

int Shift::getID(void)
{
    return id;
}

bool Shift::setTime(unsigned int start, unsigned int end)
{
    if(start>=end)
        return false;
    startTime = start;
    endTime = end;
}

unsigned int Shift::getStart(void)
{
    return startTime;
}

unsigned int Shift::getEnd(void)
{
    return endTime;
}

std::string Shift::toString(void)
{
    std::stringstream sstream;
    sstream<<assocDate->toString()<<"\n"<<(blocked?blockReason:(name+" "+shiftTimeString()+" "))<<(assocStudent==nullptr?"Open":assocStudent->getName())<<std::endl;
    return sstream.str();
}

bool Shift::shiftsOverlap(Shift &shift1, Shift &shift2)
{
    //The same shift can't overlap itself
    if(&shift1 == &shift2)
        return false;
    //Logic: use doubles to hold fractional epoch days
    Date date1 = *(shift1.date());
    Date date2 = *(shift2.date());
    unsigned int start1 = shift1.getStart();
    unsigned int end1 = shift1.getEnd();
    unsigned int start2 = shift2.getStart();
    unsigned int end2 = shift2.getEnd();

    //These doubles will hold days from the epoch, plus fractional parts of a day
    double st1 = (double)Date::daysFromEpoch(date1) + (((double)start1)/(24.00));
    double et1 = (double)Date::daysFromEpoch(date1) + (((double)end1)/(24.00));
    double st2 = (double)Date::daysFromEpoch(date2) + (((double)start2)/(24.00));
    double et2 = (double)Date::daysFromEpoch(date2) + (((double)end2)/(24.00));

    //Check if the edges of a shift are in another
    if(st2 < st1 && st1 < et2)
        return true;
    if(st2 < et1 && et1 < et2)
        return true;
    return false;
}

bool Shift::isBlocked(void)
{
    return blocked;
}

std::string Shift::startTimeString(bool hour24)
{
    return Date::timeString(startTime,hour24);
}
std::string Shift::endTimeString(bool hour24)
{
    return Date::timeString(endTime,hour24);
}
std::string Shift::shiftTimeString(bool hour24)
{
    return (Date::timeString(startTime,hour24)+"-"+Date::timeString(endTime,hour24));
}
