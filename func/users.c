#include "users.h"

#include "debugmalloc.h"

/* A bejelentkezéshez szükséges függvények */

void Bejelentkezes(bool *isAdmin, bool *isLoggedIn) //Maga a bejelentkezés folyamatát kezeli, kiír a képernyőre
{
    printf("\tUdvozoljuk!\t\n--------------------------\nKerem adja meg a belepeshez szukseges adatait!\n\nFelhasznalonev: ");

    char *input;
    input = ReadLimDynStr(12);   //itt a felhasználónevét írja be
    //stdinClear();

    UserData *User = NULL;
    if(input != NULL) User = ReadUser(input, "profilok.txt");         //Megkeressük és beolvassuk a *User-be a felhasználót, ha nem találtuk meg, akor Null-t ad vissza


    if(User != NULL)
    {
        printf("\nJelszo: ");

        if(input != NULL) free(input);
        input = ReadLimDynStr(24);
        //stdinClear();

        if(isPasswordCheck(User, input) && input != NULL)
        {
            *isAdmin = User->admin;
            *isLoggedIn = 1;
        }
    }

    if(input != NULL) free(input);

    if(User != NULL)
    {
        free(User->username);
        free(User->password);
        free(User);
    }

    clr();
}

UserData *ReadUser(char *input, char *filename)
{
    FILE *file;
    file = fopen(filename, "r");

    const int length = 40;                  //Tudjuk, hogy ez lehet a beolvasott string maximum mérete, mivel: 12(fnev) + 24(pass) + 1(adm) + 2(';') + 1('\0')
    char buffer[length];

    while(fgets(buffer, length, file))
    {
        char *temp = strtok(buffer, ";");   //Itt a beolvasott sorból kiszedi a felhasználónevet
        if(strcmp(temp, input) == 0)
        {
            UserData *User = malloc(sizeof(UserData));

            User->username = malloc(sizeof(char) * (strlen(temp) + 1));
            strcpy(User->username, temp);

            temp = strtok(NULL, ";");                                       //Itt a jelszót
            User->password = malloc(sizeof(char) * (strlen(temp) + 1));
            strcpy(User->password, temp);

            temp = strtok(NULL, ";");       //Itt a jogkört

            User->admin = temp[0] - '0';

            fclose(file);
            return User;
        }
    }

    fclose(file);

    return NULL;
}

bool isPasswordCheck(UserData *User, char *input) //Ellenőrzi, hogy a megadott felhazsnálónévhez tartozik-e a megadott jelszó
{
    return (strcmp(User->password, input) == 0);
}


/* A userek szerkesztéséhez szükséges függvények*/

void FelhasznaloKezeles()
{
    bool isDone = false;
    while(!isDone)
    {
        printf("Mit szeretne csinalni?\n-------------------------\n[1] Felhasznalok megtekintese\n[2] Uj felhasznalo hozzaadasa\n[3] Felhasznalo modositasa\n[4] Felhasznalo torlese\n[5] Vissza\n> ");
        int selection = (getchar() - '0');
        stdinClear();
        clr();
        switch(selection)
        {
            case(1):
                UserView("profilok.txt");
                break;
            case(2):
                UserAdd("profilok.txt");
                break;
            case(3):
                UserEdit("profilok.txt");
                break;
            case(4):
                UserDelete("profilok.txt");
                break;
            case(5):
                isDone = true;
                break;
        }
    }
}


void UserView(char *filename)
{
    FILE *file;
    file = fopen(filename, "r");

    const int length = 40;                  //Tudjuk, hogy ez lehet a beolvasott string maximum mérete, mivel: 12(fnev) + 24(pass) + 1(adm) + 2(';') + 1('\0')
    char buffer[length];

    printf(" S. | Felhasznalonev | Jelszo | Jogkor\n");

    int cnt = 1;

    while(fgets(buffer, length, file))
    {
        char *Username = strtok(buffer,";");
        char *Password = strtok(NULL,";");
        char *Admin = strtok(NULL,";");

        if(Admin[0] - '0' == 0)
        {
            strcpy(Admin, "Elado\n");
        }else{
            strcpy(Admin, "Adminisztrator\n");
        }

        printf("%d. %s %s %s\n", cnt, Username, Password, Admin);
        cnt++;
    }

    fclose(file);
}
void UserAdd(char *filename)
{
    FILE *file;
    file = fopen(filename,"a");

    UserData newUser;

    bool isUniqueUsername = false;
    while(!isUniqueUsername)
    {
        printf("Adja meg az uj felhasznalo adatait!\n\nFelhasznalonev(max 12 karakter): ");
        newUser.username = ReadLimDynStr(12);
        if((UserSearch("profilok.txt", newUser.username) == NULL) && ((strchr(newUser.username, ' ') == NULL) && !HasDigits(newUser.username)))
        {
            isUniqueUsername = true;
        }
        else
        {
            free(newUser.username);
            clr();
            printf("Ez a felhasznalonev mar foglalt vagy illegalis karaktert tartalmaz! Valasszon masikat!\n");
        }
    }

    bool isCorrectInput = false;
    while(!isCorrectInput)
    {
        printf("\nJelszo(max 24 karakter): ");
        newUser.password = ReadLimDynStr(24);
        if(strchr(newUser.password, ' ') != NULL)
        {
            free(newUser.password);
            printf("Illegalis karakter!");
        }else { isCorrectInput = true; }
    }

    printf("\nMilyen jogkort kapjon?\n1. Elado\t2. Adminisztrator\n> ");
    int sel = (getchar() - '0');
    stdinClear();
    if(sel == 1){
        newUser.admin = 0;
    }else{
        newUser.admin = 1;
    }

    fprintf(file, "\n%s;%s;%d",newUser.username,newUser.password,newUser.admin);

    free(newUser.username);
    free(newUser.password);

    fclose(file);
    clr();
}

