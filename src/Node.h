#ifndef NODE_H
#define NODE_H

/**
 * A single Node inside of the Neural Network
**/
typedef struct Node_t {
    double data, target, error;
    struct Node_t *bias; //A pointer to the bias Node if it has one
    char *name;
}Node;

#endif