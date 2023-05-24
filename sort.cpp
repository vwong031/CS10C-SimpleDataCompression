#ifndef SORT
#define SORT

#include "WordEntry.h"
#include "HashTable.h"
#include <vector>
#include <utility>

int Partition(vector<WordEntry*>& words, int pivot, int i, int k) {
  bool isDone = false; 

  while (!isDone) {
    // Increment low index while numbers[i] < pivot
    // Find a number that doesn't belong in the high partition
    while (words.at(i)->getFrequency() > pivot) {
      ++i; 
    }

    // Decrement high index while pivot < numbers[k]
    // Find a number that doesn't belong in the low partition
    while (pivot > words.at(k)->getFrequency()) {
      --k; 
    }

    // If zero or one elements remain, then all numbers are partitioned.
    // Return high index
    if (i >= k) {
      isDone = true;
    }
    else {
      // Swap numbers[i] & numbers[k]
      swap(words.at(i), words.at(k));

      // Update low and high index
      ++i; 
      --k; 
    }
  }

  // Return highest index of the low partition
  //   (= the pivot position or next to it)
  return k; 
}

int medianPartition(vector<WordEntry*>& words, int left, int right) {
  int pivotFrequency;
  int center = left + (right - left) / 2;

  // Move the max of all three to the left
  if (words.at(center)->getFrequency() > words.at(left)->getFrequency()) {
    swap(words.at(center), words.at(left));
  }
  if (words.at(right)->getFrequency() > words.at(left)->getFrequency()) {
    swap(words.at(right), words.at(left));
  }

  // Move the min of all three to the right
  if (words.at(right)->getFrequency() > words.at(center)->getFrequency()) {
    swap(words.at(right), words.at(center));  
  }

  // Move the pivot out of the way while sorting
  swap(words.at(center), words.at(right - 1));

  // Get the frequency of the word at the pivot position
  pivotFrequency = words.at(right-1)->getFrequency();

  // Ensure high partition on left >= pivot
  // Ensure low partition on right <= pivot
  // Return highest index of the high partition
  //   (= the pivot's new position OR just left of it)
  return Partition(words, pivotFrequency, left, right - 1);
}

void Quicksort_medianOfThree(vector<WordEntry*>& words, int i, int k) {
  int pivotPosition; 

  if (i >= k) {
    return;
  }

  pivotPosition = medianPartition(words, i, k);
  Quicksort_medianOfThree(words, i, pivotPosition);
  Quicksort_medianOfThree(words, pivotPosition + 1, k);
}

#endif // SORT
