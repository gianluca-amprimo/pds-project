#ifndef SERVER_DB_OPERATIONS_H
#define SERVER_DB_OPERATIONS_H

#include <iostream>
#include <sqlite3.h>

// TODO: ci va una struttura per quando vengo letti i
//  file e probabilmente ha bisogno di mutua esclusione

// function to check credentials of the user
int checkCredentials(std::string usr, std::string password);

// function to retrieve the list of files
int readFiles();

// function to add a user
// returns 1 for success and -1 for failure
int addUser(std::string user, std::string password, std::string name, std::string surname);

// function to add a file to the db
// returns 1 for success and -1 for failure
int addFile(std::string name, std::string path);

int deleteUser(std::string username, std::string password);

#endif //SERVER_DB_OPERATIONS_H
