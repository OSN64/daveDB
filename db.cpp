#include "db.hpp"

DB::DB(char* dbMemMap, size_t size)
{
    dbCount = 0;
    initRegex();
    readDb(dbMemMap, size);
    //
    string insDB = "The_Boss";
    string insTb = "RON";
    string insAttr = "id class direction";
    string insVals = "1 back right";
    Database* DB = insertDb(insDB);
    Table* tb = insertTable(DB,insTb);
    insertTableAttr(tb,insAttr);
    insert(insDB, insTb, insVals);
    printDBs();
}

DB::~DB()
{
    delDbs();
}

void DB::delDbs()
{
    //    Database* le = databases[0];
    //    string* c = &le->name;
    //    cout << "del db: " << c << le->name << endl;
    //    Table* ss = databases[0]->tables[0];
    std::vector<Database*>* v = &databases;
    for (uint i = 0; i < v->size(); i++) // cant figure out deleting db and tables
        delete (*v)[i];
    //    cout << le << endl;
    //    delete (*&le);
    //    delete le;
    //    for (std::vector<Database*>::iterator dbit = databases.begin(); dbit != databases.end(); ++dbit) {
    //        for (std::vector<Table*>::iterator tbit = (*dbit)->tables.begin(); tbit != (*dbit)->tables.end(); ++tbit)
    // {
    //            delete (*dbit);
    //        }
    //        delete (*dbit);
    //    }
    //    for (uint di = 0; di < databases.size(); di++) {
    //        for (uint ti = 0; ti < databases[di]->tables.size(); ti++) {
    //            delete databases[di]->tables[ti];
    //        }
    //        delete databases[di];
    //    }
    //    cout << "del db: " << *c << endl;
    //    string name = "to_be";
    //    Table* want = new Table(name);
    //    Table
    //    cout << "table: " << want->name << endl;
    //
    //    delete want;
    //
    //    cout << "del table: " << want->name << endl;
}
// void DB::delDb(Database*&)
// void DB::delTb(Database*&,Table*&)
void DB::readDb(char* dbMemMap, size_t size)
{
    Database* currDb;
    Table* currTb;
    string line;
    for (unsigned int i = 0; i < size; ++i) {
        if (dbMemMap[i] == '\n') {
            switch (line[0]) { // change to if else cause use of STARTCHAR
            case '#':          // start of db attributes

                parseTypeLine(line, currDb, currTb);
                break;

            default: // start of data
                // if currdb is empty throw db coruption error
                parseDataLine(line, currTb);
            }
            line = "";
            //            memset (line,'\0',sizeof(line));
            continue;
        }
        line += dbMemMap[i];
    }
}

bool DB::parseTypeLine(string& line, Database*& currDb, Table*& currTb)
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

    if (ret && match[3] == "attr") {
        currTb->attributes = splitstr(&match[5].str()[0], DELIMITER.c_str());
    }

    return true;
}
bool DB::parseDataLine(string& line, Table*& currTb)
{
    std::vector<string> dataRow = splitstr(&line[0], DELIMITER.c_str());
    currTb->rows.push_back(dataRow);
    return true;
}
vector<string> DB::splitstr(char* str, const char* delimiter)
{
    std::vector<string> res;
    char* pch;
    pch = strtok(str, delimiter);
    while (pch != NULL) {
        res.push_back((string)pch);
        pch = strtok(NULL, delimiter);
    }
    return res;
}
Database* DB::insertDb(string& name)
{
    Database* newDB = findDb(name);
    if (newDB != 0)   // traverse databases to find if database exists
        return newDB; // return table

    newDB = new Database(name);

    assert(newDB != NULL); // If unable to allocate memory,
    // terminate the program

    databases.push_back(newDB);
    dbCount++;
    return newDB;
}
Table* DB::insertTable(Database*& thisDb, string& name)
{

    Table* newTable = findTable(thisDb, name);
    if (newTable != 0)   // traverse db to find if table exists
        return newTable; // return table

    newTable = new Table(name);

    assert(newTable != NULL); // If unable to allocate memory,
                              // terminate the program
    thisDb->tables.push_back(newTable);
    thisDb->tbCount++;
    return newTable;
}
bool DB::insertTableAttr(Table*& table, string& attr)
{
    table->attributes = splitstr(&attr[0], DELIMITER.c_str()); // change this ,, looks weird
    return true;
}
Table* DB::findTable(Database*& thisDb, string& name)
{
    Table* current = nullptr; // pointer to current table
    uint i = 0;               // index
    while (i < thisDb->tables.size()) {
        if (thisDb->tables[i]->name.compare(name) == 0) {
            current = thisDb->tables[i];
            break;
        }
        i++;
    }
    return current;
}
Database* DB::findDb(string& name)
{
    Database* current = nullptr; // pointer to current database
    uint i = 0;                  // index
    while (i < databases.size()) {
        if (databases[i]->name.compare(name) == 0) { // if name is same
            current = databases[i];
            break;
        }
        i++;
    }
    return current;
}
bool DB::insert(string& isDB, string& isTb, string& isVals)
{
    Database* DB = findDb(isDB);
    if(DB == 0) return false;
    Table* table = findTable(DB, isTb);
    if(table == 0) return false;
    parseDataLine(isVals, table);
    return true;
}
void DB::printDBs()
{
    // print
    Database* current; // pointer to current table

    uint i = 0; // index
    while (i < databases.size()) {
        current = databases[i];
        cout << "DB: " << current->name << endl;
        printDBTables(current);
        i++;
    }
}
void DB::printDBTables(Database*& db)
{
    Table* current; // pointer to current table
    uint i = 0;     // index
    while (i < db->tables.size()) {
        current = db->tables[i];
        cout << " Table: " << current->name << endl;
        printTableAttr(current);
        printTableVals(current);
        i++;
    }
}
void DB::printTableAttr(Table*& table)
{
    string current; // pointer to current attr
    uint i = 0;     // index
    cout << "  Attr: ";
    while (i < table->attributes.size()) {
        current = table->attributes[i];
        cout << current << " ";
        i++;
    }
    cout << endl;
}
void DB::printTableVals(Table*& table)
{
    std::vector<string> current; // pointer to current row
    uint i = 0;                  // row index

    while (i < table->rows.size()) {
        current = table->rows[i];
        uint ci = 0; // row value
        cout << "  Data: ";

        while (ci < current.size()) {
            cout << current[ci] << " ";
            ci++;
        }
        cout << endl;
        i++;
    }
}
void DB::initRegex()
{
    // regex
    //    regex rxFinddb("(#)( )((?:[a-z][a-z0-9_]*))( )(db)( )((?:[a-z][a-z0-9_]*))",regex_constants::ECMAScript);
    //    cout << "inti"<< endl;
}
