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
    hasMemoryAllocated = false;
}

Scheduler::~Scheduler()
{
    delete[] shiftList;
    delete[] dateList;
    delete[] studentList;
    hasMemoryAllocated = false;
}

void Scheduler::init(std::string inName,Date start, Date end, unsigned int numshift, std::string shiftNames[],unsigned int shiftTimes[][2], unsigned int numstudents, std::string studentNames[])
{
    name = inName;
    disShiftNum = numshift;
    dateNum = Date::daysBetween(start,end,true);    //Get the number of days between to allocate dates
    shiftNum = dateNum * disShiftNum;
    studentNum = numstudents;

    maxShifts = 14;
    minShifts = 12;
    maxConsecutive = 4;

    if(hasMemoryAllocated)
    {
        delete[] shiftList;
        delete[] dateList;
        delete[] studentList;
    }

    dateList = new Date[dateNum]();                 //Allocate mem for dates
    shiftList = new Shift[shiftNum]();              //Allocate mem for shifts
    studentList = new Student[studentNum]();        //Allocate mem for students. Forgot this earlier. Derp.
    hasMemoryAllocated = true;
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
    //Start off unassigning all
    for(int i = 0; i < shiftNum; i++)
    {
        if(!shiftList[i].isManual())
        {
            unassign(&shiftList[i],false);
        }
    }

    /*
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

        //THE GREAT OVERLAP BUG
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
                assign(&shiftList[i],eligStudent,false);
        }
        if(!shiftList[i].isBlocked() && !skip && shiftList[i].student() != nullptr)
            updateShiftCount(shiftList[i].student());       //Update the count of a student. If it's manually set, it'll update the count for the student that's assigned. Only if not blocked
    }
    return true;
    */
    ///*
    //Experimental logic
    Student* eligStudent;
    std::vector<Student*> excludevector;

    Shift* shift;
    AssignReturn state;

    //For each shift
    for(int i = 0; i < shiftNum; i++)
    {
        //Shift can't be blocked off or be manual
        if(!shiftList[i].isBlocked() && !shiftList[i].isManual())
        {
            excludevector.clear();
            /*
            for(eligStudent = minStudent(&excludevector), state = assign(&shiftList[i],eligStudent,false);(state != SUCCESS) && (excludevector.size() < studentNum) && (eligStudent != nullptr);eligStudent = minStudent(&excludevector),state = assign(&shiftList[i],eligStudent,false))
            {
                excludevector.push_back(eligStudent);   //Push the student into the list
            }
            */
            eligStudent = minStudent(&excludevector);
            state = assign(&shiftList[i],eligStudent,false);
            while((state != SUCCESS) && (excludevector.size() < studentNum) && (eligStudent != nullptr))
            {
                excludevector.push_back(eligStudent);
                eligStudent = minStudent(&excludevector);
                state = assign(&shiftList[i],eligStudent,false);
            }
            excludevector.clear();
            if(state != SUCCESS)
            {
                resolve(&shiftList[i],&excludevector);
            }
        }
    }
    return true;
    //*/

}

Scheduler::AssignReturn Scheduler::assign(Shift* shiftPtr, Student* stuPtr, bool manual, bool override)
{
    Student* oldstudent;
    oldstudent = shiftPtr->student();
    if(stuPtr == nullptr)
        return ISNULL;
    if(stuPtr->getShiftCount() < maxShifts)
    {
        if(checkOverlaps(shiftPtr,stuPtr))
            return OVERLAP;
        if(!override)   //Override can ignore these facts
        {
            if(!checkMaxConsecutiveDays(shiftPtr,stuPtr))
                return CONSEC;
            if(!checkMinTimeSinceLastShift(shiftPtr,stuPtr,8))
                return MINTIME;
        }
        shiftPtr->assign(stuPtr);
        if(oldstudent != nullptr)
            oldstudent->setShiftCount(oldstudent->getShiftCount()-1);   //Update the old student that was there
        if(manual)
            shiftPtr->setManual(manual);
        updateShiftCount(stuPtr);       //Update the count of a student. If it's manually set, it'll update the count for the student that's assigned. Only if not blocked
        return SUCCESS;
    }
    else
    {
        return OVERMAX;
    }
}

