#include "elad.h"

#include "debugmalloc.h"

void EladoMod(){
    bool isDone = false;
    bool openReceipt = false;
    StockListElement *FirstItem = NULL;     //A nyugta elsõ eleme
    StockListElement *LastItem = NULL;      //A nyugta utolsó eleme
    StockListElement *FirstElement = NULL;  //A terméklista elsõ eleme
    StockListElement *LastElement = NULL;   //A terméklista utolsó eleme

    FirstElement = ReadStockIntoList(&isDone, &LastElement);

    while(!isDone)
    {
        printf(" | Elado Mod |\n----------------\n");
        printf(" Tetelek:\n----------------\n");
        (FirstItem != NULL) ? RDisplay(FirstItem) : printf("[Nincsenek felvitt tetelek!]\n");
        printf("----------------\n> ");

        char *input = ReadLimDynStr(14);
        if (input != NULL) { EvaluateInput(input, &isDone, &openReceipt, &FirstItem, &LastItem, FirstElement); }
        else { printf("Hibas bemenet!Nyomjon ENTER-t a tovabblepeshez!\n"); getchar(); }
        free(input);
        clr();
    }

}

void RDisplay(StockListElement *FirstItem)
{
    for(StockListElement *i = FirstItem; i != NULL; i = i->Next)
    {
        printf(" %c | %*s %*s %*d db %*d HUF\n",
               (i->Product.Count > 0) ? '+' : '-',
               5, i->Product.Id,
               21, i->Product.Name,
               8, i->Product.Count,
               11, i->Product.Price);
    }
}

void EvaluateInput(char *input, bool *isDone, bool *openReceipt, StockListElement **FirstItem, StockListElement **LastItem, StockListElement *FirstElement)
{
    char *IDin = strtok(input, " ");
    char *CNin = strtok (NULL, " ");
    if((CNin != NULL) && (atoi(CNin) == 0)) {printf("Nullat adott meg darabszamkent! Tovabblepeshez nyomja meg az ENTER-t!\n"); getchar(); return; }
    if((isdigit(input[0]) && (5 >= strlen(input))))
    {
        if(CNin != NULL)
        {
            RAdd(FirstItem, LastItem, FirstElement, IDin, atoi(CNin));
        }else{
            RAdd(FirstItem, LastItem, FirstElement, IDin, 1);
        }
        if(*FirstItem != NULL) { *openReceipt = true; }
    }
    else
    {
        switch(IDin[0])
        {
            case('-'):
                if(input[1] == '-')          //Kilépés esete
                { RExit(isDone, openReceipt, FirstElement); }
                else if(CNin != NULL)                       //Törlés esete
                { char *temp = IDin+1;
                    HasOnlyDigits(CNin) && HasOnlyDigits(temp) ? RStorno(FirstItem, LastItem, temp, CNin) : printf(""); }
                break;
            case('/'):                      //Nyugta lezár esete
                if(*FirstItem != NULL)
                { CloseReceipt(FirstItem, FirstElement); *openReceipt = false; }
                else { printf("Ures nyugta nem zarhato le! Nyomjon ENTER-t a tovabblepeshez!\n"); getchar(); }
                break;
            case('\\'):                     //Számla lezár esete
                if(*FirstItem != NULL)
                { CloseInvoice(FirstItem, FirstElement); *openReceipt = false; }
                else { printf("Ures szamla nem zarhato le! Nyomjon ENTER-t a tovabblepeshez!\n"); getchar(); }
                break;
            default:
                printf("Hibas bemenet! Nyomjon ENTER-t a tovabblepeshez!\n");
                getchar();
                break;
        }
    }
}

void RAdd(StockListElement **FirstItem, StockListElement **LastItem, StockListElement *FirstElement, char *IDin, int cnt)
{
    StockListElement *ToAdd = SListFind(FirstElement, IDin);
    StockListElement *ToAddTo = SListFind(*FirstItem, IDin);
    if(ToAdd != NULL)
    {
        if(ToAddTo == NULL)
        {
            StockListElement *temp = malloc(sizeof(StockListElement));
            temp->Product.Id = malloc(sizeof(char) * (strlen(ToAdd->Product.Id)+1));
            memcpy(temp->Product.Id, ToAdd->Product.Id, (strlen(ToAdd->Product.Id)+1));

            temp->Product.Name = malloc(sizeof(char) * (strlen(ToAdd->Product.Name)+1));
            memcpy(temp->Product.Name, ToAdd->Product.Name, (strlen(ToAdd->Product.Name)+1));

            temp->Product.Count = cnt;
            temp->Product.Price = (ToAdd->Product.Price * cnt);

            if(*FirstItem!= NULL)
            {
                temp->Next = NULL;
                (*LastItem)->Next = temp;
                *LastItem = temp;
            }
            else
            {
                temp->Next = NULL;
                temp->Prev = NULL;
                *FirstItem = temp;
                *LastItem = temp;
            }
        }
        else
        {
            ToAddTo->Product.Count += cnt;
            ToAddTo->Product.Price = (ToAddTo->Product.Count) * ToAdd->Product.Price;
        }
    }
    else
    {
        printf("Nem letezik ilyen ID-val rendelekezo termek!\nNyomjon ENTER-t a tovabblepeshez!\n");
        getchar();
    }
}

