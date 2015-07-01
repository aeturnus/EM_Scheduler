#include <iostream>

#include <String>
#include "Date.h"
#include "Shift.h"
#include "Day.h"
#include "Student.h"
#define DEBUG 0

using namespace std;


unsigned char promptNew(void);
unsigned char promptMain(void);
int main() {

    #if(DEBUG == 1)
    cout << "Hello, World!" << endl;

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
