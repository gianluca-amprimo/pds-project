#ifndef SERVER_DB_OPERATIONS_H
#define SERVER_DB_OPERATIONS_H

#include <iostream>
#include <set>
#include <tuple>

#include <sqlite3.h>

// data structure for the files available on the server
extern std::set<std::tuple<std::string, std::string>> file_list;

// function to check credentials of the user
int checkCredentials(std::string usr, std::string password);

// function to retrieve the list of files
// updates the file_list
int readFiles();

// function to add a user
// returns 1 for success and -1 for failure
int addUser(std::string user, std::string password, std::string name, std::string surname);

// function to add a file to the db
// returns 1 for success -1 for failure and 0 if file already exists
int addFile(std::string name, std::string path);

// function to delete an existing user
// returns 1 for user deleted -1 for db error and 0 if user does not exist
int deleteUser(std::string username, std::string password);

// function to delete existing file
// returns 1 for file deleted -1 for db error and 0 if file does not exist
int deleteFile(std::string name, std::string path);

// function to retrieve the user name and surname
// returns a tuple with the info or error if the user does not exist
std::tuple<std::string, std::string> getPersonalInfo(std::string username);

// function to change the password of a user
// returns 1 for success -1 for failure and 0 if the user does not exist
int changePassword(std::string username, std::string oldPassword, std::string newPassword);

#endif //SERVER_DB_OPERATIONS_H
