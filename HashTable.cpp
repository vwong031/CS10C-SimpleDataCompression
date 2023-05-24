#include "HashTable.h"

/* HashTable constructor
*  initialize array of lists of WordEntry*
*  input numBucketsInHashTable is the size of the array
*/
HashTable::HashTable (int numBucketsInHashTable) {
  size = numBucketsInHashTable;
  hashTable = new list<WordEntry*>[size];
}

/* Store hash -> token in Decoder Hash Table
      - In the DecoderHashTable, the hash will be from the WordEntry's code
      - In the EncoderHashTable, the hash will be from the WordEntry's text/token
      - But the HashTable base class doesn't have to know that
   If the word already exists, don't add it
      - This shouldn't happen since our vector counts duplicates within the WordEntry,
        but we don't want to add duplicates here because it will increase memory
        and never be used since we return as soon as we find a word
   */
void HashTable::put(WordEntry* word) {
  int hash = computeHash(word);
  list<WordEntry*> &bucket = hashTable[hash];

  // Add check in case there are duplicates
  for (WordEntry *entry : bucket) {
    if (*entry == *word) {
      return;
    }
  }
  
  bucket.push_back(word);
}

// All other functions are implemented by derived classes
