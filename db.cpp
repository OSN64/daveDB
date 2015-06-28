#include "db.hpp"

DB::DB(char* dbMemMap, size_t size)
{
    dbFirst = NULL;
    dbLast = NULL;
    dbCount = 0;
    initRegex();
    readDb(dbMemMap, size);
    //    string d = "ll";
    //    insertDb(d);
    //    insertDb(d);
    printDBs();
}

DB::~DB()
{
    delDbs();
}

void DB::delDbs()
{
}
void DB::readDb(char* dbMemMap, size_t size)
{
    //    string currDb = "";
    //    string currTb = "";

    Database* currDb;
    Table* currTb;
    string line;
    for (unsigned int i = 0; i < size; ++i) {
        if (dbMemMap[i] == '\n') {
            switch (line[0]) {
            case '#': // start of db attributes
            {
                parseLine(line, currDb, currTb);
                //                cout << "mem mid2: " << currDb << currDb->name << endl;
                //                                cout << line << endl;
                break;
            }
            default: // start of data
                     //                cout << "data: " << line << endl;
                // if currdb is empty throw db coruption error
                int i = 2;
                i++;
                //                cout << "here: " << currDb->tbCount;
            }
            line = "";
            //            memset (line,'\0',sizeof(line));
            continue;
        }

        //        cout << dbMemMap[i];
        line += dbMemMap[i];
    }
}

bool DB::parseLine(string line, Database*& currDb, Table*& currTb)
{
    regex rxFinddb("(#)( )((?:[a-z][a-z0-9_]*))( )(db)( )((?:[a-z][a-z0-9_]*))", regex_constants::ECMAScript);
    regex rxFindtb("(#)( )((?:[a-z][a-z0-9_]*))( )(table)( )((?:[a-z][a-z0-9_]*))", regex_constants::ECMAScript);
    regex rxFindattr("(#)( )(attr)( )s*(.*)", regex_constants::ECMAScript);
    std::smatch match;
    // db check
    bool ret = regex_search(line, match, rxFinddb);
    if (ret) {
        if (match[3] == "start") {
            string dbName = match[7].str();
            currDb = insertDb(dbName);
        } else if (match[3] == "end") {
            currDb = NULL;
        }
    }
    ret = false;

    // table check
    ret = regex_search(line, match, rxFindtb);

    if (ret) {
        if (match[3] == "start") {
            string tableName = match[7].str();
            currTb = insertTable(currDb, tableName);
        } else if (match[3] == "end") {
            currTb = NULL;
        }
    }
    ret = false;

    // attribute check
    ret = regex_search(line, match, rxFindattr);

    string delimiter = " ";

    if (ret && match[3] == "attr") {
        //        cout << match.str() << match.size() << endl;
        //        cout << match[5].str() << endl;
        cout << splitstr(&match[5].str()[0], delimiter.c_str()) << endl;
        //        for (int i = 3; i < match.size(); ++i) {
        //            string attr = match[i].str();
        // cout << "-:" << match[i] << match.size() << endl;
        //            currTb->attributes.push_back(match[i]);
        //            currTb = match[7]; // name of the db
        //        }
    }

    return true;
}
const char* DB::splitstr(char* str, const char* delimiter)
{
    //    char res[strlen(str)];
    char* res[strlen(str)][10];
    int ln = strlen(str);
    char* pch;
    int i = 0;
    pch = strtok(str, delimiter);
    while (pch != NULL) {
        printf("%s\n", pch);
        //        res[i] = pch;
        strcpy(res[i], pch);
        pch = strtok(NULL, delimiter);
        i++;
    }
    return str;
}
Database* DB::insertDb(string& name)
{
    //    traverse dbs to find if db name exists
    Database* newDB;
    newDB = new Database(name);

    assert(newDB != NULL); // If unable to allocate memory,
    // terminate the program

    if (dbFirst == NULL) { // if the list is empty, newNode is
                           // both the first and last node
        dbFirst = newDB;
        dbLast = newDB;
        dbCount++; // increment count
    } else {       // the list is not empty, insert newNode after last

        dbLast->next = newDB; // insert newNode after last
        dbLast = newDB;       // make last point to the actual last node
        dbCount++;            // increment count
    }
    return newDB;
}
Table* DB::insertTable(Database*& thisDb, string& name)
{
    //    traverse dbs to find if db name exists
    Table* newTable;
    newTable = new Table(name);
    //
    assert(newTable != NULL);         // If unable to allocate memory,
                                      // terminate the program
    if (thisDb->tableFirst == NULL) { // if the list is empty, newNode is
                                      //                                      // both the first and last node
        thisDb->tableFirst = newTable;
        thisDb->tableLast = newTable;
        thisDb->tbCount++; // increment count

    } else { // the list is not empty, insert newNode after last

        thisDb->tableLast->next = newTable; // insert newNode after last
        thisDb->tableLast = newTable;       // make last point to the actual last node
        thisDb->tbCount++;                  // increment count
    }
    return newTable;
}
void DB::printDBs()
{
    // print
    Database* current; // pointer to traverse the list

    current = dbFirst; // set current so that it points to
    // the first node
    while (current != NULL) // while more data to print
    {
        cout << "DB: " << current->name << endl;
        printDBTables(current);
        current = current->next;
    }
}
void DB::printDBTables(Database*& db)
{
    // print
    Table* current; // pointer to traverse the list

    current = db->tableFirst; // set current so that it points to
    // the first node
    while (current != NULL) // while more data to print
    {
        cout << "  Table: " << current->name << endl;
        current = current->next;
    }
}
void DB::initRegex()
{
    // regex
    //    regex rxFinddb("(#)( )((?:[a-z][a-z0-9_]*))( )(db)( )((?:[a-z][a-z0-9_]*))",regex_constants::ECMAScript);
    //    cout << "inti"<< endl;
}