#include "keszlet.h"

#include "debugmalloc.h"

void KeszletKezeles()                                               //
{
    bool isDone = false;
    StockListElement *FirstElement = NULL;
    StockListElement *LastElement = NULL;
    FirstElement = ReadStockIntoList(&isDone, &LastElement);

    while(!isDone)
    {
        printf("Mit szeretne csinalni?\n-------------------------\n[1] Keszlet megtekintese\n[2] Uj termek hozzaadasa\n[3] Termek modositasa\n[4] Termek torlese\n[5] Vissza\n>");
        int selection = (getc(stdin) - '0');
        stdinClear();
        clr();
        switch(selection)
        {
            case(1):
                StockView(FirstElement);
                break;
            case(2):
                StockAdd(&LastElement, &FirstElement);
                break;
            case(3):
                StockEdit(FirstElement);
                break;
            case(4):
                StockDelete(&FirstElement, &LastElement);
                break;
            case(5):
                FirstElement != NULL ? SListFreeList(FirstElement): printf("");
                isDone = true;
                break;
        }
    }
}

void StockView(StockListElement *FirstElement)                                      //
{
    for(StockListElement *i = FirstElement; i != NULL; i = i->Next)
    {
        printf("%*s %*s %*d db %*d HUF\n",
               5, i->Product.Id,
               21, i->Product.Name,
               8, i->Product.Count,
               11, i->Product.Price);
    }
}

/*ADD es hozzatartozo fuggvenyek*/

void StockAdd(StockListElement **LastElement, StockListElement **FirstElement)      //
{
    bool isDone = false;
    ProductData newProduct = {NULL, NULL, -1, -1};

    while(!isDone)
    {
        printf("Adja meg az uj termek adatait!\n[1] - Id: %*s\n[2] - Nev: %*s\n[3] - Darabszam: %*d\n[4] - Ar: %*d\n\n[5] - Mentes\n[6] - Vissza\n>",
               42, newProduct.Id != NULL ? newProduct.Id : "[Meg nincs megadva]",
               41, newProduct.Name != NULL ? newProduct.Name : "[Meg nincs megadva]",
               35, newProduct.Count != -1 ? newProduct.Count : -1,
               42, newProduct.Price != -1 ? newProduct.Price : -1);
        int selection = (getc(stdin) - '0');
        stdinClear();
        clr();
        switch(selection)
        {
            case(1):
                SAddID(&newProduct, *FirstElement);
                break;
            case(2):
                SAddName(&newProduct);
                break;
            case(3):
                SAddCount(&newProduct);
                break;
            case(4):
                SAddPrice(&newProduct);
                break;
            case(5):
                if(newProduct.Id != NULL && newProduct.Name != NULL && newProduct.Count != -1 && newProduct.Price != -1)
                {
                    SAddSave(FirstElement, LastElement, &newProduct);
                    isDone = true;
                }else{printf("!Valamelyik parametert meg nem adta meg!\n");}
                break;
            case(6):
                SAddExit(&newProduct);
                isDone = true;
                break;
        }
    }
}

void SAddID(ProductData *newProduct, StockListElement *FirstElement)            //
{
    if(newProduct->Id != NULL)
    {
        free(newProduct->Id);
        newProduct->Id = NULL;
    }

    printf("ID: ");
    newProduct->Id = ReadLimDynStr(5);

    if(SListFind(FirstElement, newProduct->Id) != NULL)
    {
        free(newProduct->Id);
        newProduct->Id = NULL;
        printf("Ez a termek mar letezik, adjon meg masik ID-t! Nyomjon ENTER-t a tovabblepeshez!\n");
        getchar();
    }

    if(newProduct->Id != NULL)
    {
        for(int i = 0; strlen(newProduct->Id) > i; i++)
        {
            if(!isdigit(newProduct->Id[i]))
            {
                free(newProduct->Id);
                newProduct->Id = NULL;

                printf("Rosszul megadott Id! Csak szamokat tartalmazhat!\n");
                break;
            }
        }
    }else{printf("Rosszul megadott Id!\n");}
    clr();
}

