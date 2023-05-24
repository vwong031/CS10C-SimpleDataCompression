#ifndef DECODERHASHTABLE_H
#define DECODERHASHTABLE_H

#include "HashTable.h"

class DecoderHashTable : public HashTable {
  private:
    int computeHashFromCode(const int&) const;
  protected:
    int computeHash(const WordEntry* word) const override;
  public:
    /* Inherit the base class constructor (https://stackoverflow.com/a/8093904) */
    DecoderHashTable(const int& numBuckets) : HashTable(numBuckets) {}
    string decode(const int) const;
};

/* Modulo hash
   https://learn.zybooks.com/zybook/UCRCS010CSpring2021/chapter/8/section/5
   
   This is a perfect hash function (zero collisions!),
   because every code we're hashing is unique
   and our table size is exactly equal to the number of codes
   */
int DecoderHashTable::computeHashFromCode(const int& code) const {
  return code % size; 
}

int DecoderHashTable::computeHash(const WordEntry* word) const {
  int code = word->getCode();
  return computeHashFromCode(code);
}

string DecoderHashTable::decode(const int code) const {
  int hash = computeHashFromCode(code);
  list<WordEntry*> &bucket = hashTable[hash];

  for (WordEntry *entry : bucket) {
    if (entry->getCode() == code) {
      return entry->getText();
    }
  }

  // Word not in hash table
  // Return default value
  return "";
}

#endif
