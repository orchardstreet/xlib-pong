/* A doubly linked list interface */
#ifndef ll_h
#define ll_h
#include <inttypes.h>
#include <stdlib.h>

typedef struct ll_node_struct {
        struct ll_node_struct* next;
        struct ll_node_struct* prior;
        char data[];
} ll_node;

typedef struct ll_struct {
        ll_node *head;
        ll_node *tail;
        uint32_t num_nodes;
} ll;


uint8_t ll_insert_blank(ll *list, ll_node *before, size_t size_of_data, uint32_t max_nodes, ll_node **return_node);
uint8_t ll_delete(ll *list, ll_node *node);
uint8_t ll_new(ll **new_ll);

#endif
