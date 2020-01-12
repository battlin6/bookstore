#include <iostream>
#include "User.h"
#include "Book.h"
#include <fstream>
#include <bits/stdc++.h>
using namespace std;

#define debug cout<<123<<endl;
const int Hash=3;  //just 103 files I dont want too many
//but now just for easy I simplify it
int getHash(const string &s); //get hash number

int DeleteUser=0,NowPage=0,NowPos=0;
Book NowBook;

enum Info{ISBN,NAME,AUTHOR,PRICE,KEYWORD,ERROR};

void Pre_work(); //check if create a preparation and do preparations
void Create(); //just create all
void Create_User();
void Create_Book();
void Create_Selete();
void Create_Bill();
int todo(const string &s); //judgy the command
int getnowPerm(); //gu ming si yi
void Login(const string &Id,const string &Passwd);
void ResetLogin(User U);
void Delete(const string &s);
bool OKId(const string &s);  //judgy if the Id has appeared
void AddUser(User U);
void ChangePasswdroot(const string& Id,const string& Passwd);
void ChangePasswd(const string &Id,const string &OldPasswd,const string &NewPasswd);
/*void display_userlist()
{
    cout<<"!!!!"<<endl;
    fstream io("User",ios::binary|ios::in|ios::out);
    User tmp;
    io.read((char*)(&tmp),sizeof(tmp));
    while(!io.eof())
    {
        tmp.out();
        io.read((char*)(&tmp),sizeof(tmp));
    }
    io.close();
}
 */   //cout users
void Select(const string &s); //change the select Book
void DoSelect(); //update the nowSelect file
Info getInfo(const string &s); //get the Info from a string
void NonSelect(); //do the things opposite the DoSeclect   that is download the nowSelect
void Modify1(const int &cas,const string &s);  //just Modify1
void Modify2(const string &s);  //just Modify2
void UpdateBook(Book B); //update rudely
bool OKISBN(const string &s);  //judgy if the Id has appeared
void ImportBook(const string &s);
void OutportBook(double);
void ShowFinance();
void ShowFinance(const string& s);
int gettotTime();
struct pay{
    double moneyin,moneyout;
};
pay getFinance(const int &k);
void ShowAll();
struct Bookinfo{
    string ISBN;
    int page,pos;
};
bool cmp(const Bookinfo &a,const Bookinfo &b){
    return a.ISBN<b.ISBN;
}
void ShowBook(const int &page,const int &pos);
void Show1(const int &cas,const string &s);
void Show2(const string &s);
void buy(const string &_ISBN,const string &_Quti);

int main() {
    //Book B("asdasd","asdasd","asdada","asdad",123,0.1);
    //B.show();
    //ofstream out("pre",ios::binary|ios::out);
    //out << "本" << endl;
    //out.close();
    Pre_work();
    while(true){
        string command;
        getline(cin,command);
        int result=todo(command);
        //display_userlist();
        if(result==1)
            continue;
        else if(result==2)
            break;
        else if(result==3){
            printf("Invalid\n");
            continue;
        }
    }
}

