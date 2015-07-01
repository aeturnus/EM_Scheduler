//
// Created by Brandon on 7/1/2015.
//

#ifndef EM_SCHEDULER_DAY_H
#define EM_SCHEDULER_DAY_H

#include <vector>
#include <string>

#include "Shift.h"
#include "Date.h"

class Day
{
public:
    Day();
    ~Day();
protected:
private:
    std::vector<Date> vShift;

};


#endif //EM_SCHEDULER_DAY_H
