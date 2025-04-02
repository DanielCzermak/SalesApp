#ifndef USERS_FUNC_H
#define USERS_FUNC_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "util.h"

typedef struct UserData
{
    char *username;         //Maximum 12 karakter
    char *password;         //Maximum 24 karakter
    bool admin;
}UserData;

void Bejelentkezes(bool*,bool*);
UserData *ReadUser(char*, char*);
bool isPasswordCheck(UserData*, char*);

void FelhasznaloKezeles();
void UserView(char*);
void UserAdd(char*);
void UserEdit(char*);
void UserDelete(char*);
char *UserSearch(char*, char*);

#endif
