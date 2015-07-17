#include <iostream>


#include "Prompt.h"

#include <string>
#include <sstream>
#include "Date.h"
#include "Shift.h"
#include "Day.h"
#include "Student.h"

#include <xlslib.h>
#include <xlslib/common.h>


//#define LINUX
#ifndef LINUX
#define WINDOWS
#endif

#define XLS_TEST 1
#define TEST 2
#define DEBUG 0

using namespace std;
using namespace xlslib_core;

enum State_enum
{
    START,MAIN,SHIFT,STUDENT,EXIT
};

//Global variable fields
State_enum state = START;
string cfgname = "";

Date* datelist;
unsigned int numdates = 0;

Shift* shiftlist;
unsigned int numshifts = 0;

Student* studentlist;
unsigned int numstudents = 6;

//Hard coded limit of 5 shifts
unsigned int numdisshifts = 5;
string shiftnames[5] = {"7am-4pm","3pm-12am","11pm-8am","Crash:7am-4pm","Crash:3pm-12am"};   //Refer to by ID

void inline newcfg()
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
    numdates = Date::daysBetween(startDate,endDate,true);   //Include the end date
    datelist = new Date[numdates]();    //Create an array of dates
    for(int i = 0; i<numdates; i++)
    {
        startDate.setDate(startDate.getNumDay()+(i==0?0:1),startDate.getNumMonth(),startDate.getNumYear()); //Add additional days from start
        datelist[i] = startDate;
    }

    //Get memory for all the shifts
    numshifts = numdates * numdisshifts;
    shiftlist = new Shift[numshifts]();
    //Link the shifts to dates
    for(int i = 0; i<numdates; i++)                                 //For every date
    {
        for(int j = 0; j<numdisshifts; j++)                            //For each particular shift
        {
            shiftlist[i*numdisshifts + j].init(shiftnames[j],&datelist[i]);
        }
    }

    //Auto block certain shifts
    Date* shiftdate;
    for(int i = 0; i<numshifts; i++)
    {
        shiftdate = shiftlist[i].date();
        //Thursday (4) block
        if(shiftdate->getNumDayOfWeek() == 4)
            //Not the third Thursday
            if(shiftdate->weekdayOfMonth() != 3)
                //Not the 7am-4pm or crash version shifts
                if(shiftlist[i].getID() == 0 || shiftlist[i].getID() == 3)
                    shiftlist[i].block("EM conference 7am-2pm");
        //Wednesday (3) block
        if(shiftdate->getNumDayOfWeek() == 3)
            //Not the third Wednesday
            if(shiftdate->weekdayOfMonth() != 3)
                //Not the 11pm-8am or crash version shifts
                if(shiftlist[i].getID() == 2 || shiftlist[i].getID() == 2)
                    shiftlist[i].block("EM conference 11pm-8am");
    }
    //TODO: Block off the last day's non day shifts

    /*
     * Debug printout
    for(int i = 0; i< numshifts;i++)
    {
        cout<<shiftlist[i].toString()<<endl;
    }
     */


    //Hard coded 6 students
    //Get memory for them
    studentlist = new Student[numstudents]();
    string stuname="";
    for(int i = 0; i<numstudents; i++)
    {
        cout<<"What is student "<<(i+1)<<"'s name?:: ";
        getline(cin,stuname);
        cout<<endl;
        studentlist[i].setID(i);
        studentlist[i].setName(stuname);
    }
    for(int i = 0; i<numstudents; i++)
    {
        cout<<studentlist[i].toString()<<endl;
    }

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
            newcfg();
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


//Returns the student with the least amount of shifts
unsigned int minStudent(void)
{
    unsigned int lowestID = 0;
    for(int i = 0; i < numstudents; i++)
    {
        if(studentlist[i].getShiftCount() < studentlist[lowestID].getShiftCount())
        {
            lowestID = i;
        }
    }
    return lowestID;
}

//Enumerate the number of shifts a particular student has.
unsigned int countShifts(Student* stuPtr)
{
    unsigned int output = 0;
    for(int i = 0; i<numshifts; i++)
    {
        //Check for each shift to see if it's associated with a particular student
        if(shiftlist[i].student() == stuPtr)
        {
            output++;
        }
    }
    return output;
}

void autoassign(void)
{
    //Scan the entire list of shifts
    //Prehensive shift enumeration of all the students
    for(int i = 0; i<numstudents; i++)
    {
        studentlist[i].setShiftCount(countShifts(&studentlist[i]));
    }

    for(int i = 0; i<numshifts; i++)
    {
        //If the particular shift doesn't have a manual set
        if(!shiftlist[i].isManual())
        {
            //If it fails, it's because it's blocked
            shiftlist[i].assign(&studentlist[minStudent()]);
            //cout<<"Shift!!!"<<endl;
            //cout<<shiftlist[i].toString()<<endl;
        }
        else
        {
            //cout<<"Shift is manual"<<endl;
        }
        (shiftlist[i].student())->setShiftCount(countShifts(shiftlist[i].student()));   //Update shift count
    }
}

string studentScheduleOut(unsigned int stuID)
{
    unsigned int num = 0;   //Count the shift number
    stringstream sstream;

    sstream<<studentlist[stuID].getName()<<"\n";
    for(int i = 0; i < numshifts;i++)
    {
        if(&studentlist[stuID] == shiftlist[i].student())
        {
            //Print the shift they have
            num++;
            sstream<<dec<<"(" <<num<<") "<<shiftlist[i].toString()<<endl;
        }
    }

    return sstream.str();
}

string shiftsOut(void)
{
    stringstream sstream;

    for(int i = 0; i<numshifts; i++)
    {
        sstream<<shiftlist[i].toString()<<endl;
    }

    return sstream.str();
}

void inline mainmenu(unsigned char input)
{
    switch(input)
    {
        case 'a':
            autoassign();
//            cout<<studentScheduleOut(0);
            cout<<shiftsOut();
            state = MAIN;
            break;
        case 'b':
            state = SHIFT;
            break;
        case 'c':
            state = STUDENT;
            break;
        case 'z':
            state = EXIT;
            break;
    }

}
int main()
{
    #if(DEBUG == XLS_TEST)
    cout << "Hello, World!" << endl;
    string yoyo;
    cin>>yoyo;
    //Test .xls capabilities
    workbook wb;
    worksheet* sh = wb.sheet(yoyo);
    sh->number(0,0,3);
    sh->number(1,0,5);
    sh->label(0,1,"String, nigga");
    #ifdef WINDOWS
    wb.Dump("C:/temp/wb.xls");
    #else
    wb.Dump("/home/brandon/wb.xls");
    #endif
    cout <<"Dumped"<<endl;
    #elif(DEBUG == TEST)
    Date test;
    test.setDate(29,11,2015);
    cout<<test.toString()<<"\nWeekday of Month: "<<test.weekdayOfMonth()<<endl;
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

    delete[] datelist;
    delete[] studentlist;
    delete[] shiftlist;


    //cin.get();
    return 0;
}
