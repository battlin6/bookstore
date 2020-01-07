#include <iostream>
#include "User.h"
#include "Book.h"
#include <fstream>
#include <bits/stdc++.h>
using namespace std;

const int Hash=3;  //just 103 files I dont want too many
//but now just for easy I simplify it
int getHash(const string &s); //get hash number

int DeleteUser=0;
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
bool OKId(const string &s);
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
 */

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
    //todo:
}
void Create_Bill() {
    //todo:
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
    if(test[0]=="exit"&&length==1) return 2;
    if(length==0) return 1;
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
