//
// Created by Brandon on 7/17/2015.
//

#include "Scheduler.h"
#include <string>
#include <sstream>
#include <vector>
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

void Scheduler::init(std::string inName,Date start, Date end, unsigned int numshift, std::string shiftNames[],unsigned int shiftTimes[][2], unsigned int numstudents, std::string studentNames[])
{
    name = inName;
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
            shiftList[i*disShiftNum + j].init(shiftNames[j],&dateList[i],shiftTimes[j][0],shiftTimes[j][1]);  //Initialize shift names and dates and times
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
    std::vector<Student*> excludevector;
    int eligcount;
    bool skip = false;
    for(int i = 0; i < shiftNum; i++)
    {
        //Kills itself on the 78th iteration

        //Tries to find an elligible student if it's not manually set or is blocked
        if(!shiftList[i].isManual() && !shiftList[i].isBlocked())
        {
            skip = false;
            eligStudent = minStudent();
            excludevector.clear();
            while(checkOverlaps(&shiftList[i],eligStudent) && !checkMinTimeSinceLastShift(&shiftList[i],eligStudent,8) && skip == false && eligStudent != nullptr)
            {
                //While it overlaps, try another eligStudent by finding another minStudent excluding itself

                //Should not
                if(excludevector.size() >= studentNum)
                {
                    //If none fit the criteria, then we skip this shift. Such automation.
                    skip = true;
                }
                if(!skip)
                {
                    excludevector.push_back(eligStudent);
                    eligStudent = minStudent(&excludevector);  //minStudent will exclude all entries up to the eligStudent
                }
            }
            if(eligStudent == nullptr)
            {
                return false;
            }
            if(!skip)
                assign(&shiftList[i],eligStudent);
        }
        if(!shiftList[i].isBlocked() && !skip)
            updateShiftCount(shiftList[i].student());       //Update the count of a student. If it's manually set, it'll update the count for th student that's assigned. Only if not blocked
    }
    return true;
}

bool Scheduler::assign(Shift *shiftPtr, Student *stuPtr)
{
    return shiftPtr->assign(stuPtr);
}

