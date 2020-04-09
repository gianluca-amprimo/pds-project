#include "db_operations.h"


static std::string db_path = "../pds_db";

static int db_counter = 0;

std::set<std::tuple<std::string, std::string>> file_list;

/*
 * Callback function for checkCredentials
 */
static int check(void *NotUsed, int argc, char **argv, char **azColName) {
    db_counter++;
    return 0;
}


/*
 * Function to check if the user exists and its credentials are correct
 */
int checkCredentials(std::string usr, std::string password) {
    int rc;
    sqlite3 *db;
    std::string sql;
    char *errMsg = nullptr;

    // open database connection
    rc = sqlite3_open(db_path.c_str(), &db);

    // check the connection has been established
    if (rc != SQLITE_OK) {
        std::cerr << "Can't open the database: " << sqlite3_errmsg(db) << std::endl;
        return -1;
    }

    // prepare sql operation
    sql = "SELECT * FROM USERS WHERE USERNAME = '" + usr + "' AND PASSWORD = '" + password + "'";

    // execute sql statement
    db_counter = 0;
    rc = sqlite3_exec(db, sql.c_str(), check, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return -1;
    }
    sqlite3_close(db);

    if (db_counter == 1) // username and password were correct
        return 1;
    return 0;
}


/*
 * Callback function for readFiles
 */
static int files(void *NotUsed, int argc, char **argv, char **azColName) {
    // check if argc is correct
    if (argc != 3) {
        std::cerr << "Error in the number of columns" << std::endl;
        return -1;
    }

    // argc is 3 because the number of columns is 3
    // ID, NAME, PATH

    // create a tuple with name and path of the file
    std::tuple<std::string, std::string> file = std::make_tuple(argv[1], argv[2]);

    // add the tuple to the vector of files
    file_list.insert(file);
    return 0;
}


/*
 * This function uses the files function as callback
 * function to modify the list of files structure
 */
int readFiles() {
    int rc;
    sqlite3 *db;
    std::string sql;
    char *errMsg = nullptr;

    // open database connection
    rc = sqlite3_open(db_path.c_str(), &db);

    // check the connection has been established
    if (rc != SQLITE_OK) {
        std::cerr << "Can't open the database: " << sqlite3_errmsg(db) << std::endl;
        return -1;
    }

    // prepare sql operation
    sql = "SELECT * FROM FILES;";

    // execute sql statement
    rc = sqlite3_exec(db, sql.c_str(), files, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return -1;
    }
    sqlite3_close(db);

    if (db_counter == 1) // username and password were correct
        return 1;
    return 0;
}


/*
 * function to add a new user in the database
 */
int addUser(std::string user, std::string password, std::string name, std::string surname) {
    int rc;
    sqlite3 *db;
    std::string sql;
    char *errMsg = nullptr;

    // open database connection
    rc = sqlite3_open(db_path.c_str(), &db);

    // check the connection has been established
    if (rc != SQLITE_OK) {
        std::cerr << "Can't open the database: " << sqlite3_errmsg(db) << std::endl;
        return -1;
    }

    //check the chosen username does not exist yet
    sql = "SELECT * FROM USERS WHERE username='" + user + "'";
    db_counter = 0;
    rc = sqlite3_exec(db, sql.c_str(), check, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return -1;
    }

    if (db_counter == 1){
        // username already exists
        sqlite3_close(db);
        return 0;
    }

    // prepare sql operation
    sql = "INSERT INTO USERS VALUES ('" + user + "', '"
          + password + "', '"
          + name + "', '"
          + surname + "')";

    // execute sql statement
    rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
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
int addFile(std::string name, std::string path) {
    int rc;
    sqlite3 *db;
    std::string sql;
    char *errMsg = nullptr;

    // open database connection
    rc = sqlite3_open(db_path.c_str(), &db);

    // check the connection has been established
    if (rc != SQLITE_OK) {
        std::cerr << "Can't open the database: " << sqlite3_errmsg(db) << std::endl;
        return -1;
    }

    // prepare sql operation to see if the file does not already exist
    sql = "SELECT * FROM FILES WHERE NAME = '" + name + "' AND PATH = '" + path + "'";
    db_counter = 0;
    // execute sql statement
    rc = sqlite3_exec(db, sql.c_str(), check, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::cout << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return -1;
    }

    if (db_counter == 1){
        // file already exists
        sqlite3_close(db);
        return 0;
    } else {
        // prepare sql operation
        sql = "INSERT INTO FILES ('NAME', 'PATH') VALUES ('" + name + "', '" + path + "')";

        // execute sql statement
        rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
        if (rc != SQLITE_OK) {
            std::cout << "SQL error: " << errMsg << std::endl;
            sqlite3_free(errMsg);
            sqlite3_close(db);
            return -1;
        }
    }
    sqlite3_close(db);
    return 1;
}

int deleteUser(std::string username, std::string password) {
    int rc;
    sqlite3 *db;
    std::string sql;
    char *errMsg = nullptr;

    // open database connection
    rc = sqlite3_open(db_path.c_str(), &db);

    // check the connection has been established
    if (rc != SQLITE_OK) {
        std::cerr << "Can't open the database: " << sqlite3_errmsg(db) << std::endl;
        return -1;
    }

    // prepare sql operation to check if user exists
    sql = "SELECT * FROM USERS WHERE USERNAME = '" + username + "' AND PASSWORD = '" + password + "';";

    // execute sql statement
    rc = sqlite3_exec(db, sql.c_str(), check, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::cout << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return -1;
    }

    if (db_counter == 1) {
        // user exists
        // prepare sql operation to delete the user
        sql = "DELETE FROM USERS WHERE USERNAME = '" + username + "' AND PASSWORD = '" + password + "';";

        // execute sql statement
        rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
        if (rc != SQLITE_OK) {
            std::cout << "SQL error: " << errMsg << std::endl;
            sqlite3_free(errMsg);
            sqlite3_close(db);
            return -1;
        }
    } else {
        // user doesn't exists or credentials are wrong
        return 0;
    }

    sqlite3_close(db);
    return 1;
}


int deleteFile(std::string name, std::string path) {
    int rc;
    sqlite3 *db;
    std::string sql;
    char *errMsg = nullptr;

    // open database connection
    rc = sqlite3_open(db_path.c_str(), &db);

    // check the connection has been established
    if (rc != SQLITE_OK) {
        std::cerr << "Can't open the database: " << sqlite3_errmsg(db) << std::endl;
        return -1;
    }

    // prepare sql operation to check if user exists
    sql = "SELECT * FROM FILES WHERE NAME = '" + name + "' AND PATH = '" + path + "';";

    // execute sql statement
    rc = sqlite3_exec(db, sql.c_str(), check, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::cout << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return -1;
    }

    if (db_counter == 1) {
        // file exists
        // prepare sql operation to delete the file
        sql = "DELETE FROM FILES WHERE NAME = '" + name + "' AND PATH = '" + path + "';";

        // execute sql statement
        rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
        if (rc != SQLITE_OK) {
            std::cout << "SQL error: " << errMsg << std::endl;
            sqlite3_free(errMsg);
            sqlite3_close(db);
            return -1;
        }

        std::tuple<std::string, std::string> file = std::make_tuple(name, path);
        file_list.erase(file);
    } else {
        // file does not exist or name and path are wrong
        return 0;
    }

    sqlite3_close(db);
    return 1;
}
