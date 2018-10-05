#ifndef NETWORK_H
#define NETWORK_H
#include "Node.h"
#include "library/Hashtable.h"

typedef struct Network_T {
    double learningRate;
    Node *inputNodes, *hiddenNodes, *outputNodes;
    int inputSize, hiddenSize, outputSize;
    HashTable *weightMap;
    Node bias;
} Network;

#endif