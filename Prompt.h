//
// Created by Brandon on 7/15/2015.
//

#ifndef EM_SCHEDULER_PROMPT_H
#define EM_SCHEDULER_PROMPT_H
//Yeah, I know this is bad practice, but I literally want the preprocessor to stuff this in front of main. I would've put this in main, but it seems cluttered
#include <string>
#include <iostream>
using namespace std;
void prompt(void)
{
    cout<<endl;
    cout<<"Please select one of the following::"<<endl;
}
void promptSelect(void)
{
    cout<<"Please select one of the above:: ";
}

//Get input
char getin(void)
{
    char out = (char)cin.get();
    cin.clear();
    cin.ignore();
    return out;
}

char promptStart(void)
{
    cout<<"Welcome to the Seton Medical Student Shift Manager's \"Terminal Mode\"\n"<<endl;
//    prompt();
    cout<<endl;
    cout<<"<a> New configuration"<<endl;
    cout<<"<b> Load configuration"<<endl;
    cout<<"<z> Exit"<<endl;
    promptSelect();
    return getin();
}

char promptMain(void)
{
//    prompt();
    cout<<endl;
    cout<<"<a> Autoassign shifts with given parameters (takes manual assignments into account"<<endl;
    cout<<"<b> Shift management"<<endl;
    cout<<"<c> Student management"<<endl;
    cout<<"<d> Export .xls"<<endl;
    cout<<"<z> Exit"<<endl;
    promptSelect();
    return getin();
}

char promptShift(void)
{
    cout<<endl;
    cout<<"<a> Manually assign a student to a shift"<<endl;
    cout<<"<b> Manually unassign a shift"<<endl;
    cout<<"<c> Manually block a shift"<<endl;
    cout<<"<d> Manually unblock a shift"<<endl;
    cout<<"<e> Return automatic control to shift"<<endl;
    cout<<"<z> Back"<<endl;
    promptSelect();
    return getin();
}


#endif //EM_SCHEDULER_PROMPT_H
