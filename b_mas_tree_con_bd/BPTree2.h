#ifndef B_Tree_h
#define B_Tree_h

#include <fstream>
#include <iostream>
//#include "direct.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#define MaxValue 2000000
#define m 4
#define MaxLength 20
using namespace std;


struct BPTreeNode
{
    int n;              //numero de llaves (keys)
    int partentDiff;    //puntero del nodo padre
    int key[m + 2];     //Código clave, la clave [0] no se utiliza
    int ptrDiff[m + 2]; //Desplazamiento del nodo del subárbol
    int rightDiff;      //Desplazamiento del nodo de hoja adyacente a la derecha
    int recptr[m + 2];  //El desplazamiento en cada entrada de índice del archivo de datos de la base de datos.

    BPTreeNode()
    {
        n = 0;
        partentDiff = -1;
        for (int i = 0; i < m + 2; i++)
        {
            key[i] = MaxValue;
            ptrDiff[i] = -1;
            recptr[i] = -1;
        }
        rightDiff = -1;
    }
    const BPTreeNode& operator=(const BPTreeNode& good){
        this->n=good.n;
        this->partentDiff=good.partentDiff;
        this->rightDiff=good.rightDiff;
        for(int i=0;i<m+2;i++){
            this->key[i]=good.key[i];
            this->ptrDiff[i]=good.ptrDiff[i];
            this->recptr[i]=good.recptr[i];
        }
        return *this;
    }
//    BPTreeNode()
};
/*
*Search()Estructura de datos del valor de retorno de la función.
*/
//---------------------------------------------------------------------------------
struct Triple
{
    int diff; //Dirección de nodo en el desplazamiento del archivo de datos de la base de datos
    int i, tag;
};
class BPTree
{
private:
    int root; 
    string filePath; 
    int sqt;
    int size;
public:
    BPTree()
    {
        // root = NULL;
        // sqt = NULL;
        
        filePath = "";
        size = 0;
        ifstream fileOut;                                                  //文件输出流
        fileOut.open("C:/Users/User/source/repos/b_mas_tree/b_mas_tree/data/mydb/student.r", ios::binary | ios::out |ios::in);
        if(!fileOut.is_open()) {
            cout<<"Error al abrir el archivo del directorio"<<endl;
        }
        fileOut.seekg(0);
        fileOut.read((char *)&root, sizeof(int));
        fileOut.seekg(sizeof(int));
        fileOut.read((char *)&sqt, sizeof(int));
        fileOut.close();
//        root = 
    }; //无参构造函数
    BPTree(string &_filePath)
    {
        
        // sqt = NULL;
        filePath = _filePath;
        size = 0;
        ifstream fileOut;                                                  //文件输出流
        fileOut.open("C:/Users/User/source/repos/b_mas_tree/b_mas_tree/data/mydb/student.r", ios::binary | ios::out |ios::in);
        if(!fileOut.is_open()) {
            cout<<"Error al abrir el archivo del directorio"<<endl;
        }
        fileOut.seekg(0);
        fileOut.read((char *)&root, sizeof(int));
        fileOut.seekg(sizeof(int));
        fileOut.read((char *)&sqt, sizeof(int));
        fileOut.close();
    }
    Triple Search(const int &x);                
    bool Insert(const int &x, const int &diff,Triple loc); //Inserte el código clave x y almacene el desplazamiento del cabezal de los datos en el archivo de datos de la base de datos en recptr[];
    bool Remove(const int &x);                  //Eliminar código key x
    void Travelsal(){
        BPTreeNode temp;
        int p=sqt;
        while(GetNode(temp, p)&&p!=-1){
            for(int i=0;i<temp.n ;i++){
                cout<<temp.key[i+1]<<endl;
        }
            p=temp.rightDiff;
        }
    }
    bool GetNode(BPTreeNode &bpn, int pidff); //La lectura del nodo bpn del archivo de datos de la base de datos requiere un desplazamiento de nodo conocido
    int PutNode(BPTreeNode &bpn); //Agrega un nodo al final del archivo de base de datos, devolviendo el desplazamiento del nodo
    bool ModifyNode(BPTreeNode &bpn, int pidff);// Modifica los datos del nodo en la ubicación original del archivo
    bool ModifyRootSqt(int tag,int diff);
    void move(BPTreeNode& p,BPTreeNode& q,int s,int x,int qd);// Se utiliza para asignar datos para nodos secundarios formados mediante división.
};

bool BPTree::GetNode(BPTreeNode &bpn, int pidff)
{
    ifstream fileOut;
    string ProcessDatePath = filePath; 
    fileOut.open(ProcessDatePath.data(), ios::binary | ios::in);
    if (!fileOut.is_open())
    {
        cout << "Error al abrir el archivo de índice de la base de datos" << endl;
        return false;
    }
    else
    {
        /* code */
        fileOut.seekg(pidff);
        fileOut.read((char *)&bpn, sizeof(BPTreeNode));
        fileOut.close();
    }
    return true;
}

