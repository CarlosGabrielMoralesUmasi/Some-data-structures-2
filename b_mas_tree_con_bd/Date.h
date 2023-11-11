
#ifndef DATA_H_INCLUDED
#define DATA_H_INCLUDED

#include "BPTree2.h"

#include <fstream>
#include <iostream>
//#include "direct.h"
#include "assert.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string>
#include <io.h>  // Para _access
#include <direct.h>  // Para _mkdir
#define MaxTable 10
#define MaxColumn 10
#define MaxLength 20

using namespace std;

struct Student
{
    int sno;
    char sname[MaxLength];
    char ssex[MaxLength];
    int diff;
};

class DataBase
{
public:
    string dbName;
    //string dbTable[MaxTable];
    DataBase(string _dbname){
        
        this->dbName = _dbname;
            string folderPath = "/Users/zhaotong/C++Program/DSTrain/B+TreeV3.0/data/" + dbName;
            if (0 != _access(folderPath.data(), 0))
{
    // if this folder not exist, create a new one.
    int flag = _mkdir(folderPath.data()); // Retorna 0 si la creación es exitosa, -1 si falla

    if (flag == 0)
        cout << "Database make successfully" << endl;
    else
        cout << "Database already exists" << endl;
}
else
    cout << "database " << dbName << " has already exists" << endl;
    }
};

//DataBase::DataBase(string _dbName)


class StudentTable
{
private:
    string tableName; 
    int n;            
    string dbName;    
    Student st_root;  
    BPTree *bptree;   
    int locate;     
public:
    StudentTable(){
        
            tableName = "student.td";
            dbName = "";
            n = 0;
            locate = 0;
            bptree = NULL;
    }
    bool CreateTable(string dbName);
    bool buildTrees(Student &st, int n);
    int insertData(string fileName);      
    bool insertData(Student &st);          
    int spilt_n(string temp);             
    bool Outdate();                        
    bool modifyDate(Student st, int diff); 
};
/*
StudentTable::StudentTable()
{
    tableName = "student.td";
   dbName = "";
    n = 0;
    locate = 0;
   bptree = NULL;
}*/

bool StudentTable::CreateTable(string _dbName){
    dbName = _dbName;
    string path = "/Users/zhaotong/C++Program/DSTrain/B+TreeV3.0/data/" + dbName + "/" + tableName;
    fstream file;
    file.open(path.data(), ios::in);
    if (file)
    {
        cout << "Table " << tableName << " has already exits in database " << dbName << endl;
        file.seekg(0, ios::end);
        locate = (int)file.tellg();
        file.close();
        cout << "locate为" << locate << endl;
    }
    else
    {
        ofstream fout(path.data());
        if (fout)
        {
            cout << "Create table " << tableName << " successfully in database " << dbName << endl;
            
            fout.close();
        }
    }
    file.close();

    /*string ProcessDatePath = "/Users/zhaotong/C++Program/DSTrain/B+TreeV3.0/data/" + dbName + "/" + tableName; //生成的数据库数据文件地址
     ifstream fileOut(ProcessDatePath.data());;
     if (!fileOut.is_open())
     {
          cout << "Table " << tableName << " has already exits in database " << dbName << endl;
     }
     else
     {
         fileOut.seekg(0, ios::end);
         locate = fileOut.tellg();
        fileOut.close();
         cout << "locate为" << locate << endl;
     }*/ 
    return true;
}

int StudentTable::insertData(string fileName)
{
    if (n == 0)
    {
        string filePath = "/Users/zhaotong/C++Program/DSTrain/B+TreeV3.0/data/" + dbName + "/" + "student.i";
        bptree = new BPTree(filePath);
    }
    Student st_temp;
    ifstream fileOut; 
    int i = 0;        

    string DatePath = "/Users/zhaotong/C++Program/DSTrain/B+TreeV3.0/data/" + fileName;

    cout << DatePath << endl;
    fileOut.open(DatePath.data(), ios::in); 
    assert(fileOut);

    if (!fileOut.is_open())
    {
        cout << "Error al abrir el archivo de datos" << endl;
        exit(1);
    }
    while (!fileOut.eof())
    {
        /* code */
        fileOut >> st_temp.sno >> st_temp.sname >> st_temp.ssex; //Leer el archivo de datos línea por línea
//        cout << st_temp.sno << st_temp.sname << st_temp.ssex << endl;
        if (insertData(st_temp))
        {
            i++;
        };
    }
    bptree->Travelsal();
    fileOut.close();
    cout << "insertado exitosamente " << i << "datos del txt" << endl;
    return n;
}

bool StudentTable::insertData(Student &st)
{
    ofstream fileIn;                                               
    string ProcessDatePath = "/Users/zhaotong/C++Program/DSTrain/B+TreeV3.0/data/" + dbName + "/" + tableName; 
    fileIn.open(ProcessDatePath.data(), ios::binary | ios::app);  
    if (!fileIn.is_open())
    {
        cout << "Error al abrir el archivo de datos de la base de datos" << endl;
        return false;
    }
    assert(fileIn);
    Triple loc=bptree->Search(st.sno);
    if(!loc.tag)return false;
    else{
    st.diff = locate;
    fileIn.write((char *)&st, sizeof(Student));
    fileIn.close();
    locate += sizeof(Student);
    bptree->Insert(st.sno,locate-sizeof(Student),loc);
    cout << "La compensación es " << locate << endl;
    }
    return true;
}

bool StudentTable::Outdate()
{
    Student st;
    ifstream fileOut;
    int diff = 0;
    int n = 0;                                                     //数据量
    string ProcessDatePath = "/Users/zhaotong/C++Program/DSTrain/B+TreeV3.0/data/" + dbName + "/" + tableName; 
    cout << ProcessDatePath << endl;
    fileOut.open(ProcessDatePath.data(), ios::binary | ios::in);
    if (!fileOut.is_open())
    {
        cout << "Error al abrir el archivo de datos de la base de datos" << endl;
        return false;
    }
    assert(fileOut);
    fileOut.seekg(diff); //Coloco el archivo al principio del archivo.
    cout << "La compensación es" << diff << endl;
    while (fileOut.tellg() != -1) 
    {
        /* code */
        //Solo se verificará el último estado y se debe revisar nuevamente en el bucle para evitar que el final del archivo se lea dos veces.
        if (fileOut.peek() == EOF)
        {
            break;
        }
        fileOut.read((char *)&(st), sizeof(Student));
        cout << st.sno << " " << st.sname << " " << st.ssex << " " << st.diff << endl;
        diff = diff + sizeof(Student);
        cout << "La compensación es" << diff << endl;
        n++;
    }
    fileOut.close();
    cout << "compartir " << n << " datos del artículo" << endl;
    return true;
}

bool StudentTable::modifyDate(Student st, int diff)
{
    ofstream fileIn;                                                      
    string ProcessDatePath = "/Users/zhaotong/C++Program/DSTrain/B+TreeV3.0/data/" + dbName + "/" + tableName; //Dirección del archivo de bd de datos generada
    fileIn.open(ProcessDatePath.data(), ios::binary | ios::out | ios::in); 
    if (!fileIn.is_open())
        return false;
    fileIn.seekp(diff);
    fileIn.write((char *)&st, sizeof(Student));
    fileIn.close();
    return true;
}

#endif // DATA_H_INCLUDED

