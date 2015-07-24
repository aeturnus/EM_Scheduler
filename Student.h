//
// Created by Brandon on 7/1/2015.
//

#ifndef EM_SCHEDULER_STUDENT_H
#define EM_SCHEDULER_STUDENT_H

#include <cstdint>
#include <string>
#include <ostream>
#include <istream>

class Student
{
public:
//    static const int MAX_SHIFTS = 50;
//    static const int MIN_SHIFTS = 12;
    static int MAX_SHIFTS;
    static int MIN_SHIFTS;

    Student();
    ~Student();

    //Call these when assigning shifts. Returns a true if successful
    bool assign(void);
    bool unassign(void);

    void setName(std::string inName);
    std::string getName(void);

    void setID(unsigned int input);
    unsigned int getID(void);

    unsigned int getShiftCount(void);
    void setShiftCount(unsigned int count);

    std::string toString(void);

    static void setMinShift(int min);
    static void setMaxShift(int max);

    void streamOutBinary(std::ostream& stream);
    void streamInBinary(std::istream& stream);
protected:
private:
    std::string name;       //Student name
    unsigned int shiftCount;    //Student shift count
    std::uint16_t color;    //Student color on sheet (Do I need this?)
    unsigned int id;
};


#endif //EM_SCHEDULER_STUDENT_H