void SAddName(ProductData *newProduct)          //
{
    if(newProduct->Name != NULL)
    {
        free(newProduct->Name);
        newProduct->Name = NULL;
    }

    printf("Nev: ");
    newProduct->Name = ReadLimDynStr(20);
    if(newProduct->Name == NULL) {printf("Rosszul megadott nev!\n");}
}

void SAddCount(ProductData *newProduct)         //
{
    printf("Darabszam: ");
    char *temp = ReadLimDynStr(6);
    newProduct->Count = atoi(temp);
    free(temp);

    if(newProduct->Count < 0 || newProduct->Count >= 100000)
    {
        printf("Rosszul megadott darabszam!\n");
        newProduct->Count = -1;
    }
}

void SAddPrice(ProductData *newProduct)         //
{
    printf("Ar: ");
    char *temp = ReadLimDynStr(9);
    newProduct->Price = atoi(temp);
    free(temp);

    if(newProduct->Price <= 0 || newProduct->Price >= 100000000)
    {
        printf("Rosszul megadott ar!\n");
        newProduct->Price = -1;
    }
}

void SAddSave(StockListElement **FirstElement, StockListElement **LastElement, ProductData *newProduct)      //
{
    StockListElement *newElement = (StockListElement*) malloc(sizeof(StockListElement));
    newElement->Product = *newProduct;
    newElement->Next = NULL;
    newElement->Prev = *LastElement;

    if(*FirstElement != NULL)
    {
        (*LastElement)->Next = newElement;
        *LastElement = newElement;
    }
    else
    {
        *FirstElement = newElement;
        *LastElement = newElement;
    }
    SListWriteToFile(*FirstElement);
}

void SAddExit(ProductData *newProduct)
{
    if(newProduct->Id != NULL) free(newProduct->Id);
    if(newProduct->Name != NULL) free(newProduct->Name);
}


/*EDIT es hozzatartozo fuggvenyek*/

void StockEdit(StockListElement *FirstElement)
{
    bool isDone = false;

    while(!isDone)
    {
        printf("Adja meg a modositando termek ID-jat vagy -1-et a visszalepeshez!\n>");
        char *input = ReadLimDynStr(5);
        if(strcmp(input, "-1") != 0)
        {
            StockListElement *ToEdit = SListFind(FirstElement,input);
            if(ToEdit != NULL)
            {
                SEditMenu(ToEdit, FirstElement);
            }
            else
            {
                printf("Nem letezik ilyen ID-val rendelkezo termek.\n");
            }
        }
        else
        {
            isDone = true;
        }
        free(input);
    }
}

void SEditMenu(StockListElement *ToEdit, StockListElement *FirstElement)
{
    bool isDone = false;
    while(!isDone)
    {
        printf("Adja meg a modositando termek adatait!\n[1] - Id: %*s\n[2] - Nev: %*s\n[3] - Darabszam: %*d\n[4] - Ar: %*d\n\n[5] - Vissza\n>",
           42, ToEdit->Product.Id != NULL ? ToEdit->Product.Id : "-1",
           41, ToEdit->Product.Name != NULL ? ToEdit->Product.Name : "-1",
           35, ToEdit->Product.Count != -1 ? ToEdit->Product.Count : -1,
           42, ToEdit->Product.Price != -1 ? ToEdit->Product.Price : -1);
        int select = (getc(stdin) - '0');
        stdinClear();
        clr();
        switch(select)
        {
            case(1):
                SEditID(ToEdit, FirstElement);
                break;
            case(2):
                SEditName(ToEdit);
                break;
            case(3):
                SEditCount(ToEdit);
                break;
            case(4):
                SEditPrice(ToEdit);
                break;
            case(5):
                isDone = true;
                FirstElement != NULL ? SListWriteToFile(FirstElement) : printf("Az utolsó elemet is kitörölte a készletből!\n");
                break;
        }
    }
}

