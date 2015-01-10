#include <iostream>
#include <math.h>
#include <vector>
#include "dictionary.h"

using namespace std;

#define MAX_LOOPS 100
#define CAPACITY 17
#define MAX_LOAD_FACTOR 0.40


template <class K, class V>
Dictionary<K,V>::Dictionary()
{
  capacity = CAPACITY;
  table1 = new pair<K,V>*[capacity];
  table2 = new pair<K,V>*[capacity];
  table3 = new pair<K,V>*[capacity];

  size1 = 0;
  size2 = 0;
  size3 = 0;
  generateNewRands();

  for(int i = 0; i < capacity; i++) {
    table1[i] = NULL;
    table2[i] = NULL;
    table3[i] = NULL;
  }

}

template <class K, class V>
Dictionary<K,V>::~Dictionary() {
  	clear();
}

template <class K, class V>
Dictionary<K,V>::Dictionary(const Dictionary<K,V> & other) {
	copy(other);
}

template <class K, class V> 
Dictionary<K,V> & Dictionary<K,V>::operator=(const Dictionary<K,V> & other) {

	if(this != &other) {
		clear();
		copy(other);
	}

	return *this;

}

template <class K, class V>
void Dictionary<K,V>::insert(const K & key, const V & value)
{
  if(keyExists(key)) return; // If the key already exists then return.

  pair<K,V>* kickedPair = new pair<K,V>(key, value);

  kickedPair = insertInner(kickedPair, true);
  int count = 0;
  while (kickedPair != NULL) {
    // Only resize when the number of loops passes the threshold.
    // Otherwise, see if rehashing fixes things.
    if (count++ % MAX_LOOPS == 0) {
      resize();
    } else {
      rehash();
    }
    kickedPair = insertInner(kickedPair, true); // try to insert the pair that was kicked
  }
}

template <class K, class V>
pair<K,V>* Dictionary<K,V>::insertInner(pair<K,V>* kickedPair, bool growSizeIfNeeded)
{
  if (kickedPair == NULL) return NULL;

  if(growSizeIfNeeded && shouldResize()) {
    resize();
  }
  int count = 0;
  size_t putIndex = 0;

  // Try inserting into the first array. If that does not work,
  // then kick out the current element in the array and try to insert
  // that into the second array while inserting your initial element
  // in the first array. Rinse and repeat until there is a successful insertion.

  while(++count < MAX_LOOPS) {
    putIndex = hash1(kickedPair -> first);
    if (table1[putIndex] == NULL) {
    		table1[putIndex] = kickedPair;
    		size1++;
    		return NULL;
    } else {
      pair<K,V>* tempPair = table1[putIndex];
      table1[putIndex] = kickedPair;
      kickedPair = tempPair;
    }

    putIndex = hash2(kickedPair -> first);
    if (table2[putIndex] == NULL) {
    		table2[putIndex] = kickedPair;
    		size2++;
    		return NULL;
    } else {
      pair<K,V>* tempPair = table2[putIndex];
      table2[putIndex] = kickedPair;
      kickedPair = tempPair;
    }

    putIndex = hash3(kickedPair -> first);
    if (table3[putIndex] == NULL) {
    		table3[putIndex] = kickedPair;
    		size3++;
    		return NULL;
    } else {
      pair<K,V>* tempPair = table3[putIndex];
      table3[putIndex] = kickedPair;
      kickedPair = tempPair;
    }
  }
  // This will be null if we inserted successfully. Otherwise, it will be
  // the pair that was last kicked out.
  return kickedPair;
}

template <class K, class V>
void Dictionary<K,V>::resize() {

  // Standard copy of the internal arrays. Copy the contents of the arrays into
  // temp arrays and resize the original arrays. Then, set all pointers in the new resized arrays
  // to point to the ones in the old arrays. At the end, we take care of rehashing.

  pair<K,V>** tempTable1 = new pair<K,V>*[capacity];
  pair<K,V>** tempTable2 = new pair<K,V>*[capacity];
  pair<K,V>** tempTable3 = new pair<K,V>*[capacity];

  for(int i = 0; i < capacity; i++) {
    tempTable1[i] = table1[i];
    tempTable2[i] = table2[i];
    tempTable3[i] = table3[i];
  }
  delete[] table1;
  delete[] table2;
  delete[] table3;

  int newCapacity = capacity * 1.5;
  table1 = new pair<K,V>*[newCapacity];
  table2 = new pair<K,V>*[newCapacity];
  table3 = new pair<K,V>*[newCapacity];
  for (int i = 0; i < newCapacity; i++) {
    table1[i] = NULL;
    table2[i] = NULL;
    table3[i] = NULL;
  }

  for(int i = 0; i < capacity; i++) {
    table1[i] = tempTable1[i];
    table2[i] = tempTable2[i];
    table3[i] = tempTable3[i];
  }

  // clear all memory in the temp tables
  delete[] tempTable1;
  delete[] tempTable2;
  delete[] tempTable3;

  capacity = newCapacity;
  rehash();
}

