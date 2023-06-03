// define a dequeue structure in c
#ifndef TQUEUE_H
#define TQUEUE_H

#define T int
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    T data;
    struct Node *next;
    struct Node *prev;
} Node;

Node* next(Node *self) {
    return self->next;
}
Node* prev(Node *self) {
    return self->prev;
}

typedef struct {
    Node *head;
    Node *tail;
    size_t size;
} BodyQueue;

BodyQueue* new_Tqueue() {
    BodyQueue *queue = (BodyQueue*)malloc(sizeof(BodyQueue));
    queue->head = NULL;
    queue->tail = NULL;
    queue->size = 0;
    return queue;
}

void push_front(BodyQueue *self, T data) {
    Node *new_node = (Node*)malloc(sizeof(Node));
    new_node->data = data;
    new_node->next = self->head;
    new_node->prev = NULL;
    if (self->head != NULL) {
        self->head->prev = new_node;
    }
    self->head = new_node;
    if (self->tail == NULL) {
        self->tail = new_node;
    }
    self->size++;
}

void push_back(BodyQueue *self, T data) {
    Node *new_node = (Node*)malloc(sizeof(Node));
    new_node->data = data;
    new_node->next = NULL;
    new_node->prev = self->tail;
    if (self->tail != NULL) {
        self->tail->next = new_node;
    }
    self->tail = new_node;
    if (self->head == NULL) {
        self->head = new_node;
    }
    self->size++;
}

T pop_front(BodyQueue *self) {
    if (self->head == NULL) {
        printf("Error: pop_front() on empty queue\n");
        exit(1);
    }
    Node *node = self->head;
    T data = node->data;
    self->head = node->next;
    if (self->head != NULL) {
        self->head->prev = NULL;
    }
    if (self->tail == node) {
        self->tail = NULL;
    }
    free(node);
    self->size--;
    return data;
}

T pop_back(BodyQueue *self) {
    if (self->tail == NULL) {
        printf("Error: pop_back() on empty queue\n");
        exit(1);
    }
    Node *node = self->tail;
    T data = node->data;
    self->tail = node->prev;
    if (self->tail != NULL) {
        self->tail->next = NULL;
    }
    if (self->head == node) {
        self->head = NULL;
    }
    free(node);
    self->size--;
    return data;
}

T front(BodyQueue *self) {
    if (self->head == NULL) {
        printf("Error: front() on empty queue\n");
        exit(1);
    }
    return self->head->data;
}

T back(BodyQueue *self) {
    if (self->tail == NULL) {
        printf("Error: back() on empty queue\n");
        exit(1);
    }
    return self->tail->data;
}

bool isempty(BodyQueue *self) {
    return self->size == 0;
}

void clear(BodyQueue *self, void (*free_data)(T) = NULL) {
    if (free_data != NULL)
        while (!isempty(self)) free_data(pop_front(self));
    else
        while (!isempty(self)) pop_front(self);
}

void destroy_Tqueue(BodyQueue *self, void (*free_data)(T) = NULL) {
    clear(self, free_data);
    free(self);
}



#endif // TQUEUE_H
