#ifndef NODE_H
#define NODE_H

typedef struct Node_t {
    double data, target, error;
    struct Node_t *bias;
    char *name;
}Node;

#endif