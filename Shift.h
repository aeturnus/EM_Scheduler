//
// Created by Brandon on 7/1/2015.
//

#ifndef EM_SCHEDULER_SHIFT_H
#define EM_SCHEDULER_SHIFT_H

#include "Student.h"
#include <string>

class Shift
{
public:
    void assign(Student* taker);
    void setName(std::string input);
    void block(void);
    void unblock(void);
protected:
private:
    std::string name;   //The shift's name
    bool blocked;       //Determines if the shift is blocked
    Student* student;   //Pointer to the student who has this shift. Equals nullptr if not taken
};


#endif //EM_SCHEDULER_SHIFT_H
