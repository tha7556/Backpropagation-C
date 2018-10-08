#include "./Network.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

double getRandomNumber() {
    if(rand() % 2 == 0)
        return ((rand() % 9) + 1) / 10.0;
    else
        return ((rand() % 9) + 1) / -10.0;
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
    for(int h = 0; h < hidden; h++) {
        Node node;
        char *name = (char*)malloc(sizeof(char) * 15);
        sprintf(name, "HIDDEN_%d", h+1);
        node.name = name;
        node.bias = &network->bias;
        char *biasName = (char*)malloc(sizeof(char) * 20);
        sprintf(biasName, "%s->%s", name, network->bias.name);
        insert(network->weightMap, biasName, getRandomNumber());
        network->hiddenNodes[h] = node;
        //Initializing weights from input -> hidden
        for(int i = 0; i < inputs; i++) {
            char *weightName = (char*)malloc(sizeof(char) * 22);
            sprintf(weightName, "%s->%s", network->inputNodes[i].name, name);
            insert(network->weightMap, weightName, getRandomNumber());
        }
    }
    //Initializing output nodes and weights
    for(int o = 0; o < outputs; o++) {
        Node node;
        char *name = (char*)malloc(sizeof(char) * 15);
        sprintf(name, "OUTPUT_%d", o+1);
        node.name = name;
        node.bias = &network->bias;
        char *biasName = (char*)malloc(sizeof(char) * 20);
        sprintf(biasName, "%s->%s", name, network->bias.name);
        insert(network->weightMap, biasName, getRandomNumber());
        network->outputNodes[o] = node;
        //Initializing weights from hidden -> output
        for(int h = 0; h < hidden; h++) {
            char *weightName = (char*)malloc(sizeof(char) * 22);
            sprintf(weightName, "%s->%s", network->hiddenNodes[h].name, name);
            insert(network->weightMap, weightName, getRandomNumber());
        }
    }
}
double getWeight(Network *network, Node *a, Node *b) {
    char key[30];
    sprintf(key, "%s->%s", a->name, b->name);
    DataItem *result = search(network->weightMap, key);
    if(result != NULL)
        return result->data;
    return -2;
}
void setWeight(Network *network, Node *a, Node *b, double value) {
    char *key = (char*)malloc(sizeof(char) * 30);
    sprintf(key, "%s->%s", a->name, b->name);
    insert(network->weightMap, key, value);
    double val = search(network->weightMap, key)->data;

}
void enterInputs(Network *network, double *inputs) {
    for(int i = 0; i < network->inputSize; i++) 
        network->inputNodes[i].data = inputs[i];
}
double sigmoid(double x) {
    return 1.0 / (1.0 + exp(-x));
}
double sigmoidDerivative(double x) {
    return x * (1.0 - x);
}
void runNetwork(Network *network) {
    //Setting values of hidden nodes
    for(int h = 0; h < network->hiddenSize; h++) {
        double data = getWeight(network, &network->hiddenNodes[h], &network->bias) * network->bias.data;
        for(int i = 0; i < network->inputSize; i++) {
            data += getWeight(network, &network->inputNodes[i], &network->hiddenNodes[h]) * network->inputNodes[i].data;
        }
        data = sigmoid(data);
        network->hiddenNodes[h].data = data;
    }
    //Setting values of output nodes
    for(int o = 0; o < network->outputSize; o++) {
        double data = getWeight(network, &network->outputNodes[o], &network->bias) * network->bias.data;
        for(int h = 0; h < network->hiddenSize; h++) {
            data += getWeight(network, &network->hiddenNodes[h], &network->outputNodes[o]) * network->hiddenNodes[h].data;
        }
        data = sigmoid(data);
        network->outputNodes[o].data = data;
    }
}
double calculateErrors(Network *network) {
    double totalError = 0.0;
    //Setting error of output nodes
    for(int o = 0; o < network->outputSize; o++) {
        double error = (network->outputNodes[o].target - network->outputNodes[o].data) * sigmoidDerivative(network->outputNodes[o].data);
        network->outputNodes[o].error = error;
        totalError += pow(error, 2.0);
    }
    //Setting error of hidden nodes
    for(int h = 0; h < network->hiddenSize; h++) {
        double error = 0.0;
        for(int o = 0; o < network->outputSize; o++) {
            error += getWeight(network, &network->hiddenNodes[h], &network->outputNodes[o]) * network->outputNodes[o].error;
        }
        error *= sigmoidDerivative(network->hiddenNodes[h].data);
        network->hiddenNodes[h].error = error;
    }
    totalError = totalError / (double)network->outputSize;
    return sqrt(totalError);
}
void calculateNewWeights(Network *network) {
    for(int o = 0; o < network->outputSize; o++) {
        double newWeight = getWeight(network, &network->outputNodes[o], &network->bias) + (network->learningRate * network->outputNodes[o].error * network->bias.data);
        setWeight(network, &network->outputNodes[o], &network->bias, newWeight);
        
        for(int h = 0; h < network->hiddenSize; h++) {
            newWeight = getWeight(network, &network->hiddenNodes[h], &network->outputNodes[o]) + (network->learningRate * network->outputNodes[o].error * network->hiddenNodes[h].data);
            setWeight(network, &network->hiddenNodes[h], &network->outputNodes[o], newWeight);
        }
    }
    for(int h = 0; h < network->hiddenSize; h++) {
        double newWeight = getWeight(network, &network->hiddenNodes[h], &network->bias) + (network->learningRate * network->hiddenNodes[h].error * network->bias.data);
        setWeight(network, &network->hiddenNodes[h], &network->bias, newWeight);
        for(int i = 0; i < network->inputSize; i++) {
            newWeight = getWeight(network, &network->inputNodes[i], &network->hiddenNodes[h]) + (network->learningRate * network->hiddenNodes[h].error * network->inputNodes[i].data);
            setWeight(network, &network->inputNodes[i], &network->hiddenNodes[h], newWeight);
        }
    }
}
void train(Network *network, double **trainingInputs, double **trainingOutputs, double targetError) {
    double error = 999.9;
    int numInputs = sizeof(trainingInputs) / network->inputSize;
    int numOutputs = sizeof(trainingOutputs) / network->outputSize;
    double *inputs;
    double *outputs;
    int j = 0;
    while(error >= targetError) {
        int i = rand() % numInputs;
        inputs = trainingInputs[i];
        outputs = trainingOutputs[i];
        enterInputs(network, inputs);
        for(int x = 0; x < network->outputSize; x++) 
            network->outputNodes[x].target = outputs[x];        
        runNetwork(network); 
        error = calculateErrors(network);
        calculateNewWeights(network);
        if(j % 1000 == 0)
            printf("%.20lf\r", error);
        j++;
    }
    return;
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
    double targetError = 0.000000000000001;
    printf("%.20lf - target\n", targetError);
    double ** trainingInputs = (double**)malloc(sizeof(double*) * 4);
    double ** trainingOutputs = (double**)malloc(sizeof(double*) * 4);
    for(int i = 0; i < 4; i++) {
        trainingInputs[i] = (double*)malloc(sizeof(double) * 2);
        trainingOutputs[i] = (double*)malloc(sizeof(double) * 1);
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
    train(network, (double**)trainingInputs, (double**)trainingOutputs, targetError);

    //Testing
    for(int i = 0; i < 4; i++) {
        enterInputs(network, trainingInputs[i]);
        runNetwork(network);
        printf("%.1f   %.1f   |   %.2f vs %.1f\n", trainingInputs[i][0], trainingInputs[i][1], network->outputNodes[0].data, trainingOutputs[i][0]);
    }




    //Cleanup
    cleanup(network);
    for(int i = 0; i < 4; i++) {
        free(trainingInputs[i]);
        free(trainingOutputs[i]);
    }
    free(trainingInputs);
    free(trainingOutputs);
}