template <class K, class V>
void Dictionary<K,V>::rehash() {

  pair<K,V>** tempTable1 = new pair<K,V>*[capacity];
  pair<K,V>** tempTable2 = new pair<K,V>*[capacity];
  pair<K,V>** tempTable3 = new pair<K,V>*[capacity];

  for(int i = 0; i < capacity; i++) {
    tempTable1[i] = table1[i];
    tempTable2[i] = table2[i];
    tempTable3[i] = table3[i];
  }

  // If at any time during the rehashing, the insertion fails(a cycle), then break out,
  // generate new random numbers, and try again. 
  bool done = false;
  while (!done) {
    generateNewRands();

    // This ensures that we start fresh in the event that we have to start over. We can do this
    // because the temp tables have the pointers stored. As a result, we do not have to worry about
    // losing any data.
    for (int i = 0; i < capacity; i++) {
      table1[i] = NULL;
      table2[i] = NULL;
      table3[i] = NULL;
    }

    size1 = 0;
    size2 = 0;
    size3 = 0;
    done = true;
    for(int i = 0; i < capacity && done; i++) {
      if (insertInner(tempTable1[i], false) != NULL) {
        done = false;
        break;
      }
    }
    for(int i = 0; i < capacity && done; i++) {
      if (insertInner(tempTable2[i], false) != NULL) {
        done = false;
        break;
      }
    }
    for(int i = 0; i < capacity && done; i++) {
      if (insertInner(tempTable3[i], false) != NULL) {
        done = false;
        break;
      }
    }
  }
  // clear all memory in the temp tables
  delete[] tempTable1;
  delete[] tempTable2;
  delete[] tempTable3;

}

template <class K, class V>
void Dictionary<K,V>::generateNewRands() {

  // Generate new random hashing numbers. This makes sure that the new numbers
  // we generate are the same as the previous ones.
  size_t newRand1 = 0;
  size_t newRand2 = 0;
  size_t newRand3 = 0;
  do {
    newRand1 = rand() % 100;
    newRand2 = rand() % 100;
    newRand3 = rand() % 100;
  } while(newRand1 == newRand2 == newRand3 || newRand1 == rand1 || newRand2 == rand2 || newRand3 == rand3);

  rand1 = newRand1;
  rand2 = newRand2;
  rand3 = newRand3;

}

template <class K, class V>
void Dictionary<K,V>::remove(const K & key)
{

  // The key will exist in one of the three tables. If it is found,
  // then delete it. If it is not found, then do nothing.

  size_t firstIdx = hash1(key);
  if(table1[ firstIdx ] != NULL) {

    if(table1[ firstIdx ] -> first == key) {
    		delete table1[ firstIdx ];
    		table1[ firstIdx ] = NULL;
    		size1--;
    		return;
    }
  }

  size_t secondIdx = hash2(key);
  if(table2[ secondIdx ] != NULL) {

    if(table2[ secondIdx ] -> first == key) {
    		delete table2[ secondIdx ];
    		table2[ secondIdx ] = NULL;
    		size2--;
    		return;
    }
  }

  size_t thirdIdx = hash3(key);
  if(table3[ thirdIdx ] != NULL) {

    if(table3[ thirdIdx ] -> first == key) {
    		delete table3[ thirdIdx ];
    		table3[ thirdIdx ] = NULL;
    		size3--;
    		return;
    }
  }
}

template <class K, class V>
V Dictionary<K,V>::find(const K & key)
{

  // The key will exist in one of the three tables. If it is not found, then return
  // the default value.

  size_t firstIdx = hash1(key);
  if(table1[ firstIdx ] != NULL) {

    if(table1[ firstIdx ] -> first == key) {
    		return table1[ firstIdx ] -> second;
    }
  }

  size_t secondIdx = hash2(key);
  if(table2[ secondIdx ] != NULL) {

    if(table2[ secondIdx ] -> first == key) {
    		return table2[ secondIdx ] -> second;
    }
  }

  size_t thirdIdx = hash3(key);
  if(table3[ thirdIdx ] != NULL) {

    if(table3[ thirdIdx ] -> first == key) {
    		return table3[ thirdIdx ] -> second;
    }
  }

  return V();
}