void RStorno(StockListElement **FirstItem, StockListElement **LastItem, char *IDin, char *CNin)
{
    StockListElement *Storno = SListFind(*FirstItem, IDin);
    if(Storno == NULL) {
        printf("Nincsen ilyen ID-val rendelkezo termek a blokkon!\nNyomjon ENTER-t a tovabblepeshez!\n");
        getchar();
    }else if(Storno->Product.Count < atoi(CNin))
    {
        printf("Tobb termeket probal eltavolitani, mint a blokkon szerepel!\nNyomjon ENTER-t a tovabblepeshez!\n");
        getchar();
    }else{
        Storno->Product.Price -= (atoi(CNin) * (Storno->Product.Price / Storno->Product.Count));
        Storno->Product.Count -= atoi(CNin);
    }
}

void RExit(bool *isDone, bool *openReceipt, StockListElement *FirstElement)                 //
{
    if((*openReceipt))
    { printf("Lezaratlan blokk, igy nem lephet ki! Tovabblepeshez nyomjon ENTER-t!\n"); getchar(); }
    else
    {
        *isDone = true;
        SListFreeList(FirstElement);
    }
}

void CloseReceipt(StockListElement **FirstItem, StockListElement *FirstElement)
{
    PrintReceipt(*FirstItem);
    UpdateStock(FirstElement, *FirstItem);
    SListWriteToFile(FirstElement);
    SListFreeList(*FirstItem);
    *FirstItem = NULL;
}

void PrintReceipt(StockListElement *FirstItem)                                          //
{
    clr();
    int sum = 0;
    for(StockListElement *i = FirstItem; i != NULL; i = i->Next)
    {
         printf("%c | %*s %*s %*d db %*d HUF\n",
               (i->Product.Count > 0) ? '+' : '-',
               5, i->Product.Id,
               21, i->Product.Name,
               8, i->Product.Count,
               11, i->Product.Price);
        sum += i->Product.Price;
    }
    printf("----------\nVegosszeg: %d HUF\nNyomjon ENTER-t a tovabblepeshez!", sum);
    getchar();
}

void CloseInvoice(StockListElement **FirstItem, StockListElement *FirstElement)
{
    PrintInvoice(*FirstItem);
    UpdateStock(FirstElement, *FirstItem);
    SListWriteToFile(FirstElement);
    SListFreeList(*FirstItem);
    *FirstItem = NULL;
}

void PrintInvoice(StockListElement *FirstItem)
{
    FILE *file;
    file = fopen("szamlak/datum.txt", "w");
    BuyerData Buyer = GetBuyer();
    fprintf(file, "%s\n%s\n%s\n----------\n", Buyer.Name, Buyer.Address, Buyer.TaxNum);
    FreeBuyer(&Buyer);

    int sum = 0;
    for(StockListElement *i = FirstItem; i != NULL; i = i->Next)
    {
        fprintf(file, "%c | %*s %*s %*d db %*d HUF\n",
           (i->Product.Count > 0) ? '+' : '-',
           5, i->Product.Id,
           21, i->Product.Name,
           8, i->Product.Count,
           11, i->Product.Price);
        sum += i->Product.Price;
    }
    fprintf(file, "----------\nVegosszeg: %d HUF", sum);

    fclose(file);
    char *DateStr = GetCurrentDate();
    rename("szamlak/datum.txt", DateStr);
    free(DateStr);
}

BuyerData GetBuyer()
{
    BuyerData Buyer;
    printf("Adja meg a vasarlo nevet!\n> ");
    Buyer.Name = ReadDynStr();
    printf("\nAdja meg a vasarlo lakcimet!\n> ");
    Buyer.Address = ReadDynStr();
    printf("\nAdja meg a vasarlo adoszamat! (max 11 karakter)\n> ");
    Buyer.TaxNum = ReadLimDynStr(11);

    return Buyer;
}

void FreeBuyer(BuyerData *Buyer)
{
    free(Buyer->Name);
    free(Buyer->TaxNum);
    free(Buyer->Address);
}

void UpdateStock(StockListElement *FirstElement, StockListElement *FirstItem)
{
    for(StockListElement *i = FirstItem; i != NULL; i = i->Next)
    {
        StockListElement *temp = SListFind(FirstElement, i->Product.Id);
        temp->Product.Count -= i->Product.Count;
    }
}

char *GetCurrentDate()
{
    time_t seconds;
    struct tm *Time;
    time(&seconds);
    Time = localtime(&seconds);
    char *TimeStr = malloc(sizeof(char) * 25);
    strcpy(TimeStr, "szamlak/");
    char temp[13];
    strftime(temp, 13, "%Y%m%d%H%M",Time);
    strcat(TimeStr, temp);
    strcat(TimeStr, ".txt");
    TimeStr[24] = '\0';
    return TimeStr;
}
