//
// Created by Brandon on 7/15/2015.
//

#ifndef EM_SCHEDULER_PROMPT_H
#define EM_SCHEDULER_PROMPT_H
//Yeah, I know this is bad practice, but I literally want the preprocessor to stuff this in front of main. I would've put this in maain, but it seems cluttered
#include <string>
#include <iostream>
using namespace std;
void prompt(void)
{
    cout<<endl;
    cout<<"Please select one of the following:: ";
}
void promptSelect(void)
{
    cout<<"Please select one of the above:: ";
}

char getin(void)
{
    char out = (char)cin.get();
    cin.clear();
    cin.ignore();
    return out;
}

char promptStart(void)
{
    prompt();
    cout<<endl;
    cout<<"<a> New configuration"<<endl;
    cout<<"<b> Load configuration"<<endl;
    cout<<"<z> Exit"<<endl;
    promptSelect();
    return getin();
}


#endif //EM_SCHEDULER_PROMPT_H