int BPTree::PutNode(BPTreeNode &bpn)
{
    ofstream fileIn;
    fileIn.open(filePath.data(), ios::binary | ios::app);
    if (!fileIn.is_open())
    {
        cout << "Error al abrir el archivo del directorio" << endl;
        return -1;
    }
    assert(fileIn);
    fileIn.write((char *)&bpn, sizeof(BPTreeNode));
    size += sizeof(BPTreeNode);
    fileIn.close();
    return size - sizeof(BPTreeNode); // Devuelve el desplazamiento del cabezal del nodo insertado
}

Triple BPTree::Search(const int &x)
{
    Triple result;

    // GetNode(root, 0); //Obtiene la raíz con desplazamiento 0 del archivo de índice para que el nodo raíz resida en la memoria
    // cout << root.key[1] << endl;
    BPTreeNode p, q;
    GetNode(p, root);
    int resultDiff = 0;
    int i = 0;
    while (1)
    {
        /* code */
        i = 0;
        //cout <<"search的p key为"<<p.key[1]<<endl;
        p.key[p.n + 1] = MaxValue;
        if (p.n == 0)
        {
            resultDiff = -1;
            break;
        }
        while (p.key[i + 1] < x)
        {
            i++;
        }
        if (p.key[i + 1] == x)
        {
            if (p.ptrDiff[i] != -1)
            {
                resultDiff = p.ptrDiff[i];
                GetNode(p, p.ptrDiff[i]); //Si tiene nodos secundarios, actualice p para convertirlo en un nodo secundario
                while (1)
                {
                    /* code */
                    if (p.ptrDiff[p.n - 1] == -1)
                    {
                        break;
                    }
                    i = p.n;
                    resultDiff = p.ptrDiff[p.n - 1];
                    GetNode(p, p.ptrDiff[p.n - 1]);
                }
                result.diff = resultDiff;
                result.i = i;
                result.tag = 0;
                return result;
            }
            else
            {
                /* code */
                result.diff = resultDiff;
                result.i = i + 1;
                result.tag = 0;
                return result;
            }
        }
        if(i!=0){
        if (p.ptrDiff[i-1] == -1)
        {
            break;
        }
        }else{
            if(p.ptrDiff[i]==-1)
//                i++;
                break;
        }
        if (i == p.n)
        {
            resultDiff = p.ptrDiff[i - 1];
            GetNode(p, p.ptrDiff[i - 1]);
        }
        else
        {
            resultDiff = p.ptrDiff[i];
            GetNode(p, p.ptrDiff[i]);
        }
    }
    result.diff = resultDiff;
    result.i = i;
    result.tag = 1;
    return result; //La búsqueda falló, regresa a la posición de inserción
     //Los intervalos K[i] y K[i+1] en los que x puede caer
}
void insertKey(BPTreeNode& p,int j,int k,int ap,int pstu){
    for(int i=p.n;i>j;i--){
        p.key[i+1]=p.key[i];
        p.ptrDiff[i]=p.ptrDiff[i-1];
        p.recptr[i]=p.recptr[i-1];
    }

    p.key[j+1]=k;
    p.ptrDiff[j]=ap;
    p.recptr[j]=pstu;
    p.n++;
    
};
void BPTree::move(BPTreeNode& p,BPTreeNode& q,int s,int x,int qd){
    
    int i=0,j=p.n-s+1;
         for(i=0;i<s;i++){
             q.key[i+1]=p.key[j];
             q.ptrDiff[i]=p.ptrDiff[j-1];
//             if(q.ptrDiff[i]!=NULL&&q.ptr[i]->parent!=NULL){
             if(q.ptrDiff[i]!=-1){
                 BPTreeNode temp;
                 GetNode(temp, q.ptrDiff[i]);
                 temp.partentDiff=qd;
                 ModifyNode(temp, q.ptrDiff[i]);
             }
             q.recptr[i]=p.recptr[j-1];
             j++;
         }
     //    i=0;
     //    BPTreeNode* t=p->parent;
     //    while(t->ptr[i]!=p)i++;
     //    for(j=t->n-1;j>=i;j--){
     //        t->ptr[j+1]=t->ptr[j];
     //    }
     //    t->ptr[i+1]=q;
    q.ptrDiff[i]=p.ptrDiff[j-1];
    q.recptr[i]=p.recptr[j-1];
    p.n=p.n-s;
    q.n=s;
}
bool BPTree::Insert(const int &x, const int &diff,Triple loc){
//    Triple loc = Search(x);
//    if (!loc.tag)
//        return false;
    
    BPTreeNode p,q,pp,teststruct;
    BPTreeNode t;
    
    int ap = -1,qd=-1,pd =loc.diff;
    int pstu=diff;
    int k=x,j=loc.i,k1,i=-1;
    
    if (loc.diff == -1){
            ofstream fileIn;
            fileIn.open(filePath.data(), ios::binary | ios::app); 
            p.key[1]=x;
            p.recptr[0]=pstu;
            p.partentDiff=-1;
            p.n++;

        sqt=root=PutNode(p);
        ModifyRootSqt(1,root);
        ModifyRootSqt(0,sqt);
        return true;
    }
    GetNode(p,pd);
//    GetNode(teststruct, root);
    while(1){
        if(p.n<m){
            if(k<p.key[p.n]||p.partentDiff==-1)
            {
            insertKey(p, j, k, ap ,pstu);
            ModifyNode(p, pd);
            }
            else{
                insertKey(p, j, k, ap,pstu);
                ModifyNode(p, pd);
                i=0;
                while(p.partentDiff!=-1){
                GetNode(t, p.partentDiff);
                while(t.ptrDiff[i]!=loc.diff)i++;
                    //Localizo la posición correspondiente del nodo principal que sea igual al valor máximo anterior en p y reemplácela con el valor máximo actual en p
                t.key[i+1]=p.key[p.n];
                ModifyNode(t, p.partentDiff);
                pd=p.partentDiff;
                GetNode(p, p.partentDiff);
//                pd=p.partentDiff;
                i=0;
                }
                ModifyNode(p, pd);
            }
            return true;
        }
        int s=(m+1)/2;
        k1=p.key[p.n];   //k1 almacena el valor máximo de p en el momento antes de insertar la división
        insertKey(p, j, k, ap,pstu);
        qd=PutNode(q);
        q.rightDiff=p.rightDiff;
        p.rightDiff=qd;    //Apunte el puntero derecho de p a la q recién dividida
        move(p,q,s,m,qd);//se mueve la clave de p[s+2,m+1] a la clave de q[1,s]
                          //Mueve ptr[s+1,m+1] de p a la tecla[0,s] de q
                           //p->n se convierte en s+1, q->n se convierte en m-s

        k=p.key[p.n];     //k almacena la clave más grande actual de q
        ap=qd;
        if(p.partentDiff!=-1){
        GetNode(t, p.partentDiff);
        i=j=0;
        while(t.ptrDiff[i]!=pd)i++;
        t.key[i+1]=k;
                
        //        while(t->key[j+1]<k1)j++;
        j=i+1;
                //Encuentro el lugar donde se insertará el valor clave máximo actual de q.
        q.partentDiff=p.partentDiff;
        ModifyNode(p, pd);
        ModifyNode(t, p.partentDiff);
        ModifyNode(q, qd);
        k=q.key[q.n];
        pd=p.partentDiff;
        p=t;
        
    }
            else{                                      //La primera vez que no hay un nodo principal, se crea un nodo principal y agréguele pq
//                root = new BPTreeNode();
            t.n=2;                              //Añado dos claves
                t.partentDiff=-1;//Creo nueva raíz
                ModifyNode(p, pd);
                ModifyNode(q, qd);
                t.key[1]=p.key[p.n];t.key[2]=q.key[q.n];     //Se integra los dos valores máximos del subárbol en
                t.ptrDiff[0]=pd;t.ptrDiff[1]=qd;       //Conecto el nodo secundario a p y el q recién dividido
                root=p.partentDiff=q.partentDiff=PutNode(t);
                
                ModifyNode(p, pd);
                ModifyNode(q, qd);
                                        
                ModifyRootSqt(1, root);                                  
                return true;
            }
        }
            return true;
}
//---------------------------------------------------------------------------------

bool BPTree::ModifyNode(BPTreeNode &bpn,int diff){
    ofstream fileIn;                                                 
    fileIn.open(filePath.data(), ios::binary | ios::out |ios::in);
    if(!fileIn.is_open()) return false;
    fileIn.seekp(diff);
    fileIn.write((char *)&bpn,sizeof(BPTreeNode));
    fileIn.close();
    return true;
}
bool BPTree::ModifyRootSqt(int tag,int diff){
    ofstream fileIn;                                                 
    fileIn.open("C:/Users/User/source/repos/b_mas_tree/b_mas_tree/data/mydb/student.r", ios::binary | ios::out |ios::in);
    if(!fileIn.is_open()) return false;
    if(tag){
    fileIn.seekp(0);
    fileIn.write((char *)&diff,sizeof(int));
    fileIn.close();
    return true;
    }else{
        fileIn.seekp(sizeof(int));
        fileIn.write((char*)&diff, sizeof(int));
        fileIn.close();
        return true;
    }
    return true;
}


#endif /* B_Tree_h */
