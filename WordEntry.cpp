#include "WordEntry.h"

WordEntry::WordEntry(const string& text) {
  this->text = text;
  frequency = 1;
}

void WordEntry::incrementFrequency() {
  ++frequency; 
}

/* Mutators */
void WordEntry::setCode(int code) {
  this->code = code;
}

/* Accessors */
string WordEntry::getText() const {
  return text; 
}

int WordEntry::getFrequency() const {
  return frequency; 
}

int WordEntry::getCode() const {
  return code;
}

/* Comparators */

/* See Week 3 slides for function header format:
   https://docs.google.com/presentation/d/1TxRHSF-hzygv9Bponx3Je4fdSj4YuPtR/edit#slide=id.p41
   */
bool WordEntry::operator==(const WordEntry& otherWord) const {
  /* TODO: Do you want to also compare the frequency and code?
     We just have to decide what qualifies as a "word"
     Is the word's frequency and ordering in our vector part of the definition of a "word"?
     Or just the actual word/token itself?
     */
  return this->text == otherWord.text;
}
