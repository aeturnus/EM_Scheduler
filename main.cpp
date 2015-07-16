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
    START,MAIN,EXIT
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
        cout<<"What is student "<<i<<"'s name?:: ";
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


void inline mainmenu(unsigned char input)
{
    switch(input)
    {
        case 'a':
            break;
        case 'b':
            break;
        case 'c':
            break;
        case 'z':
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
