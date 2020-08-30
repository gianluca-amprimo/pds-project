#include <memory>
#include <QtCore/QJsonObject>
#include <QtCore/QFile>
#include <QtCore/QDataStream>
#include "db_operations.h"
#include "Symbol.h"

static std::string db_path = "../pds_db";
const std::string fs_root = "../files/";
static int db_counter = 0;

std::set<std::tuple<std::string, std::string>> file_list;
std::hash<std::string> cypher;
std::string info_name, info_surname;

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
    sql = "SELECT * FROM USERS WHERE USERNAME = '" + usr
          + "' AND PASSWORD = '" + std::to_string(cypher(password))
          + "'";

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

    // argc is 4 because the number of columns is 4
    // ID, NAME, PATH
    db_counter++;

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
    db_counter = 0;

    // execute sql statement
    rc = sqlite3_exec(db, sql.c_str(), files, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return -1;
    }
    sqlite3_close(db);

    if (db_counter >= 1)
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

    if (db_counter == 1) {
        // username already exists
        sqlite3_close(db);
        return 0;
    }

    // prepare sql operation
    sql = "INSERT INTO USERS VALUES ('" + user + "', '"
          + std::to_string(cypher(password)) + "', '"
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
int addFile(std::string name, std::string username) {
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
    sql = "SELECT * FROM FILES WHERE NAME = '" + name + "'";
    db_counter = 0;
    // execute sql statement
    rc = sqlite3_exec(db, sql.c_str(), check, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::cout << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return -1;
    }

    if (db_counter == 1) {
        // file already exists
        sqlite3_close(db);
        return 0;
    } else {
        QFile fo((fs_root + name).c_str());
        fo.open(QIODevice::WriteOnly);
        if(fo.isOpen()) {
            QByteArray byteArrayBuffer;
            QDataStream stream(&byteArrayBuffer, QIODevice::ReadWrite);

            QList<Symbol> emptyFile;
            stream << emptyFile;

            fo.write(byteArrayBuffer);
            fo.close();
        } else {
            return 0;
        }

        // prepare sql operation
        sql = "INSERT INTO FILES ('NAME', 'CREATED_BY') VALUES ('" + name + "', '" + username + "')";

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
    sql = "SELECT * FROM USERS WHERE USERNAME = '" + username
          + "' AND PASSWORD = '" + std::to_string(cypher(password))
          + "';";
    db_counter = 0;

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
        sql = "DELETE FROM USERS WHERE USERNAME = '" + username
              + "' AND PASSWORD = '" + std::to_string(cypher(password))
              + "';";

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

    // prepare sql operation to check if file exists
    sql = "SELECT * FROM FILES WHERE NAME = '" + name
          + "' AND PATH = '" + path
          + "';";
    db_counter = 0;

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
        sql = "DELETE FROM FILES WHERE NAME = '" + name
              + "' AND PATH = '" + path
              + "';";

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


/*
 * Callback function for getPersonalInfo
 */
static int personalInfo(void *NotUsed, int argc, char **argv, char **azColName) {
    // check if argc is correct
    if (argc != 4) {
        std::cerr << "Error in the number of columns" << std::endl;
        return -1;
    }

    db_counter++;

    // argc is 4 because the number of columns is 4
    // USERNAME, PASSWORD, NAME and SURNAME

    // retrieve name and surname of the user
    info_name = argv[2];
    info_surname = argv[3];
    return 0;
}


/*
 * function to read the personal info of user
 * returns a tuple with name and surname if the user exists
 * else a tuple with db_error and the specific error
 */
std::tuple<QString, QString> getPersonalInfo(std::string username){
    std::tuple<QString, QString> result;
    int rc;
    sqlite3 *db;
    std::string sql;
    char *errMsg = nullptr;

    // open database connection
    rc = sqlite3_open(db_path.c_str(), &db);

    // check the connection has been established
    if (rc != SQLITE_OK) {
        std::cerr << "Can't open the database: " << sqlite3_errmsg(db) << std::endl;
        result = std::make_tuple(QString::fromStdString("db_error"), QString::fromStdString("Can't open the database"));
        return result;
    }

    // prepare sql operation to check if user exists
    sql = "SELECT * FROM USERS WHERE USERNAME = '" + username + "';";
    db_counter = 0;

    // execute sql statement
    rc = sqlite3_exec(db, sql.c_str(), personalInfo, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::cout << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        sqlite3_close(db);

        result = std::make_tuple("db_error", errMsg);
        return result;
    }
    sqlite3_close(db);

    if (db_counter == 1) {
        // user exists
        result = std::make_tuple(QString::fromStdString(info_name), QString::fromStdString(info_surname));
        return result;
    } else {
        // user doesn't exists or credentials are wrong
        result = std::make_tuple("db_error", "User does not exist");
        return result;
    }
}


/*
 * function to change a user password
 */
int changePassword(std::string username, std::string oldPassword, std::string newPassword){
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
    sql = "SELECT * FROM USERS WHERE USERNAME = '" + username
          + "' AND PASSWORD = '" + std::to_string(cypher(oldPassword))
          + "';";
    db_counter = 0;

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
        // prepare sql operation to update the password
        sql = "UPDATE USERS "
              "SET PASSWORD = '" + std::to_string(cypher(newPassword))
              + "' WHERE USERNAME = '" + username
              + "';";

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

int checkIfFileExists(std::string name) {
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
    sql = "SELECT * FROM FILES WHERE NAME = '" + name +"';";
    db_counter = 0;

    // execute sql statement
    rc = sqlite3_exec(db, sql.c_str(), files, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return -1;
    }
    sqlite3_close(db);

    if (db_counter >= 1)
        return 1;
    return 0;
}