void Create(){
    Create_User();
    Create_Book();
    Create_Selete();
    Create_Bill();
}
void Pre_work(){


    Create(); //just for debug;


    fstream io;
    io.open("pre",ios::in|ios::out|ios::binary);
    if(!io){
        ofstream out;
        out.open("pre",ios::trunc|ios::binary); //clear the pre
        out.close();
    }
    io.close();
}   //todo:a debugger
void Create_User() {
    ofstream out;
    User root("root","sjtu",7,"root");

    out.open("nowUser",ios::binary|ios::out);
    if(!out){cerr<<"Create_User error";exit(0);};
    out.write(reinterpret_cast<char*>(&root),sizeof(root));
    out.close();

    out.open("User",ios::binary|ios::out);
    if(!out){cerr<<"Create_User error";exit(0);};
    out.write(reinterpret_cast<char*>(&root),sizeof(root));
    out.close();
}
void Create_Book() {
    for(int i=0;i<Hash;++i){
        ofstream out("Books"+to_string(i),ios::binary|ios::out);
        if(!out){cerr<<"Create_Book error"<<endl;exit(0);}
        out.close();
    }
}
void Create_Selete() {
    ofstream out("nowSelect",ios::binary|ios::out);
    if(!out){cerr<<"Create_Select error"<<endl; exit(0);}
    NowPage=0,NowPos=-1;
    out.seekp(0);
    out.write(reinterpret_cast<char*>(&NowPage),sizeof(NowPage));
    out.write(reinterpret_cast<char*>(&NowPos),sizeof(NowPos));
    out.write(reinterpret_cast<char*>(&NowBook),sizeof(NowBook));
    out.close();
}
void Create_Bill() {
    ofstream out("Bill",ios::binary|ios::out);
    int tmp1=0;  //the times is 0
    double tmp2=0; //the first in and out is double(0)
    out.seekp(0);
    out.write(reinterpret_cast<char*>(&tmp1),sizeof(tmp1));
    out.write(reinterpret_cast<char*>(&tmp2),sizeof(tmp2));
    out.write(reinterpret_cast<char*>(&tmp2),sizeof(tmp2));
    out.close();
}
int getHash(const string &s) {
    int tmp=0;
    for(unsigned int i=0;i<s.size();++i){
        tmp=tmp*10+int(s[i]-'0');
        tmp%=Hash;
    }
    return tmp;
}
int todo(const string &s) {
    vector<string> test;
    //parsing
    for(int i=0;i<s.size();){
        if(s[i]==' ') {
            i++;
            continue;
        }
        if(s[i]=='"'){
            int pos=i+1;
            while(pos<s.size()){
                if(s[pos]=='"')
                    break;
                pos++;
            }
            test.push_back(s.substr(i+1,pos-(i+1)));
            i=pos+1;
        } else{
            int pos=i+1;
            while(s[pos]!=' '&&s[pos]!='"'&&pos<s.size()){
                pos++;
            }
            test.push_back(s.substr(i,pos-i));
            i=pos;
        }
    }
    //for(int i=0;i<test.size();++i) cout<<test[i]<<endl;
    int length=test.size();
    if(length==0) return 1;
    if(test[0]=="exit"&&length==1) return 2;
    //User
    if(length==2&&test[0]=="su") {Login(test[1], ""); return 1;}
    if(length==3&&test[0]=="su") {Login(test[1],test[2]); return 1;}
    if(length==1&&test[0]=="logout"){
        if(getnowPerm()!=1&&getnowPerm()!=3&&getnowPerm()!=7)
            return 3;
        User U;
        ResetLogin(U);
        return 1;
    }
    if(length==2&&test[0]=="delete"){
        if(getnowPerm()==7){
            Delete(test[1]);
            return 1;
        }
        return 3;
    }
    if(length==4&&test[0]=="register"){
        //cerr<<23<<endl;
        if(test[1].size()>30||test[2].size()>30||test[3].size()>30||OKId(test[1]))
            return 3;
        User U(test[1].c_str(),test[2].c_str(),1,test[3].c_str());
        AddUser(U);
        return 1;
    }
    if(length==5&&test[0]=="useradd"){
        if(test[1].size()>30||test[2].size()>30||test[4].size()>30||OKId(test[1])||(test[3]!="7"&&test[3]!="3"&&test[3]!="1")||int(test[3][0]-'0')>=getnowPerm())
            return 3;
        User U(test[1].c_str(),test[2].c_str(),int (test[3][0]-'0'),test[4].c_str());
        AddUser(U);
        return 1;
    }
    if(length==3&&test[0]=="passwd"){
        if(test[1].size()>30||test[2].size()>30||getnowPerm()!=7)
            return 3;
        ChangePasswdroot(test[1],test[2]);
        return 1;
    }
    if(length==4&&test[0]=="passwd"){
        if(test[1].size()>30||test[2].size()>30||test[3].size()>30||(getnowPerm()!=7&&getnowPerm()!=3&&getnowPerm()!=1))
            return 3;
        ChangePasswd(test[1],test[2],test[3]);
        return 1;
    }
    if(length==2&&test[0]=="select"){
        if(getnowPerm()!=3&&getnowPerm()!=7)
            return 3;
        Select(test[1]);
        return 1;
    }
    if(length>=2&&test[0]=="modify"){
        if(getnowPerm()!=3&&getnowPerm()!=7)
            return 3;
        bool vis[31];
        memset(vis,0,sizeof(vis));
        for(int i=1;i<length;++i) {
            Info info = getInfo(test[i]);
            if(info==ERROR||vis[info]) return 3;
            vis[info]=true;
            if(info==NAME||info==AUTHOR||info==KEYWORD) i++;
            //cout<<int(info)<<endl;
        }
        //debug
        //cout<<123<<endl;
        NonSelect();
        if(NowPos==-1&&NowPage==0) return 3;
        for(int i=1;i<length;++i) {
            if(getInfo(test[i])==NAME||getInfo(test[i])==AUTHOR||getInfo(test[i])==KEYWORD){
                Modify1(int(getInfo(test[i])),test[i+1]);
                i++;
            } else Modify2(test[i]);
        }
        DoSelect();
        return 1;
    }
    if(length==3&&test[0]=="import"){
        NonSelect();
        if(NowPos==-1&&NowPage==0) return 3;
        ImportBook(test[2]);
        NowBook.AddBook(test[1].c_str());
        DoSelect();
        UpdateBook(NowBook);
        return 1;
    }
    if(length==2&&test[0]=="show"&&test[1]=="finance"){
        if(getnowPerm()!=7) return 3;
        ShowFinance();
        return 1;
    }
    if(length==3&&test[0]=="show"&&test[1]=="finance"){
        if(getnowPerm()!=7) return 3;
        ShowFinance(test[2]);
        return 1;
    }
    if(test[0]=="show"){
        if(getnowPerm()!=1&&getnowPerm()!=3&&getnowPerm()!=7)
            return 3;
        if(length==1){
            ShowAll();
            return 1;
        }
        if(getInfo(test[1])==NAME||getInfo(test[1])==AUTHOR||getInfo(test[1])==KEYWORD) {
            Show1(int(getInfo(test[1])),test[2]);
            return 1;
        }
        if(getInfo(test[1])==ISBN){
            Show2(test[1]);
            //cout<<test[1]<<endl;
            return 1;
        }
        return 3;
    }
    if(length==3&&test[0]=="buy"){
        if(getnowPerm()!=1&&getnowPerm()!=3&&getnowPerm()!=7)
            return 3;
        buy(test[1],test[2]);
        return 1;
    }
    return 3;
}

