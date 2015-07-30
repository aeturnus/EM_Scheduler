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
    Student();
    ~Student();

    //Call these when assigning shifts. Returns a true if successful
    //bool assign(void);
    //bool unassign(void);

    void setName(std::string inName);
    std::string getName(void);

    void setID(unsigned int input);
    unsigned int getID(void);

    int getShiftCount(void);
    void setShiftCount(int count);

    std::string toString(void);

    void streamOutBinary(std::ostream& stream);
    void streamInBinary(std::istream& stream);
protected:
private:
    std::string name;       //Student name
    int shiftCount;    //Student shift count
    std::uint16_t color;    //Student color on sheet (Do I need this?)
    unsigned int id;
};


#endif //EM_SCHEDULER_STUDENT_H