void Scheduler::unassign(Shift *shiftPtr, bool manual)
{
    Student* student = shiftPtr->student();
    if(student == nullptr)
        return;
    shiftPtr->unassign();
    if(manual)
        shiftPtr->setManual(manual);
    updateShiftCount(student);
}

bool Scheduler::resolve(Shift *shiftPtr,std::vector<Student*>* excludevector)
{
    //Recursion break case
    if(excludevector->size() >= studentNum)
        return false;

    AssignReturn resultList[studentNum];    //Make an array of assignment returns for each student
    for(int i = 0 ; i < studentNum; i++)
    {
        if(!studentInVector(&studentList[i],excludevector))
        {
            resultList[i] = assign(shiftPtr,&studentList[i],false);
            if(resultList[i] == SUCCESS)
                return false;                         //None of them should've been a success. Just handle this corner case
        }
        else
        {
            //If student not in vector, handle
            resultList[i] = ISNULL;
        }
    }
    int tryindex = -1;       //Index of student to try to resolve
    for(int i = 0 ; i < studentNum; i++)
    {
        //Try for mintime first
        if(resultList[i] == MINTIME)
        {
            tryindex = i;
        }
    }
    if(tryindex == -1)
    {
        //Try consec
        for(int i = 0; i < studentNum; i++)
        {
            if(resultList[i] == CONSEC)
            {
                tryindex = i;
            }
        }
    }
    if(tryindex == -1)
    {
        //Try same time
        for(int i = 0; i < studentNum; i++)
        {
            if(resultList[i] == OVERLAP)
            {
                tryindex = i;
            }
        }
    }
    if(tryindex == -1)
        return false;

    //We now have an index of student to try: looks backwards
    Student* stuPtr = &studentList[tryindex];
    int shiftDex = indexOfShift(shiftPtr);  //Get a starting index in the list
    for (int i = shiftDex; i >= 0; i--)
    {
        if(shiftList[i].student() == stuPtr)
        {
            shiftDex = i;   //Shiftdex is now the index of the shift to replace
            break;
        }
    }
    AssignReturn state = ISNULL;
    //Try assigning another student to the shift
    for(int i=0; i < studentNum && state!=SUCCESS; i++)
    {
        if(&studentList[i] != stuPtr)
        {
            state = assign(&shiftList[shiftDex],&studentList[i],false);     //Find a student to assign the previous shift
        }
    }
    if(state != SUCCESS)
    {
        //Comes up negative
        unassign(&shiftList[shiftDex],false);
    }


    //Assign the target shift with the targeted student
    state = assign(shiftPtr,&studentList[tryindex],false);
    if(state != SUCCESS)
    {
        //If it didn't work, try again
        excludevector->push_back(&studentList[tryindex]);
        return resolve(shiftPtr,excludevector);
    }

    return true;

}

bool Scheduler::studentInVector(Student *stuPtr, std::vector<Student *>* vector)
{
    for(int i = vector->size()-1; i >= 0; i--)
    {
        if(stuPtr == (*vector)[i])
            return true;
    }
    return false;
}

