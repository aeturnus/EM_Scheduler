//
// Created by Brandon on 7/1/2015.
//
#include <string>
#include <sstream>
#include "Student.h"


int Student::MAX_SHIFTS = 14;
int Student::MIN_SHIFTS = 12;


Student::Student()
{
    shiftCount = 0;
}

Student::~Student()
{

}

void Student::setMinShift(int min)
{
    MIN_SHIFTS = min;
}

void Student::setMaxShift(int max)
{
    MAX_SHIFTS = max;
}


bool Student::assign(void)
{
    if(shiftCount < MAX_SHIFTS)
        return true;
    return false;
}

bool Student::unassign()
{
    if(shiftCount < 0)
        return false;
    return true;
}

void Student::setName(std::string inName)
{
    name = inName;
}

std::string Student::getName(void)
{
    return name;
}

void Student::setID(unsigned int input)
{
    id = input;
}
unsigned int Student::getID(void)
{
    return id;
}

unsigned int Student::getShiftCount()
{
    return shiftCount;
}

void Student::setShiftCount(unsigned int count)
{
    shiftCount = count;
}

std::string Student::toString()
{
    std::stringstream sstream;
    sstream<<std::dec<<"["<<id<<"] "<<name<<": "<<std::dec<<shiftCount<<(shiftCount==1?" shift":" shifts")<<std::endl;
    return sstream.str();
}

