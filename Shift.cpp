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

void Shift::init(std::string shiftName, Date* datePtr)
{
    assocDate = datePtr;
    setName(shiftName);
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
    assocStudent->unassign;
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

std::string Shift::toString(void)
{
    std::stringstream sstream;
    sstream<<assocDate->toString()<<"\n"<<(blocked?blockReason:name)<<": "<<(assocStudent==nullptr?"Open":assocStudent->getName())<<std::endl;
    return sstream.str();
}
