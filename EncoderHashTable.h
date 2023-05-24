#ifndef ENCODERHASHTABLE_H
#define ENCODERHASHTABLE_H

#include "HashTable.h"

class EncoderHashTable : public HashTable {
  private:
    int computeHashFromToken(const string&) const; 
  protected:
    int computeHash(const WordEntry*) const override;
  public:
    /* Inherit the base class constructor (https://stackoverflow.com/a/8093904) */
    EncoderHashTable(const int& numBuckets) : HashTable(numBuckets) {}
    int encode(const string&) const; 
};

/* David J. Bernstein's multiplicative string hash
   https://learn.zybooks.com/zybook/UCRCS010CSpring2021/chapter/8/section/5?content_resource_id=45126725
   "Bernstein's hash function performs well for hashing short English strings."

   The function causes at most 7 collisions per bucket
   for our Melville and Alice in Wonderland texts.
*/
int EncoderHashTable::computeHashFromToken(const string& token) const {
  unsigned initialVal;
  unsigned hashMultiplier;
  unsigned hash;

  hash = initialVal = 5381;
  hashMultiplier = 33;

  for (char character : token) {
    hash *= hashMultiplier;
    // add ASCII value of character, ignoring case
    hash += tolower(character);
  }

  return hash % size;
}

int EncoderHashTable::computeHash(const WordEntry* word) const {
  string token = word->getText();
  return computeHashFromToken(token);
}

int EncoderHashTable::encode(const string& token) const {
  int hash = computeHashFromToken(token); 
  list<WordEntry*> &bucket = hashTable[hash]; 

  for (WordEntry *entry : bucket) {
    if (entry->getText() == token) {
      return entry->getCode();
    }
  }

  return -1;
}

#endif