int getnowPerm() {
    User U;
    ifstream in;
    in.open("nowUser",ios::binary|ios::in|ios::out);
    if(!in){cerr<<"getnowPerm error"<<endl; exit(0);}
    in.seekg(0);
    in.read(reinterpret_cast<char*>(&U),sizeof(U));
    in.close();
    int tmp=U.getPerm();
    return tmp;
}
void Login(const string &Id,const string &Passwd) {
    int Perm=getnowPerm();
    //cout<<1<<endl;
    //cout<<Perm<<endl;
    ifstream in("User",ios::binary|ios::in);
    if(!in){cerr<<"Login error"<<endl; exit(0);}
    User U;
    bool flag=false;
    in.read(reinterpret_cast<char*>(&U),sizeof(U));
    while(!in.eof()){
        if(U.getOK()&&U.Check_Id(Id.c_str())&&(Perm==7||U.Check_Passwd(Passwd.c_str()))){
            flag=true;
            break;
        }
        in.read(reinterpret_cast<char*>(&U),sizeof(U));
    }
    in.close();
    if(flag)
        ResetLogin(U);
    else
        printf("Invalid\n");
}
void ResetLogin(User U) {
    fstream io("nowUser",ios::binary|ios::in|ios::out);
    if(!io){cerr<<"ResetLogin error"<<endl; exit(0);}
    io.seekp(0);
    io.write(reinterpret_cast<char*>(&U),sizeof(U));
    io.close();
}
void Delete(const string &s) {
    fstream io;
    io.open("User",ios::binary|ios::in|ios::out);
    if(!io){cerr<<"Delete error"<<endl; exit(0);}
    io.seekg(0);
    User U;
    int pos=0;
    bool flag=false;
    io.read(reinterpret_cast<char*>(&U),sizeof(U));
    while(!io.eof()){
        if(U.getOK()&&U.Check_Id(s.c_str())){
            U.Delete();
            flag=true;
            io.seekg(pos*sizeof(U));
            io.write(reinterpret_cast<char*>(&U),sizeof(U));
            break;
        }
        pos++;
        io.read(reinterpret_cast<char*>(&U),sizeof(U));
    }
    io.close();
    if(flag) return;
    printf("Invalid\n");
}
bool OKId(const string &s) {
    ifstream in("User",ios::binary|ios::in);
    if(!in){cerr<<"OKId error"<<endl; exit(0);}
    in.seekg(0);
    User U;
    in.read(reinterpret_cast<char*>(&U),sizeof(U));
    while(!in.eof()){
        if(U.getOK()&&U.Check_Id(s.c_str())){
            //cerr<<1<<endl;
            return true;
        }
        in.read(reinterpret_cast<char*>(&U),sizeof(U));
    }
    return false;
}
void AddUser(User U) {
    ofstream out("User",ios::binary|ios::out|ios::app);
    if(!out){cerr<<"AddUser error"<<endl; exit(0);}
    out.seekp(0,ios::end);
    out.write(reinterpret_cast<char*>(&U),sizeof(U));
    out.close();
}
void ChangePasswdroot(const string &Id, const string &Passwd) {
    fstream io("User",ios::binary|ios::in|ios::out);
    if(!io) {cerr<<"ChangePasswdroot error"<<endl; exit(0);}
    User U;
    bool flag=false;
    io.seekg(0);
    io.read(reinterpret_cast<char*>(&U),sizeof(U));
    int pos=0;
    while(!io.eof()){
        if(U.getOK()&&U.Check_Id(Id.c_str())){
            U.Change_Passwd(Passwd.c_str());
            io.seekp(pos*sizeof(U));
            io.write(reinterpret_cast<char*>(&U),sizeof(U));
            flag=true;
            break;
        }
        io.read(reinterpret_cast<char*>(&U),sizeof(U));
        pos++;
    }
    io.close();
    if(flag) return;
    printf("Invalid\n");
}
void ChangePasswd(const string &Id, const string &OldPasswd, const string &NewPasswd) {
    fstream io("User",ios::binary|ios::in|ios::out);
    if(!io) {cerr<<"ChangePasswd error"<<endl; exit(0);}
    User U;
    bool flag=false;
    io.seekg(0);
    io.read(reinterpret_cast<char*>(&U),sizeof(U));
    int pos=0;
    while(!io.eof()){
        if(U.getOK()&&U.Check_Id(Id.c_str())){
            if(U.Check_Passwd(OldPasswd.c_str())) {
                U.Change_Passwd(NewPasswd.c_str());
                io.seekp(pos * sizeof(U));
                io.write(reinterpret_cast<char *>(&U), sizeof(U));
                flag = true;
            }
            break;
        }
        io.read(reinterpret_cast<char*>(&U),sizeof(U));
        pos++;
    }
    io.close();
    if(flag) return;
    printf("Invalid\n");
}
void Select(const string &s) {
    NowPage=getHash(s),NowPos=0;
    fstream io("Books"+to_string(NowPage),ios::binary|ios::in|ios::out);
    if(!io){cerr<<"Select error"<<endl; exit(0);}
    bool flag=false;
    Book B;
    io.read(reinterpret_cast<char*>(&B),sizeof(B));
    while(!io.eof()){
        if(B.getOK()&&B.Check_ISBN(s.c_str())){
            flag=true;
            break;
        }
        NowPos+=sizeof(B);
        io.read(reinterpret_cast<char*>(&B),sizeof(B));
    }
    if(flag)
        NowBook=B;
    else{
        Book tmp(s.c_str());
        NowBook=tmp;
        io.seekp(NowPos);
        io.write(reinterpret_cast<char*>(&tmp),sizeof(tmp));
    }
    io.close();
    DoSelect();
}
void DoSelect() {
    fstream io("nowSelect",ios::binary|ios::in|ios::out);
    if(!io){cerr<<"DoSelect error"<<endl; exit(0);}
    io.seekp(0);
    io.write(reinterpret_cast<char*>(&NowPage),sizeof(NowPage));
    io.write(reinterpret_cast<char*>(&NowPos),sizeof(NowPos));
    io.write(reinterpret_cast<char*>(&NowBook),sizeof(NowBook));
    io.close();
}
Info getInfo(const string &s) {
    //cout<<s<<endl;
    int pos=0;
    for(int i=0;i<s.size();++i){
        if(s[i]=='='){
            pos=i;
            break;
        }
    }
    if(pos==0||s[0]!='-')
        return ERROR;
    string tmp=s.substr(1,pos-1);
    //cout<<tmp<<endl;
    if(tmp=="ISBN") return ISBN;
    else if(tmp=="price") return PRICE;
    //the next thing is to clear the ""
    else if(tmp=="name") return NAME;
    else if(tmp=="author") return AUTHOR;
    else if(tmp=="keyword") return KEYWORD;
    else return ERROR;
}
void NonSelect() {
    fstream io("nowSelect",ios::binary|ios::in|ios::out);
    if(!io){cerr<<"NonSelect error"<<endl; exit(0);}
    io.seekg(0);
    io.read(reinterpret_cast<char*>(&NowPage),sizeof(NowPage));
    io.read(reinterpret_cast<char*>(&NowPos),sizeof(NowPos));
    io.read(reinterpret_cast<char*>(&NowBook),sizeof(NowBook));
    io.close();
}
void Modify1(const int &cas,const string &s){
    if(cas==int(NAME)){
        NowBook.ModifyName(s.c_str());
        UpdateBook(NowBook);
    } else if(cas==int(AUTHOR)){
        NowBook.ModifyAuthor(s.c_str());
        UpdateBook(NowBook);
    } else if(cas==int(KEYWORD)){
        NowBook.ModifyKeyword(s.c_str());
        UpdateBook(NowBook);
    }
}
void Modify2(const string &s) {
    int pos=0;
    for(int i=0;i<s.size();++i){
        if(s[i]=='='){
            pos=i;
            break;
        }
    }
    if(pos==0||s[0]!='-'){printf("Invalid\n");return;}
    //cout<<123<<endl;
    string tmp=s.substr(1,pos-1);
    //cout<<tmp<<endl;
    if(tmp=="ISBN"){
        string news=s.substr(pos+1,s.size()-(pos+1));
        if(OKISBN(news)){printf("Invalid\n");return;}
        Book T=NowBook;

        NowBook.Delete();
        UpdateBook(NowBook);  //delete the former

        Select(news);
        NowBook=T;
        NowBook.ModifyISBN(news.c_str());
        UpdateBook(NowBook);  //update the new
    } else if(tmp=="price"){
        string news=s.substr(pos+1,s.size()-(pos+1));
        NowBook.ModifyPrice(news.c_str());
        UpdateBook(NowBook);
    }
}
bool OKISBN(const string &s) {
    NowPage=getHash(s),NowPos=0;
    fstream io("Books"+to_string(NowPage),ios::binary|ios::in|ios::out);
    if(!io){cerr<<"OKISBN error"<<endl; exit(0);}
    Book B;
    io.read(reinterpret_cast<char*>(&B),sizeof(B));
    while(!io.eof()){
        if(B.getOK()&&B.Check_ISBN(s.c_str()))
            return true;
        io.read(reinterpret_cast<char*>(&B),sizeof(B));
    }
    io.close();
    return false;
}
void UpdateBook(Book B) {
    fstream io("Books"+to_string(NowPage),ios::binary|ios::in|ios::out);
    io.seekp(NowPos);
    io.write(reinterpret_cast<char*>(&B),sizeof(B));
    io.close();
}
void ImportBook(const string &s) {
    double out,in;
    fstream io("Bill",ios::binary|ios::in|ios::out);
    io.seekg(0);
    int sum;
    io.read(reinterpret_cast<char*>(&sum),sizeof(sum));
    sum++;
    io.seekp(0);
    io.write((char*)(&sum),sizeof(sum));
    io.seekg(-16,ios::end);
    io.read(reinterpret_cast<char*>(&in),sizeof(in));
    io.read(reinterpret_cast<char*>(&out),sizeof(out));
    double sum1;
    sscanf(s.c_str(),"%lf",&sum1);
    out+=sum1;
    io.seekp(0,ios::end);
    io.write(reinterpret_cast<char*>(&in),sizeof(in));
    io.write(reinterpret_cast<char*>(&out),sizeof(out));
    io.close();
}
void OutportBook(double sum1) {
    double out,in;
    fstream io("Bill",ios::binary|ios::in|ios::out);
    io.seekg(0);
    int sum;
    io.read(reinterpret_cast<char*>(&sum),sizeof(sum));
    sum++;
    io.seekp(0);
    io.write((char*)(&sum),sizeof(sum));
    io.seekg(-16,ios::end);
    io.read(reinterpret_cast<char*>(&in),sizeof(in));
    io.read(reinterpret_cast<char*>(&out),sizeof(out));
    in+=sum1;
    io.seekp(0,ios::end);
    io.write(reinterpret_cast<char*>(&in),sizeof(in));
    io.write(reinterpret_cast<char*>(&out),sizeof(out));
    io.close();
}
void ShowFinance() {
    //cout<<123<<endl;
    //debug
    int nowtime=gettotTime();
    pay tmp=getFinance(nowtime);
    printf("+ %.2f - %.2f\n",tmp.moneyin,tmp.moneyout);
}
void ShowFinance(const string &s) {
    //cout<<456<<endl;
    int nowtime=gettotTime();
    int require;
    sscanf(s.c_str(),"%d",&require);
    if(require>nowtime){printf("Invaild\n");return;}
    pay tmp1=getFinance(nowtime),tmp2=getFinance(nowtime-require);
    //cout<<tmp1.moneyout<<" "<<tmp2.moneyout<<endl;
    printf("+ %.2f - %.2f\n",tmp1.moneyin-tmp2.moneyin,tmp1.moneyout-tmp2.moneyout);
}
int gettotTime() {
    fstream io("Bill",ios::binary|ios::in|ios::out);
    int sum;
    io.seekg(0);
    io.read(reinterpret_cast<char*>(&sum),sizeof(sum));
    io.close();
    //cout<<sum<<endl;
    return sum;
}
pay getFinance(const int &k) {
    fstream io("Bill",ios::binary|ios::in|ios::out);
    io.seekg(sizeof(int)+2*sizeof(double)*k);
    double a,b;
    io.read(reinterpret_cast<char*>(&a),sizeof(a));
    io.read(reinterpret_cast<char*>(&b),sizeof(b));
    io.close();
    pay tmp;
    tmp.moneyin=a;
    tmp.moneyout=b;
    //cout<<a<<" "<<b<<endl;
    return tmp;
}
void ShowAll(){
    vector<Bookinfo> A;
    for(int i=0;i<Hash;++i){
        fstream io;
        io.open("Books"+to_string(i),ios::binary|ios::in|ios::out);
        if(!io){cout<<"ShowAll error"<<endl; exit(0);}
        io.seekg(0);
        Book B;
        int pos=0;
        io.read(reinterpret_cast<char*>(&B),sizeof(B));
        while(!io.eof()){
            if(B.getOK()) {
                Bookinfo tmp;
                tmp.ISBN = B.getISBN();
                tmp.page = i;
                tmp.pos = pos;
                A.push_back(tmp);
            }
                pos+=sizeof(B);
                io.read(reinterpret_cast<char*>(&B),sizeof(B));
        }
        io.close();
    }
    sort(A.begin(),A.end(),cmp);
    for(auto const &i:A){
        ShowBook(i.page,i.pos);
    }
}
void ShowBook(const int &page, const int &pos) {
    fstream io;
    io.open("Books"+to_string(page),ios::binary|ios::in|ios::out);
    if(!io){cout<<"ShowBook error"<<endl; exit(0);}
    io.seekg(pos);
    Book B;
    io.read(reinterpret_cast<char*>(&B),sizeof(B));
    B.show();
}
void Show1(const int &cas, const string &s) {
    vector<Bookinfo> A;
    for(int i=0;i<Hash;++i){
        fstream io;
        io.open("Books"+to_string(i),ios::binary|ios::in|ios::out);
        if(!io){cout<<"Show1 error"<<endl; exit(0);}
        io.seekg(0);
        Book B;
        int pos=0;
        io.read(reinterpret_cast<char*>(&B),sizeof(B));
        while(!io.eof()){
            if(B.getOK()) {
                if(cas==int(NAME)&&B.Check_Name(s.c_str())){
                        Bookinfo tmp;
                        tmp.ISBN = B.getISBN();
                        tmp.page = i;
                        tmp.pos = pos;
                    A.push_back(tmp);
                }
                if(cas==int(AUTHOR)&&B.Check_Author(s.c_str())){
                    Bookinfo tmp;
                    tmp.ISBN = B.getISBN();
                    tmp.page = i;
                    tmp.pos = pos;
                    A.push_back(tmp);
                }
                if(cas==int(KEYWORD)&&B.Check_Keyword(s.c_str())){
                    Bookinfo tmp;
                    tmp.ISBN = B.getISBN();
                    tmp.page = i;
                    tmp.pos = pos;
                    A.push_back(tmp);
                }
            }
            pos+=sizeof(B);
            io.read(reinterpret_cast<char*>(&B),sizeof(B));
        }
        io.close();
    }
    sort(A.begin(),A.end(),cmp);
    for(auto const &i:A){
        ShowBook(i.page,i.pos);
    }
}
void Show2(const string &s) {
    int pos=0;
    for(int i=0;i<s.size();++i){
        if(s[i]=='='){
            pos=i;
            break;
        }
    }
    if(pos==0||s[0]!='-'){printf("Invalid\n");return;}
    string news=s.substr(pos+1,s.size()-(pos+1));
    //cout<<news<<endl;
    vector<Bookinfo> A;
    for(int i=0;i<Hash;++i){
        fstream io;
        io.open("Books"+to_string(i),ios::binary|ios::in|ios::out);
        if(!io){cout<<"Show2 error"<<endl; exit(0);}
        io.seekg(0);
        Book B;
        int pos=0;
        io.read(reinterpret_cast<char*>(&B),sizeof(B));
        while(!io.eof()){
            //cout<<B.getISBN()<<endl;;
            //cout<<news<<endl;
            //cout<<(B.getOK()&&B.Check_ISBN(news.c_str()))<<endl;
            //cout<<(B.getOK())<<endl;
            //cout<<(B.Check_ISBN(news.c_str()))<<endl;
            if(B.getOK()&&B.Check_ISBN(news.c_str())){
                    Bookinfo tmp;
                    tmp.ISBN = B.getISBN();
                    tmp.page = i;
                    tmp.pos = pos;
                    A.push_back(tmp);
            }
            pos+=sizeof(B);
            io.read(reinterpret_cast<char*>(&B),sizeof(B));
        }
        io.close();
    }
    sort(A.begin(),A.end(),cmp);
    for(auto const &i:A){
        ShowBook(i.page,i.pos);
    }
}

void buy(const string &_ISBN, const string &_Quti) {
    fstream io;
    io.open("Books"+to_string(getHash(_ISBN)),ios::binary|ios::in|ios::out);
    if(!io){cout<<"buy error"<<endl; exit(0);}
    Book B;
    bool flag=false;
    io.seekg((0));
    int Quti=0;
    int pos=0;
    sscanf(_Quti.c_str(), "%d", &Quti);
    io.read(reinterpret_cast<char*>(&B),sizeof(B));
    while(!io.eof()){
        if(B.getOK()&&B.Check_ISBN(_ISBN.c_str())){
            flag=true;
            break;
        }
        io.read(reinterpret_cast<char*>(&B),sizeof(B));
        pos+= sizeof(B);
    }
    if(!flag)
        printf("Invalid\n");
    else{
        if(B.Buy(Quti)){
            OutportBook(Quti*B.getPrice());
            io.seekg(pos);
            io.write(reinterpret_cast<char*>(&B),sizeof(B));
        }
        else printf("Invalid\n");
    }
}
