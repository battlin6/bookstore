//
// Created by 52816 on 2020/1/7.
//

#ifndef BOOKSTORE_BOOK_H
#define BOOKSTORE_BOOK_H

#include <iostream>
#include <cstring>
#include <bits/stdc++.h>
using namespace std;

class Book {
private:
    bool OK;
    double Price;
    int Quti;
    char ISBN[23],Name[63],Author[63],Keyword[63];
public:
    explicit Book(const char* s1="",const char* s2="",const char* s3="",const char* s4="",const int &s5=0,const double &s6=0);
    void Delete();
    bool getOK();
    double getPrice();
    string getISBN();
    void ModifyISBN(const char *s);
    void ModifyName(const char *s);
    void ModifyAuthor(const char *s);
    void ModifyKeyword(const char *s);
    void ModifyPrice(const char *s);
    void show();
    bool Buy(const int &s);
    void AddBook(const char*s);
    bool Check_ISBN(const char *s);
    bool Check_Name(const char *s);
    bool Check_Author(const char *s);
    bool Check_Keyword(const char *s);
};


#endif //BOOKSTORE_BOOK_H
