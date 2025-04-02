#ifndef KESZLET_H_INCLUDED
#define KESZLET_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <dos.h>
#include "util.h"

typedef struct ProductData              //Egy termék adatai vannak benne
{
    char *Id;
    char *Name;
    int Count;
    int Price;
}ProductData;

typedef struct StockListElement         //Termék típusa, mint egy lista tagja
{
    ProductData Product;
    struct StockListElement *Next;
    struct StockListElement *Prev;
}StockListElement;

/* Készletkezelõ menüpontok függvényei */

void KeszletKezeles();

void StockView(StockListElement*);

void StockAdd(StockListElement**,StockListElement**);
    void SAddID(ProductData*, StockListElement*);
    void SAddName(ProductData*);
    void SAddCount(ProductData*);
    void SAddPrice(ProductData*);
    void SAddSave(StockListElement**, StockListElement**, ProductData*);
    void SAddExit(ProductData*);

void StockEdit(StockListElement*);
    void SEditMenu(StockListElement*, StockListElement*);
    void SEditID(StockListElement*, StockListElement*);
    void SEditName(StockListElement*);
    void SEditCount(StockListElement*);
    void SEditPrice(StockListElement*);

void StockDelete(StockListElement**,StockListElement**);

void SDelAndMend(StockListElement**, StockListElement**, StockListElement*);

/*Készlet lista kezelõ függvények */

StockListElement* ReadStockIntoList(bool*, StockListElement**);

StockListElement* SListBuild(char*, StockListElement***);

StockListElement* SListAllocate(char*);

StockListElement* SListFind(StockListElement*, char*);

void SListFreeList(StockListElement*);

int SListWriteToFile(StockListElement*);

#endif // KESZLET_H_INCLUDED