Student* Scheduler::minStudent(std::vector<Student*> *excludevector)
{
    //The entries will be checked against the excludevector
    int mindex = 0;
    bool exclude;
    for(int i = 0; i<studentNum; i++)
    {
        if(studentList[i].getShiftCount() < studentList[mindex].getShiftCount())
        {
            //Idea: if the new minStudent is in the excludevector, then skip it
            exclude = false;
            if(excludevector != nullptr)
            {
                for (int j = 0; j < (*excludevector).size(); j++)
                {
                    if ((*excludevector)[i] == &studentList[i])
                        exclude = true;
                }
            }
            if(!exclude)
            {
                mindex = i; //If the address of the excluded student is the same as the current, and it is smaller than the minimum index, it will skip this step updating the mindex
            }
        }
    }
    if(studentList[mindex].getShiftCount() >= Student::MAX_SHIFTS)
    {
        //Return a nullpointer if it fails to work
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

void Scheduler::autoblock()
{
    Date* shiftdate;

    Shift thurMeet;
    Shift wedMeet;
    for(int i = 0; i<shiftNum; i++)
    {
        shiftdate = shiftList[i].date();
        thurMeet.init("",shiftdate,0,14);     //Temporary shift to try overlapping with
        wedMeet.init("",shiftdate,23,32);     //Temporary shift to try overlapping with

        //Thursday
        if(shiftdate->getNumDayOfWeek()==4)
            if(shiftdate->weekdayOfMonth()!=3)
                if(Shift::shiftsOverlap(thurMeet,shiftList[i]))
                    shiftList[i].block("EM conference 7AM-2PM");
        //Wednesday
        if(shiftdate->getNumDayOfWeek()==3)
            if(shiftdate->weekdayOfMonth()!=3)
                if(Shift::shiftsOverlap(wedMeet,shiftList[i]))
                    shiftList[i].block("EM conference 11PM-8AM");
    }

    //Block off night shifts on the last day
    Shift tempShift;
    tempShift.init("temp",shiftList[shiftNum-1].date(),17,24);   //Temporary shift to compare to
    for(int i = shiftNum - disShiftNum; i < shiftNum; i++)
    {
        if(Shift::shiftsOverlap(shiftList[i],tempShift))
        {
            shiftList[i].block("");
        }
    }


}


bool Scheduler::checkMinTimeSinceLastShift(Shift *targetShiftPtr, Student *studentPtr, unsigned int minTime)
{
    unsigned int lastdex;   //The index of the latest shift
    Shift tempShift;        //Temporary shift to edit for time
    for(int i = 0; i < shiftNum; i++)
    {
        //Hunt for shifts that studentPtr has
        if(shiftList[i].student() == studentPtr)
            lastdex = i;
    }

    tempShift = shiftList[lastdex];     //Use operator= to make a temporary shift to check overlaps with
    tempShift.setTime(tempShift.getStart(),(tempShift.getEnd() + minTime - 1)); //Add minTime to the end time. Subtract 1 since shiftOverlaps returns true if the values are the exact.

    return !(Shift::shiftsOverlap(tempShift,*targetShiftPtr));    //Compare the shifts to see if they overlap. If they overlap, return false
}



//Getters

unsigned int Scheduler::getDateNum(void)
{
    return dateNum;
}
unsigned int Scheduler::getShiftNum(void)
{
    return shiftNum;
}
unsigned int Scheduler::getStudentNum(void)
{
    return studentNum;
}

Date* Scheduler::dates(void)
{
    return dateList;
}
Shift* Scheduler::shifts(void)
{
    return shiftList;
}
Student* Scheduler::students(void)
{
    return studentList;
}

std::string Scheduler::getName(void)
{
    return name;
}


//Files
/*
 *  4B: Name length
 * ??B: Name
 *  4B: # of students
 * ??B: Students
 * 12B: Start Date
 * 12B: End Date
 *  4B: # of shifts
 * ??B: Shifts
 */
void Scheduler::streamOutBinary(std::ostream &stream)
{
    unsigned int length = name.length();
    stream.write((char*)&length,4);                                 //<Name length>

    stream.write(name.c_str(),length);                              //<Name>

    stream.write((char*)&studentNum,4);                             //<Number of students>
    for(int i = 0; i < studentNum; i++)
    {
        studentList[i].streamOutBinary(stream);                     //<Students>
    }

    Date start = dateList[0];
    Date end = dateList[dateNum-1];

    start.streamOutBinary(stream);                                  //<Start date>
    end.streamOutBinary(stream);                                    //<End date>

    stream.write((char*)&shiftNum,4);                               //<# of shifts>
    for(int i = 0; i < shiftNum; i++)
    {
        shiftList[i].streamOutBinary(stream,dateList,studentList);  //<Shifts>
    }


}

void Scheduler::streamInBinary(std::istream &stream)
{
    //Free what it already has
    delete[] shiftList;
    delete[] dateList;
    delete[] studentList;
    //

    unsigned int length;
    stream.read((char*)&length,4);                                                          //<Length of Name>
    name.resize(length);
    stream.read((char*)name.c_str(),length);                                                //<Name>
    stream.read((char*)&studentNum,4);                                                      //<# of students>

    studentList = new Student[studentNum]();                                                //Allocate space for the students
    for(int i = 0; i < studentNum; i++)
    {
        studentList[i].streamInBinary(stream);                                              //<Students>
    }

    Date start;
    Date end;
    start.streamInBinary(stream);                                                           //<Start date>
    end.streamInBinary(stream);                                                             //<End date>
    dateNum = Date::daysBetween(start,end,true);
    dateList = new Date[dateNum]();

    //Setup the dateList
    for(int i = 0; i<dateNum; i++)
    {
        //Add additional days from start
        start.setDate(start.getNumDay()+(i==0?0:1),start.getNumMonth(),start.getNumYear());
        dateList[i] = start;
    }

    stream.read((char*)&shiftNum,4);                                                        //<# of shifts>
    shiftList = new Shift[shiftNum]();
    for(int i = 0; i<shiftNum; i++)
    {
        //Load the shifts
        shiftList[i].streamInBinary(stream,dateList,studentList);                           //<Shifts>
    }

    disShiftNum = 6;    //Basically worthless after the initialization
}