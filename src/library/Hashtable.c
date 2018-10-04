#include "./Hashtable.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NULL_ELEMENT "NULL_ELEMENT"

void initializeHashTable(HashTable *table, int size) {
    table->size = size;
    table->count = 0;
    table->elements = (DataItem**)malloc(sizeof(DataItem*) * size);
}
int hash(HashTable *table, char* key) {
    int hash = 5381;
    int c;
    while(c = *key++)
        hash = ((hash << 5) + hash) + c;
    return hash % table->size;
}
DataItem *search(HashTable *table, char *key) {
    int hashIndex = hash(table, key);
    while(table->elements[hashIndex] != NULL) {
        if(strcmp(table->elements[hashIndex]->key, key) == 0)
            return table->elements[hashIndex];
        hashIndex++;
        hashIndex %= table->size;
    }
    return NULL;
}
void resize(HashTable *table, int newSize) {
    int count = table->count;
    DataItem *temp = (DataItem*)malloc(sizeof(DataItem) * table->count);
    int j = 0;
    for(int i = 0; i < table->size && j < count; i++) {
        if(table->elements[i] != NULL) {
            temp[j++] = *table->elements[i];
            delete(table, table->elements[i]->key);
        }
    }
    print(table);
    table->elements = (DataItem**)realloc(table->elements, sizeof(DataItem*) * newSize);
    table->size = newSize;
    for(int i = 0; i < count; i++) {
        insert(table, temp[i].key, temp[i].data);
    }
    free(temp);
}
int isPrime(int n) {
	if(n % 2 == 0) 
		return n == 2;
	if(n % 3 == 0)
		return n == 3;
	for(int i = 2; i < n; i++) {
		if(n % i == 0)
			return 0;
	}
	return 1;
}
void insert(HashTable *table, char *key, int data) {
    int hashIndex = hash(table, key);

    DataItem *item = (DataItem*)malloc(sizeof(DataItem));
    item->data = data;
    item->key = key;
    while(table->elements[hashIndex] != NULL && table->elements[hashIndex]->key != key && strcmp(table->elements[hashIndex]->key, NULL_ELEMENT) != 0) {
        hashIndex++;
        hashIndex %= table->size;
    }    
    if(table->elements[hashIndex] != NULL)
        free(table->elements[hashIndex]);
    else
        table->count++;
    table->elements[hashIndex] = item;
    
    //Resizing the table
    if((double)table->count / table->size > .75) {
        int newSize = table->size * 2;
        while(isPrime(newSize) == 0) { //TODO: Find a faster way to find the next prime
            newSize++;
        }
        resize(table, newSize);
    }
}
void delete(HashTable *table, char *key) {
    int hashIndex = hash(table, key);
    while(table->elements[hashIndex] != NULL) {
        if(strcmp(table->elements[hashIndex]->key, key) == 0) {
            table->elements[hashIndex]->key = NULL_ELEMENT;
            table->elements[hashIndex]->data = -1;
            table->count--;
            return;
        }
        hashIndex++;
        hashIndex %= table->size;
    }
}
void print(HashTable *table) {
    for(int i = 0; i < table->size; i++) {
        if(table->elements[i] != NULL) 
            printf(" (%s,%.3f)", table->elements[i]->key, table->elements[i]->data);
        else
            printf(" ~~ ");
        printf("\n");
    }
}

int main() {
    HashTable *table = (HashTable*)malloc(sizeof(HashTable));
    //printf("(key, value)\n");
    initializeHashTable(table, 3);
    
    char *key = "help";
    insert(table, key, 5);

    key = "key";
    insert(table, key, 5);

    key = "keys";
    insert(table, key, 5);
    printf("uhhh\n");
    print(table);

    free(table);
}