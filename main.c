#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "func/elad.h"
#include "func/keszlet.h"
#include "func/users.h"
#include "func/util.h"

#include "func/debugmalloc.h"

void StartMenu(bool*, bool*);           // a program elindításakor megjelenő menü
void Menu(bool*,bool*);                 // a bejelentkezés után megjelenő "main" menü
void Kijelentkezes(bool*, bool*);       // a main menüben lévő kijelentkezés menüpont
void Kilepes();                         // mindkét menüben megjelenő kilépés menüpont

int main()
{
    bool isLoggedIn = 0;
    bool isAdmin = 0;
    CheckDefaultFiles();

    while(!isLoggedIn)
    {
        StartMenu(&isAdmin, &isLoggedIn);
        while(isLoggedIn)
        {
            CheckDefaultFiles();
            Menu(&isAdmin, &isLoggedIn);
        }
    }
    return 0;
}
/*--Start Menu definíciói--*/


void StartMenu(bool *isAdmin, bool *isLoggedIn) //A program indításakor feljövő menü, bejelentkezés nélkül innen nincsen út tovább
{
    printf("[1] - Bejelentkezes\n[2] - Kilepes\n> ");
    int selection = (getchar() - '0');
    stdinClear();
    clr();

    switch(selection)
    {
        case 1:
            Bejelentkezes(isAdmin, isLoggedIn);
            break;
        case 2:
            Kilepes();
            break;
    }

}

/*--Main Menu definíciói--*/


void Menu(bool *isAdmin, bool *isLoggedIn) //A login után megjelenő menü, a három fő funkció innen érhető el
{
    printf("~-----Menu-----~\n");
    printf("[1] - Ertekesitesi mod\n[2] - Keszletkezeles\n[3] - Felhasznalok kezelese\n[4] - Kijelentkezes\n[5] - Kilepes\n> ");

    int selection = (getchar() - '0');
    stdinClear();
    clr();
        switch(selection)
        {
            case 1:
                isFileEmpty("keszlet.txt") ? printf("A keszlet nem tartalmaz termekeket, adjon hozza!") : EladoMod();
                break;
            case 2:
                *isAdmin ? KeszletKezeles() : printf("Nincs hozzaferese");
                break;
            case 3:
                *isAdmin ? FelhasznaloKezeles() : printf("Nincs hozzaferese");
                break;
            case 4:
                Kijelentkezes(isLoggedIn, isAdmin);
                break;
            case 5:
                Kilepes();
                break;
        }
        clr();
}

void Kijelentkezes(bool *isLoggedIn, bool *isAdmin) //Rákérdez arra, hogy biztos-e és ha igen utána kijelentkezik és visszadob a start menübe
{
    bool isDone = false;
    while(!isDone)
    {
        printf("Biztosan ki szeretne jelentkezni?\n\t[1] - Igen\t[0] - Nem\n> ");
        int selection = (getchar() - '0');
        stdinClear();
        switch(selection)
        {
            case 0:
                isDone = true;
                break;
            case 1:
                *isLoggedIn = false;
                *isAdmin = false;
                isDone = true;
                break;
        }
    }
}

void Kilepes() //Egyszerűen csak kilép a programból
{
    bool isDone = false;
    while(!isDone)
    {
        printf("Biztosan ki szeretne lepni?\n\t[1] - Igen\t[0] - Nem\n> ");
        int selection = (getchar() - '0');
        stdinClear();
        switch(selection)
        {
            case 0:
                isDone = true;
                break;
            case 1:
                exit(0);
                break;
        }
    }
}
