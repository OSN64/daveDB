#include <stdio.h>
#include "davedb.hpp"

int main(int argc, char** argv)
{
    string dbName = "test.davedb";
    Davedb db(dbName);
     
    //db.printDBs();
    
//    string dbname = "testdb";
//    string tb = "pin";
//    string attributes[]= {"pon","nio"};
    
//    db.addTable(dbname,tb);
    
//    db.addTbAttr(dbname,tb,attributes);
    
//    db.printDBs();
     //    printf("hello world\n");   
//    getchar();

    // play with mem mapped file
//    char* map = db.playWithmemmap();
//    int i = 0;
//    char to = '#';
    
//    cout << "current: " << map[i] << endl;
//    map[i] = to;
//    cout << "changed: " << map[i] << endl;

//    return 0;
}