Student* Scheduler::minStudent(std::vector<Student*> *excludevector)
{
    /*
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
    if(studentList[mindex].getShiftCount() >= maxShifts)
    {
        //Return a nullpointer if it fails to work
        return nullptr;
    }
    return &studentList[mindex];
    */
    //New logic
    int mindex = 0;
    //Check if mindex 0 has a shift count that isn't in exclude
    bool mindexInExclude = false;
    if(excludevector->size() == 0)
        mindexInExclude = true;
    while(!mindexInExclude && (mindex < studentNum))
    {
        for(int i = 0; i < excludevector->size(); i++)
        {
            if(&studentList[mindex] == (*excludevector)[i])
            {
                mindex++;
            }
            else
            {
                mindexInExclude = true;
            }
        }
    }
    if(mindex >= studentNum)
        return nullptr;

    bool skip;
    for(int i = 0; i < studentNum; i++)
    {
        skip = false;
        if(studentList[i].getShiftCount() < studentList[mindex].getShiftCount())
        {
            for(int j = excludevector->size()-1; j >=0 && !skip; j--)
            {
                if(&studentList[i] == (*excludevector)[j])
                {
                    skip = true;
                }
            }
            if(!skip)
            {
                mindex = i; //Update mindex if the lowest shift count student is not in exclude vector
            }
        }
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
        if(!(shiftList[i].isManual()))
        {
            shiftdate = shiftList[i].date();
            thurMeet.init("", shiftdate, 0, 14);     //Temporary shift to try overlapping with
            wedMeet.init("", shiftdate, 23, 32);     //Temporary shift to try overlapping with

            //Thursday
            if (shiftdate->getNumDayOfWeek() == 4)
                if (shiftdate->weekdayOfMonth() != 3)
                    if (Shift::shiftsOverlap(thurMeet, shiftList[i]))
                        shiftList[i].block("EM conference");
            //Wednesday
            if (shiftdate->getNumDayOfWeek() == 3)
                if (shiftdate->weekdayOfMonth() != 3)
                    if (Shift::shiftsOverlap(wedMeet, shiftList[i]))
                        shiftList[i].block("EM conference");
        }
    }

    //Block off night shifts on the last day
    Shift tempShift;
    tempShift.init("temp",shiftList[shiftNum-1].date(),17,24);   //Temporary shift to compare to
    //Cornercase: last day also fulfills the previous autoblocker criteria
    int i = shiftNum -1;
    while(shiftList[i].date() == &dateList[dateNum-1])
    {
        i--;
    }
    for(i++; i < shiftNum; i++)
    {
        if(Shift::shiftsOverlap(shiftList[i],tempShift))
        {
            shiftList[i].block("");
        }
    }


}


