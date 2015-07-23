//
// Created by Brandon on 7/21/2015.
//
#include "xlsoutput.h"

#include <string>
#include <sstream>
#include "Date.h"
#include "Shift.h"
#include "Day.h"
#include "Student.h"

#include "Scheduler.h"

#include <xlslib.h>
#include <xlslib/common.h>

using namespace std;
using namespace xlslib_core;

void scheduleToXLS(Scheduler& schedule, workbook& wb)
{
    worksheet* sh = wb.sheet("Master Schedule");

    xf_t* format = wb.xformat();
    format->SetWrap(true);
    format->SetVAlign(VALIGN_TOP);


    //Label them
    sh->defaultColwidth(25);
    sh->label(0,0,"Sunday");
    sh->label(0,1,"Monday");
    sh->label(0,2,"Tuesday");
    sh->label(0,3,"Wednesday");
    sh->label(0,4,"Thursday");
    sh->label(0,5,"Friday");
    sh->label(0,6,"Saturday");


    //Do the dates
    unsigned int rowbase = 1;
    unsigned int row = 0;
    unsigned int col;
    Date* datePtr = nullptr;
    Shift* shiftPtr;
    stringstream sstream;

    for(int i = 0; i <(int)schedule.getShiftNum(); i++)
    {
        //For each shift, check if it's the same date as before
        if(datePtr != schedule.shifts()[i].date() || datePtr == nullptr)
        {
            //Actually put the string in
            if(datePtr != nullptr)
            {
                sh->label(row+rowbase, col, sstream.str(), format);   //Skip this if it's the first date
                if (datePtr->getNumDayOfWeek() == 6)
                    row++;
            }

            //Begin a new day in the calendar
            sstream.str("");
            sstream<<dec;
            datePtr = schedule.shifts()[i].date();
            //On a new day, put the date onto the spreadsheet
            sstream<<datePtr->getMonth()<<" "<<datePtr->getNumDay()<<"\n\n";



        }
        shiftPtr = &schedule.shifts()[i];   //Grab the pointer
        col = datePtr->getNumDayOfWeek();
        if(shiftPtr->isBlocked())
        {
            sstream<<shiftPtr->getBlockReason()<<"\n\n\n";
        }
        else
        {
            //Not blocked
            //cout<<(shiftPtr->toString())<<endl;

            sstream<<shiftPtr->getName()<<" "<<shiftPtr->shiftTimeString()<<"::\n    ";
            sstream<<((shiftPtr->student()==nullptr)?"Open":(shiftPtr->student())->getName());
            sstream<<"\n\n";
        }


        //Special last case
        if(i == (int)schedule.getShiftNum() - 1)
        {
            sh->label(row+rowbase, col, sstream.str(), format);   //If it's the last shift, make sure to write to the sheet
            sstream.clear();

        }

        //if(datePtr->getNumDayOfWeek() == 6)
        //    row++;
    }
    //End dates

    //Create a shift schedule for each student

    //Name each sheet
    //sstream.str("");
    worksheet* studentSheet[schedule.getStudentNum()];
    worksheet* sheet;
    worksheet* combsheet = wb.sheet("Per Student Schedules");
    combsheet->defaultColwidth(25);

    //Fancy formatting: alternate colors
    xf_t* evenformat = wb.xformat();
    xf_t* oddformat = wb.xformat();

    evenformat->SetWrap(true);
    evenformat->SetVAlign(VALIGN_TOP);
    evenformat->SetBorderStyle(BORDER_LEFT,BORDER_DOUBLE);
    evenformat->SetBorderStyle(BORDER_BOTTOM,BORDER_THIN);
    evenformat->SetBorderStyle(BORDER_TOP,BORDER_THIN);
    evenformat->SetBorderStyle(BORDER_RIGHT,BORDER_DOUBLE);
//    evenformat->SetFillBGColor(COLOR_CODE_GRAY25);
    evenformat->SetFillFGColor(COLOR_CODE_BLUE);

    oddformat->SetWrap(true);
    oddformat->SetVAlign(VALIGN_TOP);
    oddformat->SetBorderStyle(BORDER_LEFT,BORDER_DOUBLE);
    oddformat->SetBorderStyle(BORDER_BOTTOM,BORDER_THIN);
    oddformat->SetBorderStyle(BORDER_TOP,BORDER_THIN);
    oddformat->SetBorderStyle(BORDER_RIGHT,BORDER_DOUBLE);
    oddformat->SetFillFGColor(COLOR_CODE_WHITE);
    Student* student;

    //stringstream sstream;
    for(int i = 0; i < (int)schedule.getStudentNum(); i++)
    {
        row = 0;
        sheet = studentSheet[i];
        student= &(schedule.students()[i]);

        sheet = wb.sheet(schedule.students()[i].getName());
        sheet->defaultColwidth(25);
        sheet->label(row,0,student->getName());
        if(i%2 == 0)
            combsheet->label(row,i,student->getName(),evenformat);
        else
            combsheet->label(row,i,student->getName(),oddformat);
        row++;
        for(int j = 0; j < (int)schedule.getShiftNum(); j++)
        {
            if(schedule.shifts()[j].student() == student)
            {
                sstream.str("");
                sstream<<schedule.shifts()[j].date()->toString()<<"\n"<<schedule.shifts()[j].getName()<<(schedule.shifts()[j].getName().size()!=0?" ":"")<<schedule.shifts()[j].shiftTimeString(false)<<"\n";
                sheet->label(row,0,sstream.str(),format);
                if(i%2 == 0)
                    combsheet->label(row,i,sstream.str(),evenformat);
                else
                    combsheet->label(row,i,sstream.str(),oddformat);
                row++;
            }
        }
    }

}