template <class K, class V>
bool Dictionary<K,V>::keyExists(const K & key)
{

  // The key will exist in one of the three tables. 

  size_t firstIdx = hash1(key);
  if(table1[ firstIdx ] != NULL) {
    if(table1[ firstIdx ] -> first == key) {
    		return true;
    }
  }

  size_t secondIdx = hash2(key);
  if(table2[ secondIdx ] != NULL) {
    if(table2[ secondIdx ] -> first == key) {
    		return true;
    }
  }

  size_t thirdIdx = hash3(key);
  if(table3[ thirdIdx ] != NULL) {
    if(table3[ thirdIdx ] -> first == key) {
    		return true;
    }
  }

  return false;

}

template <class K, class V>
bool Dictionary<K,V>::isEmpty() const
{
  if(size1 == 0 && size2 == 0 && size3 == 0) return true;
  else return false;
}

template <class K, class V>
size_t Dictionary<K,V>::hash1(const K & key)
{
  return (hash(key) << rand1) % capacity;
}

template <class K, class V>
size_t Dictionary<K,V>::hash2(const K & key)
{
  return (hash(key) << rand2) % capacity;
}

template <class K, class V>
size_t Dictionary<K,V>::hash3(const K & key)
{
  return (hash(key) << rand3) % capacity;
}

template <class K, class V>
bool Dictionary<K,V>::shouldResize() {

  // This method ensures that our array does not get too full, which speeds up
  // inserts. However, this contributes to alot of memory wastage.
  return ((size1 + size2 + size3) >= (capacity * 3 * MAX_LOAD_FACTOR));

}

template <class K, class V>
void Dictionary<K,V>::printTables() {
  cout << "Printing table1 ..." << '\n';
  for(int i = 0; i < capacity; i++) {
    if(table1[i] != NULL) {
      cout << i << ": " << table1[i] -> first << '\n';
    }
    else {
      cout << i << ": " << "NULL" << '\n';
    }
  }

  cout << "Printing table2..." << '\n';

  for(int  i = 0; i < capacity; i++) {
    if(table2[i] != NULL){
      cout << i << ": " << table2[i] -> first << '\n';
    }
    else {
      cout << i << ": " << "NULL" << '\n';
    }
  }

  cout << "Printing table3..." << '\n';

  for(int i = 0; i < capacity; i++) {
    if(table3[i] != NULL){
      cout << i << ": " << table3[i] -> first << '\n';
    }
    else {
      cout << i << ": " << "NULL" << '\n';
    }
  }

}

template <class K, class V>
void Dictionary<K,V>::clear() {

  // Clears all dynamic memory associated with the class
  
  for(int i = 0; i < capacity; i++) {
    if (table1[i] != NULL) delete table1[i];
    if (table2[i] != NULL) delete table2[i];
    if (table3[i] != NULL) delete table3[i];
  }
  
  delete[] table1;
  delete[] table2;
  delete[] table3;

  size1 = 0;
  size2 = 0;
  size3 = 0;
  
}

template <class K, class V>
void Dictionary<K,V>::copy(const Dictionary<K,V> & other) {

  // Deep copy 

	this -> capacity = other.capacity;
	this -> size1 = other.size1;
	this -> size2 = other.size2;
	this -> size3 = other.size3;
	this -> rand1 = other.rand1;
	this -> rand2 = other.rand2;
	this -> rand3 = other.rand3;
	this -> hash = other.hash;

	table1 = new pair<K,V>*[capacity];
  	table2 = new pair<K,V>*[capacity];
  	table3 = new pair<K,V>*[capacity];

	for(int i = 0; i < capacity; i++) {
		if(other.table1[i] != NULL) {
			table1[i] = new pair<K,V>(other.table1[i] -> first, other.table1[i] -> second);
		}
		else {
			table1[i] = NULL;
		}
		if(other.table2[i] != NULL) {
			table2[i] = new pair<K,V>(other.table2[i] -> first, other.table2[i] -> second);
		}
		else {
			table2[i] = NULL;
		}
		if(other.table3[i] != NULL) {
			table3[i] = new pair<K,V>(other.table3[i] -> first, other.table3[i] -> second);
		}
		else {
			table3[i] = NULL;
		}

	}

}
