//
// Created by 52816 on 2020/1/6.
//

#ifndef BOOKSTORE_USER_H
#define BOOKSTORE_USER_H

#include <iostream>
#include <cstring>
#include <bits/stdc++.h>
using namespace std;

class User {
private:
    bool OK;
    int Perm;
    char Name[23]{},Id[33]{},Passwd[33]{};
public:
    explicit User(const char* ID="",const char* PASSWD="",const int &l=0,const char *NAME="");
    void Delete();
    bool getOK();
    int getPerm();
    void Change_Passwd(const char *P);
    bool Check_Passwd(const char *P);
    bool Check_Id(const char *I);
    void out();
};


#endif //BOOKSTORE_USER_H
