//
// Created by Brandon on 7/17/2015.
//

#ifndef EM_SCHEDULER_SCHEDULER_H
#define EM_SCHEDULER_SCHEDULER_H

#include <string>
#include <vector>
#include "Date.h"
#include "Student.h"
#include "Shift.h"

#include <istream>
#include <ostream>

class Scheduler
{



public:
    Scheduler();
    ~Scheduler();

    enum AssignReturn
    {
        SUCCESS,
        OVERMAX,
        OVERLAP,
        MINTIME,
        CONSEC,
        ISNULL
    };

    //Init will initialize memory allocations
    //Inputs: start: starting date; end: end date; numshift: number of distinct shifts; shiftNames: array of shift names (size corresponds to numshift), shiftTimes:matrix of int pairs, [i][0] is start time and [i][1] is end time. Max of i should be (numshift-1); numstudents is the number of students; studentNames is an array of student names
    void init(std::string name,Date start, Date end, unsigned int numshift, std::string shiftNames[], unsigned int shiftTimes[][2], unsigned int numstudents, std::string studentNames[]);

    bool autoassign();  //Returns true if successful. Returns false if *all* students exceed their max number of shifts

    enum AssignReturn assign(Shift* shiftPtr, Student* stuPtr, bool manual, bool override = false);
    void unassign(Shift* shiftPtr, bool manual);

    void autoblock(void);

    bool resolve(Shift* shiftPtr,std::vector<Student*>* excludevector);

    Shift* getShift(Date day, int shiftID);

    Date* dates(void);
    Shift* shifts(void);
    Student* students(void);

    std::string getName(void);
    unsigned int getDateNum(void);
    unsigned int getShiftNum(void);
    unsigned int getStudentNum(void);
    unsigned int getMaxShifts(void);
    unsigned int getMinShifts(void);
    unsigned int getMaxConsecutive(void);
    void setMaxShifts(unsigned int input);
    void setMinShifts(unsigned int input);
    void setMaxConsecutive(unsigned int input);
    void setName(std::string inName);


    std::string toString(void);

    int indexOfShift(Shift* shiftPtr);

    //Fileout
    void streamOutBinary(std::ostream& stream);
    void streamInBinary(std::istream& stream);
private:
    std::string name;
    unsigned int dateNum;           //Number of dates
    unsigned int shiftNum;          //Number of shifts
    unsigned int studentNum;        //Number of students
    unsigned int disShiftNum;       //Number of distinct shifts

    Date* dateList;         //Pointer to array of dates to be allocated
    Shift* shiftList;       //Pointer to array of shifts to be allocated
    Student* studentList;   //Pointer to array of students to be allocated

    unsigned int maxShifts;
    unsigned int minShifts;
    unsigned int maxConsecutive;
    bool hasMemoryAllocated;

    void updateShiftCount(Student* studentPtr);  //Given a studentID, will update the number of shifts a student has. Call after every assign

    bool checkOverlaps(Shift* shiftPtr, Student* studentPtr);    //Returns true if the pointed shift overlaps with any shifts that the pointed student has

    Student* minStudent(std::vector<Student*>* excludevector = nullptr);      //Returns pointer to student with minimum amount of shifts. Will exclude student provided (nullptr will ignore the exclusion)


    /* checkMinTimeSinceLastShift
     * Checks if the pointed to shift is at least the provided (default 8) hours after the pointed to student's last shift
     * Inputs: pointer to shift to test, pointer to student whose schedule to check, minimum time (in hours) to check
     * Output: bool: true if the shift is valid, false if not
     */

    bool checkMinTimeSinceLastShift(Shift* targetShiftPtr, Student* studentPtr, unsigned int minTime = 8);   //Returns true if it has been the minTime since the last shift, from the beginning of the target shift

    /*
     * SHIFT LIST MUST BE SORTED BY DATE
     */
    bool checkMaxConsecutiveDays(Shift* targetShiftPtr, Student* studentPtr);      //Returns true if there is less than the maximum consecutive days
//    Student* nextEligibleStudent(void); //Returns pointer to the next student that is elligible

    bool studentInVector(Student* stuPtr, std::vector<Student*>* vector);
};


#endif //EM_SCHEDULER_SCHEDULER_H
