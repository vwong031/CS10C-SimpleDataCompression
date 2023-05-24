// Collaborated with: Valerie Wong, Kira Hammond
// zyBooks
// cplusplus.com (substr, string, find_first_of)

#include "EncoderHashTable.h" 
#include "DecoderHashTable.h" 
#include "WordEntry.h" 
#include "sort.cpp" 
#include <vector> 
#include <iostream> 
#include <fstream> 
#include <sstream> // for stringstream
#include <cstdlib> 
#include <iomanip> // for setw in cout
using namespace std;

/* Compile command:
   g++ main.cpp HashTable.cpp WordEntry.cpp -Wall -Werror -o a.out
*/

string getFilesForEncoding(const string&, ifstream&, ofstream&, ofstream&);
void getFilesForDecoding(const string&, ifstream&, ofstream&);
string splitAtFirstPunctuation(string&);
void readWords(vector<WordEntry*>&, ifstream&);
void sort(vector<WordEntry*>&);
int Partition(vector<WordEntry*>&, int, int, int);
int medianPartition(vector<WordEntry*>&, int, int);
void Quicksort_medianOfThree(vector<WordEntry*>&, int, int);
void generateCodes(vector<WordEntry*>&);
void outputMapping(const vector<WordEntry*>&, ofstream&);
void createHashTables(const vector<WordEntry*>&, EncoderHashTable*&, DecoderHashTable*&);
void createEncoderHashTable(const vector<WordEntry*>&, HashTable&);
void createDecoderHashTable(const vector<WordEntry*>&, HashTable&);
void encodeData(ifstream&, const EncoderHashTable*, ofstream&);
void decodeData(ifstream&, const DecoderHashTable*, ofstream&);

int main() {
  string originalDataFilename;
  string filenameWithoutExtension;

  // Files for encoding:
  ifstream originalDataFile;
  ofstream encodedDataOutputFile;
  ofstream mappingOutputFile;
  
  // Files for decoding:
  ifstream encodedDataInputFile;
  ofstream decodedDataOutputFile;
  
  vector<WordEntry*> words;

  // Note: The hash tables must be pointers or references for polymorphism
  // to work in some situations (See zyBooks 1.17)
  EncoderHashTable* encoder;
  DecoderHashTable* decoder;
  
  cout << "Enter input file name: ";
  cin >> originalDataFilename;
  filenameWithoutExtension = getFilesForEncoding(originalDataFilename, originalDataFile, encodedDataOutputFile, mappingOutputFile);

  readWords(words, originalDataFile);
  sort(words); 
  generateCodes(words);
  outputMapping(words, mappingOutputFile);
  createHashTables(words, encoder, decoder); 
  encodeData(originalDataFile, encoder, encodedDataOutputFile);

  originalDataFile.close(); 
  encodedDataOutputFile.close();
  mappingOutputFile.close();

  getFilesForDecoding(filenameWithoutExtension, encodedDataInputFile, decodedDataOutputFile);
  decodeData(encodedDataInputFile, decoder, decodedDataOutputFile);
  
  encodedDataInputFile.close();
  decodedDataOutputFile.close();

  cout << "Success!" << endl;
  
  return 0; 
}

/* Get input filename from user.
   Determine filenames for output files.
   Set reference parameters to successfully opened files */
string getFilesForEncoding(const string& inputFilename, ifstream& inFS,
                         ofstream& encodedDataOutputFile, ofstream& mappingOutputFile) {
  string filenameWithoutExtension = inputFilename.substr(0, inputFilename.find_first_of('.'));
  string encodedDataFilename = filenameWithoutExtension + "-encoded.txt";
  string mappingFilename = filenameWithoutExtension + "-mapping.txt";

  inFS.open(inputFilename);

  if (!inFS.is_open()) {
    cerr << "Error opening " << inputFilename << endl; 
    exit(1); 
  } 
  
  encodedDataOutputFile.open(encodedDataFilename);
  if (!encodedDataOutputFile.is_open()) {
    cerr << "Error opening " << encodedDataFilename << endl; 
    exit(1); 
  } 

  mappingOutputFile.open(mappingFilename); 
  if (!mappingOutputFile.is_open()) {
    cerr << "Error opening " << mappingFilename << endl; 
    exit(1); 
  }

  cout << "I'll read from " << inputFilename
       << ", output the encoded data to " << encodedDataFilename
       << ", and output the table of words, frequencies, and codes to " << mappingFilename << endl;

  return filenameWithoutExtension;
}

/* Note: We have to close the encoded data ofstream
   before opening it as an ifstream to read and decode.
   Otherwise the program for some reason thinks it's reached the EOF
   long before the actual EOF and only decodes part/none of the data */
void getFilesForDecoding(const string& filenameWithoutExtension, ifstream& encodedDataInputFile, ofstream& decodedDataOutputFile) {
  string encodedDataFilename = filenameWithoutExtension + "-encoded.txt";
  string decodedDataFilename = filenameWithoutExtension + "-decoded.txt";

  encodedDataInputFile.open(encodedDataFilename);
  if (!encodedDataInputFile.is_open()) {
    cerr << "Error opening " << encodedDataFilename << endl;
    exit(1);
  }

  decodedDataOutputFile.open(decodedDataFilename);
  if (!decodedDataOutputFile.is_open()) {
    cerr << "Error opening " << decodedDataFilename << endl;
    exit(1);
  }

  cout << "After encoding the data, I'll decode it again and output to " << decodedDataFilename << endl;
}

/* If search_string starts with punctuation mark,
   return punctuation mark.
   Else, return the substring before the first punctuation mark
   Set search_string reference param to remainder of search_string */