void SEditID(StockListElement *ToEdit, StockListElement *FirstElement)              //
{
    printf("ID: ");
    char *temp = ReadLimDynStr(5);
    bool ContainedNonDigit = false;

    if(temp == NULL)
    {
        printf("Rosszul megadott Id!\n");
        free(temp);
        return;
    }
    else if(SListFind(FirstElement, temp) != NULL)
    {
        free(temp);
        printf("Ez a termek mar letezik, adjon meg masik ID-t! Nyomjon ENTER-t a tovabblepeshez!\n");
        getchar();
        return;
    }

    for(int i = 0; strlen(ToEdit->Product.Id) > i; i++)
    {
        if(!isdigit(ToEdit->Product.Id[i]))
        {
            ContainedNonDigit = true;
            printf("Rosszul megadott Id! Csak számokat tartalmazhat!\n");
            break;
        }
    }

    if(!ContainedNonDigit)
    {
        free(ToEdit->Product.Id);
        ToEdit->Product.Id = temp;
    }
    else free(temp);
    clr();
}

void SEditName(StockListElement *ToEdit)            //
{
    printf("Nev: ");
    char *temp = ReadLimDynStr(20);
    clr();
    if(temp != NULL)
    {
        free(ToEdit->Product.Name);
        ToEdit->Product.Name = temp;
    }
    else
    {
        printf("Rosszul megadott nev!\n");
        free(temp);
    }
}

void SEditCount(StockListElement *ToEdit)           //
{
    printf("Darabszam: ");
    char *temp = ReadLimDynStr(6);
    clr();

    if((atoi(temp) < 0 && atoi(temp) >= 100000) || temp == NULL)
    {
        printf("Rosszul megadott darabszam!\n");
    }
    else
    {
        ToEdit->Product.Count = atoi(temp);
    }

    free(temp);
}

void SEditPrice(StockListElement *ToEdit)           //
{
    printf("Ar: ");
    char *temp = ReadLimDynStr(9);
    clr();

    if((atoi(temp) <= 0 && atoi(temp) >= 100000) || temp == NULL)
    {
        printf("Rosszul megadott ar!\n");
    }
    else
    {
        ToEdit->Product.Price = atoi(temp);
    }
    free(temp);
}

/*TORLES es hozzatartozo fuggvenyek*/

void StockDelete(StockListElement **FirstElement, StockListElement **LastElement)
{
    bool isDone = false;

    while(!isDone)
    {
        printf("Adja meg a kitorlendo termek ID-jat vagy -1-et a visszalepeshez!\n>");
        char *input = ReadLimDynStr(5);
        if(strcmp(input, "-1") != 0)
        {
            StockListElement *ToDelete = SListFind(*FirstElement, input);
            if(ToDelete != NULL)
            {
                if(ToDelete->Product.Count > 0)
                {
                    printf("Biztosan ki szeretne torolni a termeket? Meg %d van keszleten belole.\n[1] - Igen\t[0] - Nem\n>", ToDelete->Product.Count);
                    if((getc(stdin) - '0') == 1)
                    {
                        SDelAndMend(FirstElement, LastElement, ToDelete);
                    }
                    stdinClear();
                }else{ SDelAndMend(FirstElement, LastElement, ToDelete); }
            }
            else
            {
                printf("Nem letezik ilyen ID-val rendelkezo termek!\n");
            }
        }
        else
        {
            isDone = true;
        }
        free(input);
    }
}

void SDelAndMend(StockListElement **FirstElement, StockListElement **LastElement, StockListElement *ToDelete)
{
    if(ToDelete->Prev == NULL && ToDelete->Next == NULL)
    {
        *FirstElement = NULL;
        *LastElement = NULL;
        free(ToDelete->Product.Id);
        free(ToDelete->Product.Name);
        free(ToDelete);
    }
    else if(ToDelete->Prev == NULL)
    {
        (ToDelete->Next)->Prev = NULL;
        *FirstElement = ToDelete->Next;
        free(ToDelete->Product.Id);
        free(ToDelete->Product.Name);
        free(ToDelete);
    }
    else if(ToDelete->Next == NULL)
    {
        (ToDelete->Prev)->Next = NULL;
        *LastElement = ToDelete->Prev;
        free(ToDelete->Product.Id);
        free(ToDelete->Product.Name);
        free(ToDelete);
    }
    else
    {
        (ToDelete->Prev)->Next = ToDelete->Next;
        (ToDelete->Next)->Prev = ToDelete->Prev;
        free(ToDelete->Product.Id);
        free(ToDelete->Product.Name);
        free(ToDelete);
    }
    SListWriteToFile(*FirstElement);
}

