//
// Created by Brandon on 7/1/2015.
//

#include <string>
#include <iostream>
#include <sstream>
#include "Shift.h"
#include "Date.h"
#include "Student.h"

Shift::Shift(Date *date, std::string shiftName)
{
    assocDate = date;
    setName(shiftName);
    unassign();
    unblock();
}

Shift::~Shift()
{
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

std::string Shift::toString(void)
{
    std::stringstream sstream;
    sstream<<(blocked?blockReason:name)<<": "<<(assocStudent==nullptr?"Open":assocStudent->getName())<<std::endl;
    return sstream.str();
}
