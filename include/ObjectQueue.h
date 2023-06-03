// define a dequeue structure in c
#ifndef OQUEUE_H
#define OQUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <Object.h>

typedef struct Node {
    Object* obj;
    struct Node *next;
    struct Node *prev;
} Node;

Node* next(Node *self);
Node* prev(Node *self);

typedef struct ObjectQueue {
    Node *head;
    Node *tail;
    size_t size;
} ObjectQueue;

void ObjectQueue_init(ObjectQueue *self);
ObjectQueue* new_ObjectQueue();
void push_front(ObjectQueue *self, Object* data);
void push_back(ObjectQueue *self, Object* body);
void erase(ObjectQueue *self, Node *node);
Object* pop_front(ObjectQueue *self);
Object* pop_back(ObjectQueue *self);
Object* front(ObjectQueue *self);
Object* back(ObjectQueue *self);
bool isempty(ObjectQueue *self);
void ObjectQueue_destroy(ObjectQueue *self);
void delete_ObjectQueue(ObjectQueue *self);

#endif // OQUEUE_H
