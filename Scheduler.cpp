//
// Created by Brandon on 7/17/2015.
//

#include "Scheduler.h"
#include <string>
#include <sstream>
#include "Date.h"
#include "Student.h"
#include "Shift.h"

Scheduler::Scheduler()
{

}

Scheduler::~Scheduler()
{
    delete[] shiftList;
    delete[] dateList;
    delete[] studentList;
}

void Scheduler::init(Date start, Date end, unsigned int numshift, std::string shiftNames[],unsigned int shiftTimes[][2], unsigned int numstudents, std::string studentNames[])
{
    disShiftNum = numshift;
    dateNum = Date::daysBetween(start,end,true);    //Get the number of days between to allocate dates
    shiftNum = dateNum * disShiftNum;
    studentNum = numstudents;

    dateList = new Date[dateNum]();                 //Allocate mem for dates
    shiftList = new Shift[shiftNum]();              //Allocate mem for shifts
    studentList = new Student[studentNum]();        //Allocate mem for students. Forgot this earlier. Derp.

    //Initialize the dates
    for(int i = 0; i<dateNum; i++)
    {
        start.setDate(start.getNumDay()+(i==0?0:1),start.getNumMonth(),start.getNumYear()); //Add additional days from start
        dateList[i] = start;
    }
    //Link shifts to date and names
    for(int i = 0; i < dateNum; i++)
    {
        for(int j = 0; j<disShiftNum; j++)
        {
            shiftList[i*disShiftNum + j].init(j,shiftNames[j],&dateList[i],shiftTimes[j][0],shiftTimes[j][1]);  //Initialize shift names and dates and times
        }
    }

    //Name the students
    for(int i = 0; i < studentNum; i++)
    {
        studentList[i].setName(studentNames[i]);
    }
}

bool Scheduler::autoassign(void)
{
    //Update shift counts for all
    for(int i = 0; i < studentNum; i++)
    {
        updateShiftCount(&studentList[i]);
    }
    Student* eligStudent;
    for(int i = 0; i < shiftNum; i++)
    {
        if(!shiftList[i].isManual())
        {
            eligStudent = minStudent();
            while(checkOverlaps(&shiftList[i],eligStudent))
            {
                //While it overlaps, try another eligStudent by finding another minStudent excluding itself
                eligStudent = minStudent(eligStudent);
            }
            if(eligStudent == nullptr)
            {
                return false;
            }
            assign(&shiftList[i],eligStudent);
        }
        if(!shiftList[i].isBlocked())
            updateShiftCount(shiftList[i].student());       //Update the count of a student. If it's manually set, it'll update the count for th student that's assigned. Only if not blocked
    }
    return true;
}

bool Scheduler::assign(Shift *shiftPtr, Student *stuPtr)
{
    return shiftPtr->assign(stuPtr);
}

Student* Scheduler::minStudent(Student* exclude)
{
    int mindex = 0;
    for(int i = 0; i<studentNum; i++)
    {
        if(studentList[i].getShiftCount() < studentList[mindex].getShiftCount())
            if(&studentList[i] != exclude || exclude == nullptr)
                mindex = i; //If the address of the excluded student is the same as the current, and it is smaller than the minimum index, it will skip this step updating the mindex
    }
    if(studentList[mindex].getShiftCount() >= Student::MAX_SHIFTS)
    {
        return nullptr;
    }
    return &studentList[mindex];
}

void Scheduler::updateShiftCount(Student* studentPtr)
{
    unsigned int count = 0;
    for(int i = 0; i < shiftNum; i++)
    {
        if(shiftList[i].student() == studentPtr)
            count++;
    }
    studentPtr->setShiftCount(count);
}

bool Scheduler::checkOverlaps(Shift *shiftPtr, Student *studentPtr)
{
    for(int i = 0; i<shiftNum; i++)
    {
        if(Shift::shiftsOverlap(*shiftPtr,shiftList[i]))
            if(shiftList[i].student() == studentPtr)
                return true;
    }
    return false;
}




std::string Scheduler::toString(void)
{

    std::stringstream sstream;
    for(int i = 0; i<shiftNum; i++)
    {
        sstream<<shiftList[i].toString()<<"\n";
    }
    sstream<<std::endl;

    return sstream.str();
}
