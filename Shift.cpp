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
}

void Shift::setName(std::string input)
{
    name = input;
}

void Shift::assign(Student *taker)
{
    if(taker->assign())
        assocStudent = taker;
    else
        std::cout<<taker->getName()<<" already has "<<std::dec<<Student::MAX_SHIFTS<<" shifts"<<std::endl;
}

void Shift::unassign()
{
    assocStudent = nullptr;
}

void Shift::block(std::string reason)
{
    blocked = true;
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
std::string Shift::toString(void)
{
    std::stringstream sstream;
    sstream<<assocDate->toString()<<"\n"<<(blocked?blockReason:name)<<": "<<(assocStudent==nullptr?"Open":assocStudent->getName())<<std::endl;
    return sstream.str();
}
