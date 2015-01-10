/**
 * @file dictionary.h
 */

#ifndef _DICTIONARY_H_
#define _DICTIONARY_H_



/**
 * Implements the dictionary ADT.
 *
 * The implementation is up to you, but you must complete all the given
 *  public functions. You will need to add some member variables and private
 *  helper functions.
 */
template <class K, class V>
class Dictionary
{
public:
  /**
   * Constructor: creates an empty dictionary.
   */
  Dictionary();

  // Destructor for the class.
  ~Dictionary();

  // Copy constructor for the class.
  Dictionary(const Dictionary & other);

  // Assignment operator for the class.
  Dictionary& operator=(const Dictionary & other);

  /**
   * Inserts the given key, value pair into the dictionary.
   * If the key already exists, do nothing.
   *
   * @param key The key to be inserted.
   * @param value The value to be inserted.
   */
  void insert(const K & key, const V & value);

  /**
   * Removes the given key (and its associated data) from the
   *  dictionary.
   *
   * @param key The key to be removed.
   */
  void remove(const K & key);

  /**
   * Finds the value associated with a given key.
   *
   * @param key The key whose data we want to find.
   * @return The value associated with this key, or the default value
   *  (V()) if it was not found.
   */
  V find(const K & key);

  /**
   * Determines if the given key exists in the dictionary.
   *
   * @param key The key we want to find.
   * @return A boolean value indicating whether the key was found.
   */
  bool keyExists(const K & key);

  /**
   * Determines if the dictionary is empty. Should be O(1).
   *
   * @return A boolean value indicating whether the dictionary is
   *  empty.
   */
  bool isEmpty() const;

  // Function helpful for debugging.
  void printTables();

private:

  // Container arrays for the hash table - An array of pointer to pair pointers.
  std::pair<K,V>** table1;
  std::pair<K,V>** table2;
  std::pair<K,V>** table3;

  // Keeps track of the sizes for the internal arrays.
  int size1;
  int size2;
  int size3;

  // Value that holds the capacity of all the internal arrays.
  int capacity;

  // Values that help randomize hashing.
  size_t rand1;
  size_t rand2;
  size_t rand3;

  // Hash functor that is used in the private hash methods.
  std::hash<K> hash;

  // Determines if the internal arrays should be resized.
  bool shouldResize();

  // Resize the internal arrays.
  void resize();

  // Generates new randomized hashing values.
  void generateNewRands();

  // Takes care of rehashing.
  void rehash();

  // Three hashing functions that correspond to the three internal arrays.
  size_t hash1(const K & key);
  size_t hash2(const K & key);
  size_t hash3(const K & key);

  // Helper function for the public insert method.
  std::pair<K,V>* insertInner(std::pair<K,V>*kickedPair, bool growSizeIfNeeded);

  // Clears all dynamic memory associated with the class
  void clear();

  // Helper function that is called in the copy constructor and assignment operator
  void copy(const Dictionary & other);

};


#include "dictionary.cpp"
#endif
