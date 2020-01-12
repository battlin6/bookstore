//
// Created by 52816 on 2020/1/6.
//

#include "User.h"
#include <iostream>
using namespace std;


User::User(const char* ID,const char* PASSWD,const int &l,const char *NAME) {
    memset(this,0,sizeof(*this));
    OK=true;
    Perm=l;
    strcpy(Id,ID);
    strcpy(Passwd,PASSWD);
    strcpy(Name,NAME);
}

void User::Delete() {
    OK=false;
}

bool User::getOK() {
    return OK;
}

int User::getPerm() {
    return Perm;
}

void User::Change_Passwd(const char *P) {
    strcpy(Passwd,P);
}

bool User::Check_Passwd(const char *P) {
    return strcmp(Passwd,P)==0;
}

bool User::Check_Id(const char *I) {
    return strcmp(Id,I)==0;
}

void User::out()
{
    cout<<OK<<' '<<Perm<<' '<<Id<<' '<<Passwd<<' '<<Name<<endl;
}
