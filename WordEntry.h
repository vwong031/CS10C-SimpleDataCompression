#ifndef WORDENTRY_H
#define WORDENTRY_H

#include <iostream>
#include <string> 
using namespace std; 

class WordEntry {
  private:
    string text;
    int frequency;
    int code;
  public:
    // WordEntry(); // not used so far
    WordEntry(const string&); 

    /* Mutators */
    void incrementFrequency(); 
    void setCode(int code);

    /* Accessors */
    string getText() const; 
    int getFrequency() const; 
    int getCode() const;

    /* Comparators */
    bool operator==(const WordEntry&) const;
};

#endif
