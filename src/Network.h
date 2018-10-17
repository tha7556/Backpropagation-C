#ifndef NETWORK_H
#define NETWORK_H
#include "Node.h"
#include "library/Hashtable.h"

/**
 * A Neural Network composed of an input layer, hidden layer, and output layer.
 * It uses backpropagation to train itself.
**/
typedef struct Network_T {
    double learningRate;
    Node *inputNodes, *hiddenNodes, *outputNodes;
    int inputSize, hiddenSize, outputSize;
    HashTable *weightMap;
    Node bias;
} Network;
/**
 * Initializes the Network, the nodes inside of it, and the weights between each node
 * network: A Pointer to the Network to be initialized
 * inputs: The number of input nodes to create
 * hidden: The number of hidden nodes to create
 * outputs: The number of output nodes to create
**/
void initializeNetwork(Network *network, int inputs, int hidden, int outputs);
/**
 * Gets the weight in the Network between the two Nodes
 * network: A pointer to the Network
 * a: The first Node
 * b: The second Node
**/
double getWeight(Network *network, Node *a, Node *b);
/**
 * Enters the inputs into the Network. 
 * The length of the inputs array must be equal to the number of inputs.
 * network: A pointer to the Network
 * inputs: An array of numbers to input into the Network
**/
void enterInputs(Network *network, double *inputs);
/**
 * Runs through the Network once, leaving the results in the outputNodes
 * network: A pointer to the Network
**/
void runNetwork(Network *network);
/**
 * Trains the Network based on the trainingInputs/Outputs until the total error is less than or equal to the target error.
 * network: A pointer to the Network
 * trainingInputs: An array of each set of inputs to train on
 * trainingOutputs: An array of each set of outputs that correspond to the inputs
 * targetError: The minimum error before training can stop 
*/
void train(Network *network, double **trainingInputs, double **trainingOutputs, double targetError, int numInputs, int numOutputs);
/**
 * Frees all of the Nodes, the hashtable containing the weights, and the network itself.
**/
void cleanup(Network *network);

#endif