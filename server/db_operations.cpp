
#include "db_operations.h"

static int check(void *NotUsed, int argc, char** argv, char** azColName){
    int i = 0;
    count++;
    return 0;
}


int checkCredentials(std::string usr, std::string password){
    int rc;
    sqlite3 *db;
    std::string sql;
    char* errMsg = nullptr;

    // open database connection
    rc = sqlite3_open("sql_db.db", &db);

    // check the connection has been established
    if (rc != SQLITE_OK) {
        std::cerr << "Can't open the database: " << sqlite3_errmsg(db) << std::endl;
        return -1;
    }

    // prepare sql operation
    sql = "SELECT * from USERS where USERNAME = '" + usr + "' and PASSWORD = '" + password + "';";

    // execute sql statement
    count = 0;
    rc = sqlite3_exec(db, sql.c_str(), check, nullptr, &errMsg);
    if(rc != SQLITE_OK){
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return -1;
    }
    sqlite3_close(db);

    if (count == 1) // username and password were correct
        return 1;
    return 0;
}


static int files(void *NotUsed, int argc, char** argv, char** azColName){
    int i = 0;
    // TODO: mettere i file in una struttura
    //  da mandare al client che possa visualizzare
    //  a video
    // stampo solo i nomi sul server per ora
    // per vedere se funziona
    std::cout << azColName[0] << " = " << argv[i];
    for( i = 0; i < argc ; i++ ){
        std::cout << ", " << azColName[i] << " = " << argv[i];
    }
    std::cout << std::endl;
    return 0;
}


int readFiles(){
    int rc;
    sqlite3 *db;
    std::string sql;
    char* errMsg = nullptr;

    // open database connection
    rc = sqlite3_open("sql_db.db", &db);

    // check the connection has been established
    if (rc != SQLITE_OK) {
        std::cerr << "Can't open the database: " << sqlite3_errmsg(db) << std::endl;
        return -1;
    }

    // prepare sql operation
    sql = "SELECT * from FILES;";

    // execute sql statement
    rc = sqlite3_exec(db, sql.c_str(), files, nullptr, &errMsg);
    if(rc != SQLITE_OK){
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return -1;
    }
    sqlite3_close(db);

    if (count == 1) // username and password were correct
        return 1;
    return 0;
}

