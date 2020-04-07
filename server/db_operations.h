//
// Created by gianluca on 07/04/20.
//

#ifndef SERVER_DB_OPERATIONS_H
#define SERVER_DB_OPERATIONS_H
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
static int count = 0;
static int check(void *NotUsed, int argc, char** argv, char** azColName);
int checkCredentials(std::string usr, std::string password);
static int files(void *NotUsed, int argc, char** argv, char** azColName);
int readFiles();
#endif //SERVER_DB_OPERATIONS_H
