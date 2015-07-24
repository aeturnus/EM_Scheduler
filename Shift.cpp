//
// Created by Brandon on 7/1/2015.
//

#include <string>
#include <iostream>
#include <istream>
#include <ostream>
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

Shift& Shift::operator=(Shift &rhs)
{
    if(this == &rhs)
        return *this;
    name = rhs.getName();
    blockReason = rhs.getBlockReason();
    assocDate = rhs.date();
    assocStudent = rhs.student();
    startTime = rhs.getStart();
    endTime = rhs.getEnd();
    blocked = rhs.isBlocked();
    manual = rhs.isManual();

    return *this;
}

void Shift::init(std::string shiftName, Date* datePtr,unsigned int start, unsigned int end)
{
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

std::string Shift::getBlockReason(void)
{
    return blockReason;
}
std::string Shift::getName(void)
{
    return name;
}

std::string Shift::toString(void)
{
    std::stringstream sstream;
    sstream<<assocDate->toString()<<"\n"<<(blocked?(blockReason+" "):(name+" "+shiftTimeString()+" " + (assocStudent==nullptr?"Open":assocStudent->getName())))<<std::endl;
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
    if(st2 <= st1 && st1 <= et2)
        return true;
    if(st2 <= et1 && et1 <= et2)
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


//File
/*
 *  4B: Name length
 * ??B: Name
 *  4B: Block reason length
 * ??B: Block reason
 *  1B: Blocked
 *  1B: Manual
 *  4B: Start time
 *  4B: End time
 *  4B: Date index: equals 0xFFFFFFFF if nullptr
 *  4B: Student index: equals 0xFFFFFFFF if nullptr
 */

void Shift::streamOutBinary(std::ostream &stream, Date *dateList, Student *studentList)
{
    unsigned int length = name.size();
    stream.write((char*)&length,4);                     //<Name length>
    stream.write((char*)name.c_str(),length);           //<Name>
    length = blockReason.size();
    stream.write((char*)&length,4);                     //<Block Reason length>
    stream.write((char*)blockReason.c_str(),length);    //<Block Reason>
    char charBlocked = blocked&0xFF;
    stream.write(&charBlocked,1);                       //<Blocked>
    char charManual = manual&0xFF;
    stream.write(&charManual,1);                        //<Manual>
    stream.write((char*)&startTime,4);                  //<Start time>
    stream.write((char*)&endTime,4);                    //<End time>
    unsigned int index = 0;
    if(assocDate == nullptr)
    {
        index = 0xFFFFFFFF;
    }
    else
    {
        for(index = 0; index < 0xFFFFFFFF && (assocDate != &dateList[index]); index++)
        {
            //Find the index of the date in the datelist
        }
    }
    stream.write((char*)&index,4);                      //<Date index>
    if(assocStudent == nullptr)
    {
        index = 0xFFFFFFFF;
    }
    else
    {
        for(index = 0; index < 0xFFFFFFFF && (assocStudent != &studentList[index]); index++)
        {
            //Find the index of the student in the studentlist
        }
    }
    stream.write((char*)&index,4);                      //<Student index>
}

void Shift::streamInBinary(std::istream &stream, Date *dateList, Student *studentList)
{
    unsigned int length;
    stream.read((char*)&length,4);                      //<Name length>
    name.resize(length);            //Fix up the length
    stream.read((char*)name.c_str(),length);            //<Name>
    stream.read((char*)&length,4);                      //<Block Reason length>
    blockReason.resize(length);     //Fix up the length
    stream.read((char*)blockReason.c_str(),length);    //<Block Reason>

    char charBlocked;
    stream.read(&charBlocked,1);                        //<Blocked>
    blocked = (bool)charBlocked;

    char charManual;
    stream.read(&charManual,1);                         //<Manual>
    manual = (bool)charManual;

    stream.read((char*)&startTime,4);                          //<Start time>
    stream.read((char*)&endTime,4);                            //<End time>

    unsigned int index;
    stream.read((char*)&index,4);                              //<Date index>
    if(index == 0xFFFFFFFF)
        assocDate = nullptr;
    else
        assocDate = &dateList[index];
    stream.read((char*)&index,4);                              //<Student index>
    if(index == 0xFFFFFFFF)
        assocStudent = nullptr;
    else
        assocStudent = &studentList[index];
}
