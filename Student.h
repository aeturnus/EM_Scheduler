//
// Created by Brandon on 7/1/2015.
//

#ifndef EM_SCHEDULER_STUDENT_H
#define EM_SCHEDULER_STUDENT_H

#include <cstdint>
#include <string>

class Student
{
public:
    static const int MAX_SHIFTS = 14;
    static const int MIN_SHIFTS = 12;

    Student(std::string inName="");
    ~Student();

    //Call these when assigning shifts. Returns a true if successful
    bool assign(void);
    bool unassign(void);

    void setName(std::string inName);
    std::string getName(void);

    std::string toString(void);
protected:
private:
    //std::string name;       //Student name
    unsigned int shiftCount;    //Student shift count
    std::uint16_t color;    //Student color on sheet (Do I need this?)
    std::string name;
};


#endif //EM_SCHEDULER_STUDENT_H