void UserEdit(char *filename)
{
    FILE *fileO;
    fileO = fopen(filename, "r");

    FILE *fileT;
    fileT = fopen("temp.txt", "w");

    UserView(filename);

    printf("\nValassza ki melyik felhasznalot szeretne modositani!\n");
    int selU;
    scanf("%d",&selU);
    stdinClear();
    clr();

    int len = 40;
    char buffer[len];

    int cntr = 1;
    while(fgets(buffer, len, fileO))
    {
        if(cntr != selU)
        {
            fprintf(fileT, "%s",buffer);
        }
        else
        {
            char *Username = strtok(buffer,";");
            bool newUsername = false;
            char *Password = strtok(NULL,";");
            bool newPassword = false;
            char *Admin = strtok(NULL,";");
            int sel = 0;

            while(sel != 4)
            {
                printf("%s;%s;%s\n\n", Username,Password,Admin);
                printf("Melyik adatot szeretne modositani?\n[1] Felhasznalonev\n[2] Jelszo\n[3] Jogkor\n[4] Kilepes\n> ");
                scanf("%d", &sel);
                stdinClear();

                switch(sel)
                {
                    case(1): ;
                        printf("Adja meg az uj felhasznalonevet(max 12 karakter):\n> ");
                        char *temp = ReadLimDynStr(12);
                        if(UserSearch("profilok.txt", temp) != NULL)
                        {
                            temp != NULL ? free(temp) : printf("");
                            printf("Ez a felhasznalonev mar foglalt! Valasszon masikat!\n");
                            sleep(1);
                        }
                        else if((strchr(temp, ' ') != NULL) || HasDigits(temp))
                        {
                            printf("Illegalis karakter!");
                            temp != NULL ? free(temp) : printf("");
                            sleep(1);
                        }
                        else {
                            Username = temp;
                            newUsername = true;
                        }
                        break;
                    case(2): ;
                        printf("Adja meg az uj jelszot(max 24 karakter):\n> ");
                        char *tempp = ReadLimDynStr(24);
                        if(strchr(tempp, ' ') != NULL)
                        {
                            printf("Illegalis karakter!");
                            free(tempp);
                            sleep(1);
                        }else {
                            Password = tempp;
                            newPassword = true;
                        }
                        break;
                    case(3):
                        printf("Adja meg az uj jogkort:\n1. Elado\n2. Adminisztrator\n> ");
                        scanf("%d", &sel);
                        if(sel == 1)
                        {
                            strcpy(Admin, "0");
                        }
                        else
                        {
                            strcpy(Admin, "1");
                        }
                        stdinClear();
                        break;
                }
                clr();
            }
            if(GetFileLength("profilok.txt") > selU)
            {
                Admin[1] = '\n';
                fprintf(fileT, "%s;%s;%s", Username,Password,Admin);
            }
            else
            {
                fprintf(fileT, "%s;%s;%s", Username,Password,Admin);
            }
            if(newUsername) free(Username);
            if(newPassword) free(Password);
        }
        cntr++;
    }

    fclose(fileO);
    fclose(fileT);

    remove("profilok.txt");
    rename("temp.txt","profilok.txt");

    clr();
}

void UserDelete(char *filename)
{
    FILE *fileO;
    fileO = fopen(filename, "r");

    FILE *fileT;
    fileT = fopen("temp.txt", "w");

    UserView(filename);

    printf("Valassza ki melyik felhasznalot szeretne kitorolni!\n> ");
    int sel;
    scanf("%d",&sel);
    stdinClear();

    int len = 40;
    char buffer[len];

    int cntr = 1;
    while(fgets(buffer, len, fileO))
    {
        if(sel == GetFileLength("profilok.txt") && cntr == sel-1)
        {
            buffer[strcspn(buffer, "\n")] = 0;
            fprintf(fileT, "%s",buffer);
        }
        else if(cntr != sel)
        {
            fprintf(fileT, "%s",buffer);
        }
        cntr++;
    }

    fclose(fileO);
    fclose(fileT);

    remove("profilok.txt");
    rename("temp.txt","profilok.txt");

    clr();
}

char *UserSearch(char *filename, char *Username)
{
    FILE *file;
    file = fopen(filename,"r");

    int len = 40;
    char buffer[len];

    while(fgets(buffer, len, file))
    {
        if(strcmp(Username, strtok(buffer,";")) == 0)
        {
            fclose(file);
            return strtok(buffer,";");
        }
    }

    fclose(file);
    clr();
    return NULL;
}
