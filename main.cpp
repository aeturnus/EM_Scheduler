#include <iostream>

#include <String>
#include "Date.h"
#include "Shift.h"
#include "Day.h"
#include "Student.h"

#include <xlslib.h>
#include <xlslib/common.h>


#define XLS_TEST 1

#define DEBUG XLS_TEST

using namespace std;
using namespace xlslib_core;

unsigned char promptNew(void);
unsigned char promptMain(void);
int main() {

    #if(DEBUG == XLS_TEST)
    cout << "Hello, World!" << endl;
    //Test .xls capabilities
    workbook wb;
    worksheet* sh = wb.sheet("sheet");
    wb.Dump("C:/temp/wb.xls");
    #else
    while(promptNew()!='c')
    {

    }
    #endif

    return 0;
}

void prompt()
{
    cout<<"Please select one of the following::"<<endl;
}

unsigned char promptNew()
{
    prompt();
    cout <<"(a) New configuration"<<endl;
    cout <<"(b) Load configuration"<<endl;
    cout <<"(c) Quit"<<endl;
    string out;
    cin>>out;
    return (unsigned char)out[0];
}

unsigned char promptMain()
{
    prompt();
    cout <<"(a) Set number of days"<<endl;
    cout <<"(b) Set number of shifts per day"<<endl;
    cout <<"(c) See my students"<<endl;
    cout <<"(d) "<<endl;
    cout <<"(e) Show parameters"<<endl;
    string out;
    cin>>out;
    return (unsigned char)out[0];
}