/* Készlet lista kezeléséhez szükséges függvények */

StockListElement *ReadStockIntoList(bool *isDone, StockListElement **LastElement)       //
{

    if(!isFileEmpty("keszlet.txt"))
    {
        StockListElement *FirstElement = SListBuild("keszlet.txt", &LastElement);
        if(FirstElement != NULL)
        {
            printf("A keszlet beolvasasa sikeres!\n\n");
            return FirstElement;
        }else{
            printf("A keszlet beolvasasa sikertelen!");
            sleep(2);
            *isDone = true;
        }
    }else{
        printf("Figyelem! A keszlet fajl ures, adjon hozza termekeket!\n\n");
    }
}

StockListElement *SListBuild(char* filename, StockListElement ***LastElement)       //
{
    FILE *file;
    file = fopen(filename, "r");

    int MaxLength = 44;
    char buffer[MaxLength];

    StockListElement *FirstElement = NULL;
    StockListElement *PreviousElement = NULL;

    if(fgets(buffer,MaxLength, file))
    {
        if(buffer[strlen(buffer)] == '\n')
        {
            buffer[strlen(buffer)] = '\0';
        }
        FirstElement = SListAllocate(buffer);
        PreviousElement = FirstElement;
    }

    while(fgets(buffer,MaxLength, file))
    {
        if(buffer[strlen(buffer)] =='\n')
        {
            buffer[strlen(buffer)] = '\0';
        }
        StockListElement *Element = SListAllocate(buffer);
        Element->Prev = PreviousElement;
        PreviousElement->Next = Element;
        PreviousElement = Element;
    }

    fclose(file);

    if(FirstElement->Next == NULL)
    {
        **LastElement = FirstElement;
    }
    else
    {
        **LastElement = PreviousElement;
    }

    return FirstElement;
}

StockListElement *SListAllocate(char *buffer)                               //
{
    char *token;
    ProductData Product;

    token = strtok(buffer, ";");
    Product.Id = (char*) malloc(sizeof(char)*(strlen(token)+1));
    strcpy(Product.Id, token);
    Product.Id[strlen(token)] = '\0';

    token = strtok(NULL, ";");
    Product.Name = (char*) malloc(sizeof(char)*(strlen(token)+1));
    strcpy(Product.Name, token);
    Product.Name[strlen(token)] = '\0';

    token = strtok(NULL, ";");
    Product.Count = atoi(token);

    token = strtok(NULL, ";");
    Product.Price = atoi(token);

    StockListElement *Element = malloc(sizeof(StockListElement));
    Element->Product = Product;
    Element->Next = NULL;
    Element->Prev = NULL;

    return Element;
}

StockListElement *SListFind(StockListElement *FirstElement, char* Id)       //
{
    for(StockListElement *i = FirstElement; i != NULL; i = i->Next)
    {
        if(strcmp(i->Product.Id,Id) == 0)
        {
            return i;
        }
    }
    return NULL;
}

void SListFreeList(StockListElement *FirstElement)                          //
{
    StockListElement *Element = FirstElement;
    for(StockListElement *i = FirstElement->Next ; i != NULL ; i = i->Next)
    {
        free(Element->Product.Id);
        free(Element->Product.Name);
        free(Element);

        Element = i;
    }
    free(Element->Product.Id);
    free(Element->Product.Name);
    free(Element);
}

int SListWriteToFile(StockListElement *FirstElement)                        //
{
    FILE *file;
    file = fopen("temp.txt","w");

    if(file == NULL)
    {
        printf("A file irasa sikertelen!");
        fclose(file);
        return 1;
    }

    for(StockListElement *i = FirstElement; i != NULL; i = i->Next)
    {
        if(i->Next != NULL)
        {
            fprintf(file,"%s;%s;%d;%d\n", i->Product.Id,i->Product.Name, i->Product.Count, i->Product.Price);
        }
        else
        {
            fprintf(file,"%s;%s;%d;%d",  i->Product.Id,i->Product.Name, i->Product.Count, i->Product.Price);
        }
    }
    fclose(file);

    remove("keszlet.txt");
    rename("temp.txt", "keszlet.txt");

    return 0;
}
