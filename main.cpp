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



void inline newcfg()
{
    cout<<"Enter a name for this configuration:: ";
    getline(cin,cfgname);
    cout<<endl;

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
    unsigned int num = Date::daysBetween(startDate,endDate,true);   //Include the end date
    datelist = new Date[num]();    //Create an array of dates
    for(unsigned int i = 0; i<num; i++)
    {
        startDate.setDate(startDate.getNumDay()+(i==0?0:1),startDate.getNumMonth(),startDate.getNumYear()); //Add additional days from start
        datelist[i] = startDate;
    }
    cout<<datelist[0].toString()<<endl;
    cout<<datelist[num-1].toString()<<endl;

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
            break;
        //Load configuration
        case 'b':
            loadcfg();
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
                break;
            case EXIT:
                break;
        }
    }

    #endif
    cout<<"Goodbye..."<<endl;
    //cin.get();
    return 0;
}
