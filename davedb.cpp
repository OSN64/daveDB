#include "davedb.hpp"
#include <typeinfo>

Davedb::Davedb()
{
    dbFileName = genFileName();
    init();
}
Davedb::Davedb(string& iDBName)
{
    dbFileName = iDBName;
    init();
}
Davedb::~Davedb()
{
    // return actual lenght of map from db the unmap
    db->~DB();
    memunmap(mmappedData, dbFd);
    closeDBFile();
}
void Davedb::init()
{
    // dbFd = openDBfile("asd.asd.davedb")
    int fd = openDBfile();
    size_t size = getFilesize(fd);
    if(size <= 0)
      exit(33);

    mmappedData = memmap(fd);    
    db = new DB(mmappedData, size);
}
int Davedb::openDBfile()
{
    // open file
    dbFd = open(dbFileName.c_str(), O_RDWR | O_CREAT);
    if (dbFd == -1) {
        perror("Error opening file for writing");
        exit(EXIT_FAILURE);
    }
    cout << "DB file opened: " << dbFileName << endl;
    return dbFd;
}
void Davedb::closeDBFile()
{
    // close file
    close(dbFd);
}
string Davedb::genFileName()
{
    srand(time(NULL));
    ostringstream tmpfilename;
    tmpfilename << "tmp-" << rand() << dbExt;
    return tmpfilename.str();
}
size_t Davedb::getFilesize(int fd)
{
    struct stat st;
    fstat(fd, &st);
    return st.st_size;
}
char* Davedb::memmap(int fd)
{
    size_t filesize = getFilesize(fd);
    char* iMmappedData = // MAP_PRIVATE == read only copy , MAP_SHARED == read and write copy
        (char*)mmap(NULL, filesize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (iMmappedData == MAP_FAILED) {
        perror("Error mapping file to memory");
        exit(EXIT_FAILURE);
    }
    return iMmappedData;
}
void Davedb::memunmap(char* iMmappedData, int fd) // unable to unmap
{
    size_t filesize = getFilesize(fd);
    int rc = munmap(iMmappedData, filesize);
    if (rc < 0) {
        perror("Error unmapping file to memory");
        exit(EXIT_FAILURE);
    }
}
void Davedb::printDBs()
{
//    size_t filesize = getFilesize(dbFd);
    //    write(1, mmappedData, filesize);
    //    for (unsigned int i = 0; i < filesize; ++i) {
    //        cout << mmappedData[i];
    //    }
}
void Davedb::addTable(string& dname, string& tname)
{
    if (!checkIfTableExists(dname, tname))
        return;
}
void Davedb::addTbAttr(string& dname, string& tname, string attributes[])
{
}
bool Davedb::checkIfTableExists(string& dname, string& tname)
{

    return false;
}
char* Davedb::playWithmemmap()
{
    return mmappedData;
}
