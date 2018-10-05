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
    network->bias.name = "BIAS";
    network->bias.data = 1.0;
    network->inputSize = inputs;
    network->hiddenSize = hidden;
    network->outputSize = outputs;
    int size = ((inputs * hidden) + hidden + outputs + (hidden * outputs)) * 2;
    initializeHashTable(network->weightMap, size);
    //Initializing input nodes
    for(int i = 0; i < inputs; i++) {
        Node node;
        char *name = (char*)malloc(sizeof(char) * 10);
        sprintf(name, "INPUT_%d", i+1);
        node.name = name;
        network->inputNodes[i] = node;
    }
    //Initializing hidden nodes and weights
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
        //Initializing weights from input -> hidden
        for(int y = 0; y < inputs; y++) {
            char *weightName = (char*)malloc(sizeof(char) * 22);
            sprintf(weightName, "%s->%s", network->inputNodes[y].name, name);
            insert(network->weightMap, weightName, getRandomNumber());
        }
    }
    //Initializing output nodes and weights
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
        //Initializing weights from hidden -> output
        for(int y = 0; y < hidden; y++) {
            char *weightName = (char*)malloc(sizeof(char) * 22);
            sprintf(weightName, "%s->%s", network->hiddenNodes[y].name, name);
            insert(network->weightMap, weightName, getRandomNumber());
        }
    }
}
void train(Network *network, float **trainingInputs, float **trainingOutputs, float targetError) {
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 2; j++) {
            printf("%.1f   ", trainingInputs[i][j]);
        }
        printf("\n");
    }
    float error = 999.9;
    int numInputs = sizeof(trainingInputs) / network->inputSize;
    int numOutputs = sizeof(trainingOutputs) / network->outputSize;
    float *inputs;
    float *outputs;
    while(error >= targetError) {
        int i = rand() % numInputs;
        inputs = trainingInputs[i];
        outputs = trainingOutputs[i];
        
        return;
    }
}
void cleanup(Network *network) {
    //Input Nodes
    for(int i = 0; i < network->inputSize; i++)
        free(network->inputNodes[i].name);  
    free(network->inputNodes);

    //HiddenNodes
    for(int i = 0; i < network->hiddenSize; i++)
        free(network->hiddenNodes[i].name); 
    free(network->hiddenNodes);

    //OutputNodes
    for(int i = 0; i < network->outputSize; i++)
        free(network->outputNodes[i].name); 
    free(network->outputNodes);

    //WeightMap
    deleteAll(network->weightMap);
    free(network->weightMap->elements);
    free(network->weightMap);
    //Network
    free(network);
}

int main() {
    srand(time(0));
    //Initializing network
    Network *network = (Network*)malloc(sizeof(Network));
    initializeNetwork(network, 2, 3, 1);
    //Training
    float targetError = 0.00001;
    float ** trainingInputs = (float**)malloc(sizeof(float*) * 4);
    float ** trainingOutputs = (float**)malloc(sizeof(float*) * 4);
    for(int i = 0; i < 4; i++) {
        trainingInputs[i] = (float*)malloc(sizeof(float) * 2);
        trainingOutputs[i] = (float*)malloc(sizeof(float) * 1);
    }
    trainingInputs[0][0] = 0.1;
    trainingInputs[0][1] = 0.1;
    trainingOutputs[0][0] = 0.1;

    trainingInputs[1][0] = 0.1;
    trainingInputs[1][1] = 0.9;
    trainingOutputs[1][0] = 0.9;

    trainingInputs[2][0] = 0.9;
    trainingInputs[2][1] = 0.1;
    trainingOutputs[2][0] = 0.9;

    trainingInputs[3][0] = 0.9;
    trainingInputs[3][1] = 0.9;
    trainingOutputs[3][0] = 0.1;
    train(network, (float**)trainingInputs, (float**)trainingOutputs, targetError);
    //Cleanup
    cleanup(network);
    for(int i = 0; i < 4; i++) {
        free(trainingInputs[i]);
        free(trainingOutputs[i]);
    }
    free(trainingInputs);
    free(trainingOutputs);
}