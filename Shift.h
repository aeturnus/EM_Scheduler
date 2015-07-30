//
// Created by Brandon on 7/1/2015.
//

#ifndef EM_SCHEDULER_SHIFT_H
#define EM_SCHEDULER_SHIFT_H

#include "Student.h"
#include "Date.h"
#include <string>
#include <vector>
#include <ostream>
#include <istream>

class Shift
{
public:

    Shift();
    ~Shift();
    Shift& operator=(Shift& rhs);

    void init(std::string shiftName, Date* datePtr,unsigned int start, unsigned int end);
    //Sets the name of the shift
    //In:: input: the visible name of this shift
    void setName(std::string input);

    //Assigns a student this shift
    //In:: taker:pointer to student who takes the shift
    void assign(Student* taker);

    //Unassigns this shift
    void unassign();

    //Blocks off this shift
    //In:: Reason: string to show why it's blocked off
    void block(std::string reason = "");

    //Unblocks this shift
    void unblock(void);

    //getter for manual
    bool isManual(void);
    bool isBlocked(void);

    //Manual setter
    void setManual(bool input);

    Student* student(void);
    Date* date(void);

    int getID(void);

    std::string startTimeString(bool hour24 = false);
    std::string endTimeString(bool hour24 = false);
    std::string shiftTimeString(bool hour24 = false);

    //Set the shift's time. Will return true if it works. Will return false if it fails
    bool setTime(unsigned int start, unsigned int end);
    unsigned int getStart(void);
    unsigned int getEnd(void);

    std::string getBlockReason(void);
    std::string getName(void);

    //Everyone love's a toString!
    std::string toString(void);
    //Static methods

    //This method checks if two shifts have overlapping times
    static bool shiftsOverlap(Shift& shift1, Shift& shift2);

    //This method populates a shift pointer vector with all shifts that correspond to a date pointer
    //Shiftnum is 0 if shiftList is presorted
    static void findShiftsWithDate(std::vector<Shift*>* shiftVectorPtr, Date* date,Shift* shiftList,unsigned int shiftNum, bool presorted = false);

    //A pointer to a date is provided so it can index itself into the date
    void streamOutBinary(std::ostream& stream,Date* dateList, Student *studentList);
    void streamInBinary(std::istream& stream,Date* dateList, Student *studentList);
protected:
private:
    std::string name;           //The shift's name
    std::string blockReason;    //Visible reason for the block
    bool blocked;               //Determines if the shift is blocked
    bool manual;                //Determines if it's a manual change.
    Student* assocStudent;      //Pointer to the student who has this shift. Equals nullptr if not taken
    Date* assocDate;            //Pointer to the date that it is attached to

    unsigned int startTime;     //The beginning time 0 is 12AM
    unsigned int endTime;       //The end time. Can extend to the next day if at least 24
};


#endif //EM_SCHEDULER_SHIFT_H
