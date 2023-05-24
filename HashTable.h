#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <iostream>
#include <list>
using namespace std;

#include "WordEntry.h"

class HashTable {
  protected:
    int size;
    list<WordEntry*> *hashTable;
    virtual int computeHash(const WordEntry*) const = 0;
  public:
    HashTable(int);
    void put(WordEntry*);
};

#endif 
