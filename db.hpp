#ifndef DB_HPP
#define DB_HPP

#include <iostream>
#include <string>
#include <regex>
#include <assert.h>
#include <vector>

using namespace std;
// todo only request write permisiions when need to write
// struct Attr { // todo store type
//    string name;
//
//};
struct Table
{
    string name;
    std::vector<string> attributes;
    std::vector<string> values;
    std::vector<vector<string> > rows;
    Table(string& iname)
    {
        name = iname;
    }
};
struct Database
{
    string name;
    int tbCount;
    std::vector<Table*> tables;
    Database(string& iname)
    {
        name = iname;
        tbCount = 0;
    }
    ~Database()
    {
        for (uint i = 0; i < this->tables.size(); i++)
            delete this->tables[i];
    }
};

class DB
{
private:
    std::vector<Database*> databases;

    int dbCount;
    regex rxFinddb, rxFindtb, rxFindattr;

    // int MAX_ATTR_COUNT = 20; // maximum number of attributes in a table
    string DELIMITER = " "; // delimiter
    char STARTCHAR = '#';   // starting character

    void delDbs();
    void readDb(char* dbMemMap, size_t size); // read database and parse to data structure

    bool parseTypeLine(string&, Database*&, Table*&); // parse type line eg # start db test
    bool parseDataLine(string&, Table*&);             // parse data line eg 1 lel el

    vector<string> splitstr(char*, const char*); // split string given delimeter

    Database* insertDb(string&);             // create database with name
    Table* insertTable(Database*&, string&); // create table in database with name
    bool insertTableAttr(Table*&, string&); // sets attributes in table

    Database* findDb(string&);             // find database in databases with name
    Table* findTable(Database*&, string&); // find table in give database with name

    void initRegex();

    void printDBTables(Database*&); // print database tables
    void printTableAttr(Table*&);   // print table attributes
    void printTableVals(Table*&);   // print table values

public:
    DB(char* dbMemMap, size_t size);
    ~DB();
    bool insert(string&, string&, string&);
    void printDBs();
};

#endif // DB_HPP
