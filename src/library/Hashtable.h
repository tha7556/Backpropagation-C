#ifndef HASHTABLE_H
#define HASHTABLE_H

/**
 * An element inside of the HashTable
 * data: The data associated with the key in the HashTable
 * key: The key to locate the element in the HashTable 
 **/
typedef struct DataItem_T {
    float data;
    char *key;
} DataItem;

/**
 * A HashTable for associating string keys with double data.
 * Should only be accessed through methods provided, otherwise there could be unexpected behavior
 * size: The total size of the HashTable
 * count: The number of elements stored in the HashTable
 * elements: An array of pointers to DataItems. Holds the contents of the HashTable
**/
typedef struct HashTable_T {
    int size;
    int count;
    DataItem **elements; //Array of DataItem pointers
} HashTable;
/**
 * Initializes a HashTable at the pointer provided, with the starting size provided
 * table: A pointer to the HashTable to be initialized
 * size: The desired size of the HashTable
**/
void initializeHashTable(HashTable *table, int size);
/**
 * Searches the HashTable for the provided key, returns NULL if not found
 * table: A pointer to the HashTable
 * key: The string key to search for
**/
DataItem *search(HashTable *table, char *key);
/**
 * Inserts a Key,Value pair into the HashTable. Overwrites the value in the database if the key is already in there.
 * table: A pointer to the HashTable
 * key: The string key to insert
 * data: The value that corresponds to the key
**/
void insert(HashTable *table, char *key, float data);
/**
 * Deletes the Key, Value pair from the HashTable
 * table: A pointer to the HashTable
 * key: The string key to delete 
**/
void delete(HashTable *table, char *key);
/**
 * Deletes all of the values in the HashTable
 * table: A pointer to the HashTable
**/
void deleteAll(HashTable *table);
/**
 * Prints the HashTable to the console
 * table: A pointer to the HashTable
**/
void print(HashTable *table);

#endif