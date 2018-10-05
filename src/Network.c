#include "./Network.h"
#include <stdio.h>
#include <stdlib.h>
#include<time.h>

float getRandomNumber() {
    return -1 + 2 * ((float)rand())/RAND_MAX;
}
void initializeNetwork(Network *network, int inputs, int hidden, int outputs) {
    network->learningRate = 0.5;
    network->inputNodes = (Node*)malloc(sizeof(Node) * inputs);
    network->hiddenNodes = (Node*)malloc(sizeof(Node) * hidden);
    network->outputNodes = (Node*)malloc(sizeof(Node) * outputs);
    network->weightMap = (HashTable*)malloc(sizeof(HashTable));
    network->bias.name = "_BIAS";
    network->bias.data = 1.0;
    network->inputSize = inputs;
    network->hiddenSize = hidden;
    network->outputSize = outputs;
    int size = ((inputs * hidden) + hidden + outputs + (hidden * outputs)) * 2;
    initializeHashTable(network->weightMap, size);

    for(int i = 0; i < inputs; i++) {
        Node node;
        char *name = (char*)malloc(sizeof(char) * 10);
        sprintf(name, "INPUT_%d", i+1);
        node.name = name;
        network->inputNodes[i] = node;
    }
    for(int i = 0; i < hidden; i++) {
        Node node;
        char *name = (char*)malloc(sizeof(char) * 15);
        sprintf(name, "HIDDEN_%d", i+1);
        node.name = name;
        node.bias = &network->bias;
        char *biasName = (char*)malloc(sizeof(char) * 20);
        sprintf(biasName, "%s->%s", name, network->bias.name);
        insert(network->weightMap, biasName, getRandomNumber());
        network->hiddenNodes[i] = node;
        for(int y = 0; y < inputs; y++) {
            char *weightName = (char*)malloc(sizeof(char) * 22);
            sprintf(weightName, "%s->%s", network->inputNodes[y].name, name);
            insert(network->weightMap, weightName, getRandomNumber());
        }
    }
    for(int i = 0; i < outputs; i++) {
        Node node;
        char *name = (char*)malloc(sizeof(char) * 15);
        sprintf(name, "OUTPUT_%d", i+1);
        node.name = name;
        node.bias = &network->bias;
        char *biasName = (char*)malloc(sizeof(char) * 20);
        sprintf(biasName, "%s->%s", name, network->bias.name);
        insert(network->weightMap, biasName, getRandomNumber());
        network->outputNodes[i] = node;
        for(int y = 0; y < hidden; y++) {
            char *weightName = (char*)malloc(sizeof(char) * 22);
            sprintf(weightName, "%s->%s", network->hiddenNodes[y].name, name);
            insert(network->weightMap, weightName, getRandomNumber());
        }
    }
}

int main() {
    srand(time(0));
    Network *network = (Network*)malloc(sizeof(Network));
    initializeNetwork(network, 2, 3, 1);

}