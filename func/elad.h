#ifndef ELAD_FUNC_H
#define ELAD_FUNC_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#include "keszlet.h"
#include "util.h"

typedef struct BuyerData
{
    char* Name;
    char* Address;
    char* TaxNum;
}BuyerData;

void EladoMod();
void RDisplay(StockListElement*);
void EvaluateInput(char*, bool*, bool*, StockListElement**, StockListElement**, StockListElement*);
    void RAdd(StockListElement**, StockListElement**, StockListElement*, char*, int);
    void RStorno(StockListElement**, StockListElement**, char*, char*);
    void RExit(bool*, bool*, StockListElement*);
    void CloseReceipt(StockListElement**, StockListElement*);
    void PrintReceipt(StockListElement*);
    void CloseInvoice(StockListElement**, StockListElement*);
    void PrintInvoice(StockListElement*);
        BuyerData GetBuyer();
        void FreeBuyer(BuyerData*);
    void UpdateStock(StockListElement*, StockListElement*);
    char *GetCurrentDate();

#endif