string splitAtFirstPunctuation(string& search_string) {
  string substr; 
  string punctuation = "!\"#$%&'()*+,-./:;<=>?@[]\\^_`{}|~";

  if (search_string.find_first_of(punctuation) == 0) {
    substr = search_string.at(0);
    search_string = search_string.substr(1, search_string.size() - 1);
    return substr; 
  } 

  int i = search_string.find_first_of(punctuation);

  if (i < 0) {
    substr = search_string;
    search_string = "";
    return substr;
  }

  substr = search_string.substr(0, i);
  search_string = search_string.substr(i, search_string.size() - i);

  return substr; 
}

/* Read input file and fill words vector */
void readWords(vector<WordEntry*>& words, ifstream& inFS) {
  // Note: Parker suggested strtok, find_first_of, and substr
  string text; 
  string substr; 
  WordEntry* data;
  bool wasIncremented; 

  while (inFS >> text) {
    while (text.size() != 0) {
      substr = splitAtFirstPunctuation(text);
      wasIncremented = false; 

      for (WordEntry* word : words) {
        if (substr == word->getText()) { 
          word->incrementFrequency(); 
          wasIncremented = true;
          break; // no need to traverse entire vector if match found early
        }
      }

      if (!wasIncremented) {
        data = new WordEntry(substr);
        words.push_back(data);
      }
    }
  }
}

void sort(vector<WordEntry*>& words) {
  // Sort words in order of descending frequency
  int lowIndex = 0; 
  int highIndex = words.size() - 1; 

  Quicksort_medianOfThree(words, lowIndex, highIndex); 
}

/* Generate codes with a simple counter, starting at index 1.
   Assuming words are already sorted by descending frequency,
   code 1 will indicate the most common word/token/text/punctuation,
   code 2 the next most common,
   all the way to the highest code for the least common word */
void generateCodes(vector<WordEntry*>& words) {
  WordEntry* word;

  for (long unsigned int i = 0; i < words.size(); ++i) {
    word = words.at(i);
    word->setCode(i + 1);
  }
}

void outputMapping(const vector<WordEntry*>& words, ofstream& mappingOutputFile) {
  stringstream ss;
  string output;

  ss << setw(20) << "Token"
     << setw(15) << "Frequency"
     << setw(10) << "Code" << endl;
  
  getline(ss, output);
  mappingOutputFile << output << endl;
  cout << output << endl;

  for (WordEntry* word : words) {
    ss << setw(20) << word->getText()
       << setw(15) << word->getFrequency()
       << setw(10) << word->getCode() << endl;
    
    getline(ss, output);
    mappingOutputFile << output << endl;
    cout << output << endl;
  }
}

void createHashTables(const vector<WordEntry*>& words, EncoderHashTable*& encoder,
                            DecoderHashTable*& decoder) {
  int numberOfWords = words.size();
  decoder = new DecoderHashTable(numberOfWords);
  encoder = new EncoderHashTable(numberOfWords);
  
  for (WordEntry* word : words) {
    decoder->put(word);
    encoder->put(word);
  }
}

/* Encode the data from the input file
     by replacing all tokens with their codes
   Output the encoded, compressed data to the specified output file
   Preserve whitespace and capitalization so we can easily decode the data
   
   Note: We can't open or read from a const ifstream
*/
void encodeData(ifstream& inFS, const EncoderHashTable* encoder, ofstream& outFS) {
  string currentWhitespace = "";
  char c;
  string text, token;
  int code;
  
  // Reset input file stream to read again
  inFS.clear();
  inFS.seekg(0);
  
  c = inFS.peek(); // View next character, but don't remove from ifstream
  while (c) {
    if (inFS.eof()) {
      break;
    }
    else if (isspace(c)) {
      currentWhitespace += c;
      inFS.ignore(1, EOF); // Remove exactly 1 character from ifstream
    }
    else {
      outFS << currentWhitespace;
      currentWhitespace = "";
      inFS >> text;

      while (true) {
        token = splitAtFirstPunctuation(text);
        if (text.size() == 0) {
          code = encoder->encode(token);
          // Add to output
          // (no comma since this is the last token before whitespace/EOF)
          outFS << code;
          break;
        }
        code = encoder->encode(token);

        // Add to output. Use a comma to differentiate "one,two" from "twelve"
        outFS << code << ",";
      }
    }

    // View next character, but don't remove from ifstream
    c = inFS.peek();
  }
     
  // Output any leftover whitespace at the end of the file
  outFS << currentWhitespace;
}

/* Decode the data from the input file with the encoded data
     by replacing all codes with their tokens
   Output the decoded, expanded data to the specified output file
   Preserve whitespace and capitalization to match the original data */
void decodeData(ifstream& inFS, const DecoderHashTable* decoder, ofstream& outFS) {
  inFS.clear();
  inFS.seekg(0);

  string currentWhitespace = "";
  char c;
  string token;
  int code;
  
  c = inFS.peek(); // View next character, but don't remove from ifstream
  while (c) {
    if (inFS.eof()) {
      break;
    }
    else if (isspace(c)) {
      currentWhitespace += c;
      inFS.ignore(1, EOF); // Remove exactly 1 character from ifstream
    }
    else if (c == ',') {
      // Ignore our delimiter
      // that separates codes when they're not separated by whitespaces
      // (e.g. "the," and "that's")
      inFS.ignore(1, EOF);
    }
    else {
      outFS << currentWhitespace;
      currentWhitespace = "";
      
      inFS >> code;

      token = decoder->decode(code);
      outFS << token;
    }

    // View next character, but don't remove from ifstream
    c = inFS.peek();
  }
     
  // Output any leftover whitespace at the end of the file
  outFS << currentWhitespace;
}
