#include <iostream>
#include <fstream>

#include "Prompt.h"
#include <string>
#include <sstream>
#include "Date.h"
#include "Shift.h"
#include "Day.h"
#include "Student.h"

#include "Scheduler.h"
#include "xlsoutput.h"

#include <xlslib.h>
#include <xlslib/common.h>



//#define LINUX
#ifndef LINUX
#define WINDOWS
#endif

#define MAINCASE 0
#define TESTCASE 1
#define TEST 2
#define DEBUG TESTCASE

using namespace std;
using namespace xlslib_core;

enum State_enum
{
    START,MAIN,SHIFT,STUDENT,EXIT
};

//Global variable fields
State_enum state = START;
string cfgname = "";
Scheduler scheduler;
workbook output;
string filepath = "C:/temp/schedule.xls";

void inline newcfg(Scheduler& schedule)
{
    cout<<"Enter a name for this configuration:: ";
    getline(cin,cfgname);
    cout<<endl;

    //Memory allocation for all the dates

    Date startDate;
    Date endDate;
    string datestring;
    cout<<"Enter the start date (<day #> <month> <year #> or <MMDDYYYY>)\n::";
    getline(cin,datestring);
    cout<<endl;
    while(!startDate.parseDateSet(datestring))
    {
        cout<<"Bad input. Examples: \"25 March 2015\", \"03252015\"\nEnter the start date (<day #> <month> <year #> or <MMDDYYYY>)\n::";
        getline(cin,datestring);
        cout<<endl;
    }
    cout<<"Enter the end date (<day #> <month> <year #> or <MMDDYYYY>)\n::";
    getline(cin,datestring);
    cout<<endl;
    while(!endDate.parseDateSet(datestring))
    {
        cout<<"Bad input. Examples: \"25 March 2015\", \"03252015\"\nEnter the end date (<day #> <month> <year #> or <MMDDYYYY>)\n::";
        getline(cin,datestring);
        cout<<endl;
    }


    Student::setMaxShift(50);


    //Hard coded 6 students
    //Get memory for them
    string studentnames[6];
    for(int i = 0; i<6; i++)
    {
        cout<<"What is student "<<(i+1)<<"'s name?:: ";
        getline(cin,studentnames[i]);
        cout<<endl;
    }

    //Hard coded in
    string shiftnames[5] = {"Shift","Shift","Shift","Crash","Crash"};
    unsigned int shifttimes[5][2] = {{7,16},{15,24},{23,32},{7,16},{15,24}};

    schedule.init(cfgname,startDate,endDate,5,shiftnames,shifttimes,6,studentnames);

    schedule.autoblock();


    //And we're done initializing!
}

void inline loadcfg()
{
    //TODO
}



void inline start(unsigned char input)
{
    switch(input)
    {
        //New configuration
        case 'a':
            newcfg(scheduler);
            state = MAIN;
            break;
        //Load configuration
        case 'b':
            loadcfg();
            state = MAIN;
            break;
        //Exit
        case 'z':
            state = EXIT;
            break;
        default:
            cout<<"Bad input"<<endl;
            state = START;
            break;
    }
}




/*
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

    for(int i = 0; i <schedule.getShiftNum(); i++)
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
        if(i == schedule.getShiftNum() - 1)
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
    for(int i = 0; i < schedule.getStudentNum(); i++)
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
        for(int j = 0; j < schedule.getShiftNum(); j++)
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
*/

void inline mainmenu(unsigned char input)
{
    switch(input)
    {
        case 'a':
            scheduler.autoassign();
            state = MAIN;
            break;
        case 'b':
            state = SHIFT;
            break;
        case 'c':
            state = STUDENT;
            break;
        case 'd':
            scheduleToXLS(scheduler,output);
            output.Dump(filepath);
            state = MAIN;
            break;
        case 'z':
            state = EXIT;
            break;
    }

}

int main()
{
    #if(DEBUG == TESTCASE)
    cout<<"Hello world"<<endl;

    Scheduler scheduler;
    Date dateStart,dateEnd;
    dateStart.setDate(22,Date::SEP,2014);
    dateEnd.setDate(17,Date::OCT,2014);
    Student::setMaxShift(50);
    string shiftnames[5] = {"Shift","Shift","Shift","Crash","Crash"};
    unsigned int shifttimes[5][2] = {{7,16},{15,24},{23,32},{7,16},{15,24}};
    string studentnames[6] = {"A","B","C","D","E","F"};
    scheduler.init("yayaya",dateStart,dateEnd,5,shiftnames,shifttimes,6,studentnames);

    cout<<"Autoassigning..."<<endl;
    cout<<"Wow such autoassign"<<endl;
    scheduler.autoblock();
//    scheduler.shiftList[0].block("Arbitrary");
    if(!scheduler.autoassign())
    {
        cout<<"Too many shifts for students to fulfill with their maximum"<<endl;
    }
    //cout<<scheduler.toString()<<endl;
    for(int i = 0; i <scheduler.getStudentNum(); i++)
    {
        cout << (scheduler.students()[i]).getShiftCount() << endl;
    }
    /*
    for(int i = 0; i < scheduler.getShiftNum(); i++)
    {
        cout<<scheduler.shifts()[i].toString()<<endl;
        for(int j = 0; j<0xFFFFFFF; j++);
    }
     */

    workbook wb;
    scheduleToXLS(scheduler,wb);

    wb.Dump("C:/temp/schedule.xls");


    ofstream fileo;
    fileo.open("C:/temp/schedule.schd");
    scheduler.streamOutBinary(fileo);
    fileo.close();

    ifstream filei;
    filei.open("C:/temp/schedule.schd");
    scheduler.streamInBinary(filei);
    filei.close();

    cout<<scheduler.toString()<<endl;
    cout<<"Load complete"<<endl;


    fileo.open("C:/temp/schedule-copy.schd");
    scheduler.streamOutBinary(fileo);
    fileo.close();

    cout<<"Export 2 complete"<<endl;

    cin.get();

    #elif (DEBUG == TEST)

    Student test1,test2,test3,test4;
    test1.setID(25);
    test1.setName("Chips");
    test2.setID(43);
    test2.setName("Icecream");

    ofstream fileo;

    fileo.open("test.student");
    test1.streamOutBinary(fileo);
    test2.streamOutBinary(fileo);
    fileo.close();

    ifstream filei;

    filei.open("test.student");
    test3.streamInBinary(filei);
    test4.streamInBinary(filei);
    filei.close();

    cout<<"This is test3:\n"<<test3.toString()<<endl;
    cout<<"This is test4:\n"<<test4.toString()<<endl;

    #else
    //Main logic
    while(state != EXIT)
    {
        switch (state)
        {
            case START:
                start(promptStart());
                break;
            case MAIN:
                mainmenu(promptMain());
                break;
            case EXIT:
                break;
        }
    }

    #endif
    cout<<"Goodbye..."<<endl;

    //Free that memory!


    //cin.get();
    return 0;
}


