#ifndef DAVEDB_HPP
#define DAVEDB_HPP

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <stdlib.h>  
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>

#include "db.hpp"

using namespace std;

class Davedb
{
    
private:
    const string dbExt = ".davedb"; // database file extension

    string dbFileName;
    int dbFd;
    char* mmappedData;
    
    int dbCount;
    
    void init();
    int openDBfile(); // open database file
    void closeDBFile(); // close database file
    
    string genFileName(); // generate random file name
    size_t getFilesize(int fd); // get file size
    
    char* memmap(int fd); // map file to memory using the linux mmap func
    void memunmap(char* iMmappedData,int fd); // unmap file in memory using the linux munmap func 
    
    DB* db;
    bool checkIfTableExists(string &dname,string &tname); // check if table exists
    
public:
    Davedb(); // constructor to create new db
    Davedb(string &iDBName); // constructor to use given db
    ~Davedb(); // destructor to unlink the db 
    void addTable(string &dname,string &tname); // add table to db
    void addTbAttr(string &dname,string &tname, string attributes[] ); // add table attribute to table in db
    
    void printDBs(); // print all the databases 
    
    char* playWithmemmap(); // returns memory mapped file
};

#endif // DAVEDB_HPP