bool Scheduler::checkMinTimeSinceLastShift(Shift *targetShiftPtr, Student *studentPtr, unsigned int minTime)
{
    //Case check for first and last days!!

    bool output = true;
    unsigned int lastdex = 0;   //The index of the latest shift
    Shift tempShift;        //Temporary shift to edit for time
    //NEEDS TO STOP WHEN IT REACHES THE SHIFT
    //New logic: overlap check for ALL shifts
    for(int i = 0; i < shiftNum; i++)
    {
        if(shiftList[i].student() == studentPtr)
        {
            if(&shiftList[i] != targetShiftPtr)
            {
                tempShift = shiftList[i];
                tempShift.setTime(tempShift.getStart() - minTime,tempShift.getEnd() + minTime);
                if(Shift::shiftsOverlap(tempShift,*targetShiftPtr))
                {
                    return false;
                }
            }
        }
    }
    return true;


    /*
    for(int i = 0; i < shiftNum && (&shiftList[i] == targetShiftPtr); i++)
    {
        //Hunt for shifts that studentPtr has
        if(shiftList[i].student() == studentPtr)
        {
            lastdex = i;
        }
    }
    if(lastdex == shiftNum)
    {
        lastdex = shiftNum-1;
    }

    tempShift = shiftList[lastdex];     //Use operator= to make a temporary shift to check overlaps with
    tempShift.setTime(tempShift.getStart(),(tempShift.getEnd() + minTime - 0)); //Add minTime to the end time. Subtract 1 since shiftOverlaps returns true if the values are the exact.
    if(tempShift.date() != &dateList[0])
        output = !(Shift::shiftsOverlap(tempShift,*targetShiftPtr));    //Change the output if its not first
    */
    //Look forward
    /*
    for(int i = indexOfShift(targetShiftPtr) + 1;i < shiftNum; i++)
    {
        if(shiftList[i].student() == studentPtr)
        {
            lastdex = i;
        }
    }
    if(lastdex == shiftNum)
    {
        lastdex = shiftNum -1;
    }

    tempShift = shiftList[lastdex];
    tempShift.setTime(tempShift.getStart(),(tempShift.getEnd() - minTime - 0)); //Add minTime to the end time. Subtract 1 since shiftOverlaps returns true if the values are the exact.

    if(!output)
    {
        if(tempShift.date() != &dateList[dateNum-1])
            output = !(Shift::shiftsOverlap(tempShift,*targetShiftPtr));
    }
    */
    return output;    //Compare the shifts to see if they overlap. If they overlap, return false
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

unsigned int Scheduler::getMaxShifts()
{
    return maxShifts;
}

unsigned int Scheduler::getMinShifts()
{
    return minShifts;
}

unsigned int Scheduler::getMaxConsecutive()
{
    return maxConsecutive;
}

void Scheduler::setMaxShifts(unsigned int input)
{
    maxShifts = input;
}

void Scheduler::setMinShifts(unsigned int input)
{
    minShifts = input;
}

void Scheduler::setMaxConsecutive(unsigned int input)
{
    maxConsecutive = input;
}

void Scheduler::setName(std::string inName)
{
    name = inName;
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

int Scheduler::indexOfShift(Shift *shiftPtr)
{
    int i;
    for(i = 0; (i < shiftNum) && (&shiftList[i] == shiftPtr); i++);
    if(i == shiftNum)
        return -1;
    return i;
}

bool Scheduler::checkMaxConsecutiveDays(Shift *targetShiftPtr, Student *studentPtr)
{
    /*
     * Logic: go through all the shifts. If the last shift is one day behind the current shift, then it's consecutive
     * Logic 2: find index of targetShiftPtr, work backwards (cool since it's sorted)
     * TODO: Doesn't look forward
     */
    Date* lastDatePtr = nullptr;
    Date* currDatePtr = nullptr;
    unsigned int count = 1; //Start at 1 becuase count self
    int i = 0;
    int index;  //Index of that shift
    for(i = 0; i < shiftNum && (targetShiftPtr != &(shiftList[i])); i++);
    index = i;


    for(i = index,lastDatePtr = shiftList[i].date();i>=0;i--)
    {
        if(shiftList[i].student() == studentPtr)
        {
            //Update the currDatePtr if it's the correct student
            currDatePtr = shiftList[i].date();

            //As long as the two are not the same
            if (currDatePtr != lastDatePtr) {
                if (Date::daysBetween(*currDatePtr, *lastDatePtr, false) == 1)
                {
                    //If there is only one day between the two dates, it's consecutive
                    count++;
                    lastDatePtr = currDatePtr;
                    if(count > maxConsecutive)
                        return false;       //If the count has exceeded maxCon, return false
                }
                else
                {
                    break;  //Break if the consecutive chain is broken
                }
            }
        }
    }

    //Lookforward
    for(i = index,lastDatePtr = shiftList[i].date(); i < shiftNum; i++)
    {
        if(shiftList[i].student() == studentPtr)
        {
            currDatePtr = shiftList[i].date();
            if(currDatePtr != lastDatePtr)
            {
                if(Date::daysBetween(*currDatePtr,*lastDatePtr,false)==1)
                {
                    count++;
                    lastDatePtr = currDatePtr;
                    if(count>maxConsecutive)
                        return false;
                }
                else
                {
                    break;
                }
            }

        }
    }

    if(count > maxConsecutive)
        return false;       //If there are more consecutive
    return true;

}

//Files
/*
 *  4B: Name length
 * ??B: Name
 *  4B: # of students
 * ??B: Students
 * 12B: Start Date
 * 12B: End Date
 *  4B: Max shifts
 *  4B: Min Shifts
 *  4B: Max consec
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

    stream.write((char*)&maxShifts,4);                              //<Max shifts>
    stream.write((char*)&minShifts,4);                              //<Min shifts>
    stream.write((char*)&maxConsecutive,4);                         //<Max consecutive>


    stream.write((char*)&shiftNum,4);                               //<# of shifts> NOTE: for some reason, 0x0D is being inserted in the stream
    for(int i = 0; i < shiftNum; i++)
    {
        shiftList[i].streamOutBinary(stream,dateList,studentList);  //<Shifts>
    }
}

void Scheduler::streamInBinary(std::istream &stream)
{
    //Free what it already has
    if(hasMemoryAllocated)
    {
        delete[] shiftList;
        delete[] dateList;
        delete[] studentList;
    }
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

    stream.read((char*)&maxShifts,4);                                                       //<Max shifts>
    stream.read((char*)&minShifts,4);                                                       //<Min shifts>
    stream.read((char*)&maxConsecutive,4);                                                  //<Max consecutive>

    stream.read((char*)&shiftNum,4);                                                        //<# of shifts>
    shiftList = new Shift[shiftNum]();
    for(int i = 0; i<shiftNum; i++)
    {
        //Load the shifts
        shiftList[i].streamInBinary(stream,dateList,studentList);                           //<Shifts>
    }

    disShiftNum = 6;    //Basically worthless after the initialization

    hasMemoryAllocated = true;

    //Update shift counts
    for(int i = 0; i < studentNum; i++)
    {
        updateShiftCount(&studentList[i]);
    }
}
