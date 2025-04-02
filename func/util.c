#include "util.h"

#include "debugmalloc.h"

/* ilyen segítő jellegű függvények */

#ifdef _WIN32
    void clr() //Cleareli a consolet
    {
        system("cls");
    }
#elif __linux__
    void clr()
    {
        system("clear");
    }
#else
    void clr()
    {
        printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    }
#endif

void stdinClear()
{
    char ch;
    do
    {
        ch = getchar();
    }while(ch != '\n' && ch != EOF);
}

/* filekezelõ függvények */

void CheckDefaultFiles()            //Ha a DoesFileExist függvény nemet ad vissza létrehozza a vizsgált filet
{
    if(!DoesFileExist("profilok.txt") || isFileEmpty("profilok.txt"))
    {
        FILE *file;
        file = fopen("profilok.txt", "w");
        fputs("admin;admin;1",file);
        fclose(file);
    }
    if(!DoesFileExist("keszlet.txt"))
    {
        FILE *file;
        file = fopen("keszlet.txt", "w");
        fclose(file);
    }
    if(DoesFileExist("temp.txt"))
    {
        remove("temp.txt");
    }
    if(!DoesFolderExist("szamlak"))
    {
        CreateFolder("szamlak");
    }
}

bool DoesFileExist(char *filename)  //Megnézi, hogy létezik-e egy file, ha igen true, ha nem false a return értéke
{
    FILE *file;
    if((file = fopen(filename, "r")) != NULL)
    {
        fclose(file);
        return true;
    }
    return false;
}

bool DoesFolderExist(char *folderPath)
{
    struct stat st = {0};
    return (stat(folderPath, &st) == -1) ? 0 : 1;
}

void CreateFolder(char *folderPath)
{
    mkdir(folderPath);
}

bool isFileEmpty(char *filename)    //Megnézi üres-e egy fájl, ha igen true, ha nem false a return értéke
{
    FILE *file;
    file = fopen(filename,"r");

    int c = fgetc(file);

    fclose(file);

    return c == EOF ? true : false;
}

int GetFileLength(char *filename)   //Visszaadja egy file sorainak számát
{
    FILE *file;
    file = fopen(filename, "r");

    const int length = 40;
    char buffer[length];

    int cnt = 0;

    while(fgets(buffer, length, file))
    {
        cnt++;
    }

    fclose(file);
    return cnt;
}

/* generikus memóriakezelõ függvények */

char *ReadDynStr()                  //Beolvas egy stringet dinamikusan és visszadja azt
{
    char *str = (char*) malloc(sizeof(char));
    str[0] = '\0';

    int cnt = 0;

    char newChar;

    while(scanf("%c", &newChar) == 1 && newChar != '\n')
    {
        char *temp = (char*) malloc(sizeof(char) * (cnt+1+1));
        for(int i = 0; i < cnt; i++) temp[i] = str[i];
        free(str);
        str = temp;
        str[cnt] = newChar;
        str[cnt+1] = '\0';
        cnt++;
    }
    if(str[0] != '\0') {return str;}
    else {free(str); return NULL;}
}

char *ReadLimDynStr(int limit)      //Beolvas dinamikusan egy stringet egy bizonyos karakterszámig
{
    char *str = (char*) malloc(sizeof(char));
    str[0] = '\0';

    int cnt = 0;
    char newChar;

    while(scanf("%c", &newChar) == 1 && newChar != '\n' && cnt != limit)
    {
        char *temp = (char*) malloc(sizeof(char) * (cnt+1+1));
        for(int i = 0; i < cnt; i++) temp[i] = str[i];
        free(str);
        str = temp;
        str[cnt] = newChar;
        str[cnt+1] = '\0';
        cnt++;
    }
    if(str[0] != '\0') {return str;}
    else {free(str); return NULL;}
}

bool HasDigits(char *str)
{
    for(int i = 0; strlen(str) > i; i++)
    {
        if(isdigit(str[i]))
        {
            return true;
        }
    }
    return false;
}

bool HasOnlyDigits(char *str)
{
    for(int i = 0; strlen(str) > i; i++)
    {
        if(!isdigit(str[i]))
        {
            return false;
        }
    }
    return true;
}
