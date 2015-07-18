//
// Created by Brandon on 7/17/2015.
//

#ifndef EM_SCHEDULER_SCHEDULER_H
#define EM_SCHEDULER_SCHEDULER_H

#include <string>
#include "Date.h"
#include "Student.h"
#include "Shift.h"

class Scheduler
{
public:
    Scheduler();
    ~Scheduler();

    //Init will initialize memory allocations
    //Inputs: start: starting date; end: end date; numshift: number of distinct shifts; shiftNames: array of shift names (size corresponds to numshift), shiftTimes:matrix of int pairs, [i][0] is start time and [i][1] is end time. Max of i should be (numshift-1); numstudents is the number of students; studentNames is an array of student names
    void init(Date start, Date end, unsigned int numshift, std::string shiftNames[], unsigned int shiftTimes[][2], unsigned int numstudents, std::string studentNames[]);

    bool autoassign();  //Returns true if successful. Returns false if *all* students exceed their max number of shifts

    bool assign(Shift* shiftPtr, Student* stuPtr);

    Shift* getShift(Date day, int shiftID);


    Date* dateList;         //Pointer to array of dates to be allocated
    Shift* shiftList;       //Pointer to array of shifts to be allocated
    Student* studentList;   //Pointer to array of students to be allocated


    std::string toString(void);
private:
    unsigned int dateNum;           //Number of dates
    unsigned int shiftNum;          //Number of shifts
    unsigned int studentNum;        //Number of students
    unsigned int disShiftNum;          //Number of distinct shifts



    void updateShiftCount(Student* studentPtr);  //Given a studentID, will update the number of shifts a student has. Call after every assign

    bool checkOverlaps(Shift* shiftPtr, Student* studentPtr);    //Returns true if the pointed shift overlaps with any shifts that the pointed student has

    Student* minStudent(Student* exclude = nullptr);      //Returns pointer to student with minimum amount of shifts. Will exclude student provided (nullptr will ignore the exclusion)

//    Student* nextEligibleStudent(void); //Returns pointer to the next student that is elligible
};


#endif //EM_SCHEDULER_SCHEDULER_H
