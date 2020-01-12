//
// Created by 52816 on 2020/1/7.
//

#include "Book.h"

Book::Book(const char* s1,const char* s2,const char* s3,const char* s4,const int &s5,const double &s6) {
    memset(this,0,sizeof(*this));
    Price=s6;
    Quti=s5;
    OK=true;
    strcpy(ISBN,s1);
    strcpy(Name,s2);
    strcpy(Author,s3);
    strcpy(Keyword,s4);
}

void Book::Delete() {
    OK=false;
}

bool Book::getOK() {
    return OK;
}

double Book::getPrice() {
    return Price;
}

void Book::show() {
    printf("%s\t%s\t%s\t%s\t%.2f\t%d本\n",ISBN,Name,Author,Keyword,Price,Quti);
}

string Book::getISBN() {
    return ISBN;
}

bool Book::Buy(const int &s) {
    if(s<=Quti){
        Quti-=s;
        return true;
    }
    return false;
}

void Book::ModifyISBN(const char *s) {
    strcpy(ISBN,s);
}

void Book::ModifyPrice(const char *s) {
    sscanf(s,"%lf",&Price);
}

void Book::ModifyName(const char *s) {
    strcpy(Name,s);
}

void Book::ModifyAuthor(const char *s) {
    strcpy(Author,s);
}

void Book::ModifyKeyword(const char *s) {
    strcpy(Keyword,s);
}

void Book::AddBook(const char*s) {
    int add;
    sscanf(s,"%d",&add);
    Quti+=add;
}

bool Book::Check_ISBN(const char* s) {
    return strcmp(ISBN,s)==0;
}

bool Book::Check_Name(const char* s) {
    return strcmp(Name,s)==0;
}

bool Book::Check_Author(const char *s) {
    return strcmp(Author,s)==0;
}

bool Book::Check_Keyword(const char *s) {
    for(int i=0;i<strlen(Keyword);++i){
        int pos=i+1;
        while(pos<strlen(Keyword)&&Keyword[pos]!='|') pos++;
        if(pos-i==strlen(s)) {
            bool flag = true;
            for (int j = 0; j < strlen(s); ++j) {
                if (Keyword[i + j] != s[j]) {
                    flag = false;
                    break;
                }
            }
            if(flag) return true;
        }
        i=pos;
    }
    return false;
}
