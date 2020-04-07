#include "db_operations.h"

static int count = 0;

/*
 * Callback function for checkCredentials
 */
static int check(void *NotUsed, int argc, char** argv, char** azColName){
    int i = 0;
    count++;
    return 0;
}


/*
 * Function to check if the user exists and its credentials are correct
 */
int checkCredentials(std::string usr, std::string password){
    int rc;
    sqlite3 *db;
    std::string sql;
    char* errMsg = nullptr;

    // open database connection
    rc = sqlite3_open("pds_db", &db);

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


/*
 * Callback function for readFiles
 */
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


/*
 * This function uses the files function as callback
 * function to modify the list of files structure
 */
int readFiles(){
    int rc;
    sqlite3 *db;
    std::string sql;
    char* errMsg = nullptr;

    // open database connection
    rc = sqlite3_open("pds_db", &db);

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


/*
 * function to add a new user in the database
 */
int addUser(std::string user, std::string password, std::string name, std::string surname){
    int rc;
    sqlite3 *db;
    std::string sql;
    char* errMsg = nullptr;

    // open database connection
    rc = sqlite3_open("pds_db", &db);

    // check the connection has been established
    if (rc != SQLITE_OK) {
        std::cerr << "Can't open the database: " << sqlite3_errmsg(db) << std::endl;
        return -1;
    }

    // prepare sql operation
    sql = "INSERT INTO users VALUES ('" + user + "', '"
          + password + "', '"
          + name + "', '"
          + surname + "')";

    // execute sql statement
    rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
    if(rc != SQLITE_OK){
        std::cout << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return -1;
    }
    sqlite3_close(db);
    return 1;
}


/*
 * function to add one file to the db
 */
int addFile(std::string name, std::string path){
    int rc;
    sqlite3 *db;
    std::string sql;
    char* errMsg = nullptr;

    // open database connection
    rc = sqlite3_open("pds_db", &db);

    // check the connection has been established
    if (rc != SQLITE_OK) {
        std::cerr << "Can't open the database: " << sqlite3_errmsg(db) << std::endl;
        return -1;
    }

    // prepare sql operation
    sql = "INSERT INTO FILES ('NAME', 'PATH') VALUES ('" + name + "', '" + path + "')";

    // execute sql statement
    rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
    if(rc != SQLITE_OK){
        std::cout << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return -1;
    }
    sqlite3_close(db);
    return 1;
}
