//
// Created by Brandon on 7/1/2015.
//
#include <string>
#include <sstream>
#include "Student.h"

Student::Student()
{
    shiftCount = 0;
}

Student::~Student()
{

}

bool Student::assign(void)
{
    if(shiftCount < MAX_SHIFTS)
    {
        shiftCount++;
        return true;
    }
    return false;
}

bool Student::unassign()
{
    if(shiftCount < 0)
        return false;
    shiftCount--;
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

std::string Student::toString()
{
    std::stringstream sstream;
    sstream<<std::dec<<"["<<id<<"]"<<name<<": "<<std::dec<<shiftCount<<(shiftCount==1?" shift":" shifts")<<std::endl;
    return sstream.str();
}

