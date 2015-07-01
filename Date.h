//
// Created by Brandon on 7/1/2015.
//

#ifndef EM_SCHEDULER_DATE_H
#define EM_SCHEDULER_DATE_H

#include <cstdint>

class Date
{
public:


    Date();
    ~Date();

    //Function to set the time
    void setDate(int yearIn,int monthIn,int dayIn);
    std::uint8_t dayOfWeek(void);

    //Constants
protected:


private:
    std::int32_t year;      //Current year since 2000
    std::uint8_t month;     //Current month
    std::uint8_t day;       //Current day


};


#endif //EM_SCHEDULER_DATE_H
