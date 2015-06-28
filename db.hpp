#ifndef DB_HPP
#define DB_HPP

#include <iostream>
#include <string>
#include <regex>
#include <assert.h>
#include <vector>

using namespace std;
// todo only request write permisiions when need to write
struct Table {
    string name;
    std::vector<string> attributes;
    Table(string& iname) {
        name = iname;
    }
};
struct Database {
    string name;
    Database* next;
    Table* tableFirst;
    Table* tableLast;
    std::vector<Table*> tables;
    int tbCount;
    Database(string& iname) {
        name = iname;
        next = NULL;
        tableFirst = NULL;
        tableLast = NULL;
        tbCount = 0;
    }
};


class DB
{
private:
    Database* dbFirst;
    Database* dbLast;
    
    std::vector<Database*> databases;
    
    int dbCount;
    regex rxFinddb,rxFindtb,rxFindattr;
    
    void delDbs();
    void readDb(char* dbMemMap, size_t size);
    bool parseLine(string, Database*&, Table*&);
    const char* splitstr(char*,const char*);
    
    Database* insertDb(string&);
    Table* insertTable(Database*&,string&);
    
    void initRegex();
    void printDBTables(Database*&);
    
public:
    DB(char* dbMemMap, size_t size);
    ~DB();
    void printDBs();

};

#endif // DB_HPP
