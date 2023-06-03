
#include "ObjectQueue.h"
Node* next(Node *self) {
    return self->next;
}
Node* prev(Node *self) {
    return self->prev;
}

void ObjectQueue_init(ObjectQueue *self) {
    self->head = NULL;
    self->tail = NULL;
    self->size = 0;
}
ObjectQueue* new_ObjectQueue() {
    ObjectQueue *queue = (ObjectQueue*)malloc(sizeof(ObjectQueue));
    ObjectQueue_init(queue);
    return queue;
}
void push_front(ObjectQueue *self, Object* data) {
    Node *new_node = (Node*)malloc(sizeof(Node));
    new_node->obj = data;
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
void push_back(ObjectQueue *self, Object* body) {
    Node *new_node = (Node*)malloc(sizeof(Node));
    new_node->obj = body;
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
void erase(ObjectQueue *self, Node *node) {
    if (node->prev != NULL) {
        node->prev->next = node->next;
    }
    if (node->next != NULL) {
        node->next->prev = node->prev;
    }
    if (self->head == node) {
        self->head = node->next;
    }
    if (self->tail == node) {
        self->tail = node->prev;
    }
    free(node);
    self->size--;
}
Object* pop_front(ObjectQueue *self) {
    if (self->head == NULL) {
        printf("Error: pop_front() on empty queue\n");
        exit(1);
    }
    Node *node = self->head;
    Object* data = node->obj;
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
Object* pop_back(ObjectQueue *self) {
    if (self->tail == NULL) {
        printf("Error: pop_back() on empty queue\n");
        exit(1);
    }
    Node *node = self->tail;
    Object* data = node->obj;
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
Object* front(ObjectQueue *self) {
    if (self->head == NULL) {
        printf("Error: front() on empty queue\n");
        exit(1);
    }
    return self->head->obj;
}
Object* back(ObjectQueue *self) {
    if (self->tail == NULL) {
        printf("Error: back() on empty queue\n");
        exit(1);
    }
    return self->tail->obj;
}
bool isempty(ObjectQueue *self) {
    return self->size == 0;
}
void ObjectQueue_destroy(ObjectQueue *self) {
    if (self == NULL) return;
    while (!isempty(self)) {
        Object* obj = pop_front(self);
        obj->deleter(obj);
    }
}
void delete_ObjectQueue(ObjectQueue *self) {
    if (self != NULL && self->size > 0)
        raise_warn("Warning: destroying non-empty queue\n");
    ObjectQueue_destroy(self);
    free(self);
}