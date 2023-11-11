
#include "BPTree2.h"
#include "Date.h"
#include <iostream>
#include <fstream>
#include <string.h>


using namespace std;

int main(int argc, const char* argv[]) {
    string dbName = "mydb";
    string fileName = "data.txt";
    //string tableName = "student.t";
    DataBase* db = new DataBase(dbName);
    StudentTable* st = new StudentTable();
    st->CreateTable(db->dbName);
    st->insertData(fileName);
    //st->Outdate();
    // Student s;
    // string name = "123";
    // s.sno = 123;
    // strcpy(s.sname,"123");
    // strcpy(s.ssex,"1283");
    // st->modifyDate(s,48);

//    st->Outdate();
}
