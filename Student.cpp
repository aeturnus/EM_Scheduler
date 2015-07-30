//
// Created by Brandon on 7/1/2015.
//
#include <string>
#include <sstream>
#include "Student.h"
#include <ostream>
#include <istream>


Student::Student()
{
    shiftCount = 0;
}

Student::~Student()
{

}

/*
bool Student::assign(void)
{
    if(shiftCount < MAX_SHIFTS)
        return true;
    return false;
}

Student::unassign()
{
    if(shiftCount < 0)
        return false;
    return true;
}
 */

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

int Student::getShiftCount()
{
    return shiftCount;
}

void Student::setShiftCount(int count)
{
    shiftCount = count;
}

std::string Student::toString()
{
    std::stringstream sstream;
    sstream<<std::dec<<"["<<id<<"] "<<name<<": "<<std::dec<<shiftCount<<(shiftCount==1?" shift":" shifts")<<std::endl;
    return sstream.str();
}


//Output and input
    /*
     *  4B: id
     *  4B: name length
     * ??B: name
     */
void Student::streamOutBinary(std::ostream &stream)
{
    //stream<<id<<name.length();
    stream.write((char*)&id,4);                 //<id>
    //More efficient for loopage
    unsigned int length = name.length();
    stream.write((char*)&length,4);             //<Name length>
    for(int i = 0; i < length; i++)
    {
        stream.write(&(name.c_str()[i]),1);     //<Name>
    }
}

void Student::streamInBinary(std::istream &stream)
{
    stream.read((char*)&id,4);                  //<id>
    unsigned int length;
    stream.read((char*)&length,4);              //<Name length>

    name.resize(length);
    for(int i = 0;i<length;i++)
    {
        stream.read((char*)&(name.c_str()[i]),1);   //<Name>
    }
}

