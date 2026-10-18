/* A doubly linked list module */
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include "globals.h"
#include "ll.h"


uint8_t 
ll_insert_blank(ll *list, ll_node *before, size_t size_of_data, uint32_t max_nodes, ll_node **return_node) {
/* Insert a blank node before the node 'before' in the doubly linked list 'list'. 
 * If 'before' is 0, append at head of doubly linked list. 
 * 'size_of_data' specifies the size of the node to add 
 *
 * Returns SUCCESS or EXIT_PROGRAM */
		ll_node *node;

        if(list->num_nodes == max_nodes) {
                fprintf(stderr, "Error: Exceeded max nodes allowed in linked list\n");
                return EXIT_PROGRAM;
        }
        if(!list) {
                fprintf(stderr, "Error: Pointer to a linked list must be "
                                 "supplied to linked list insert function\n");
                return EXIT_PROGRAM;
        }
        if(!size_of_data) {
                fprintf(stderr, "Error: Size of new blank node "
                                "missing\n");
                return EXIT_PROGRAM;
        }
		if(!return_node) {
				fprintf(stderr, "Error: Return node argument is null\n");
				return EXIT_PROGRAM;
		}

		/* Create new node on heap */
        node = calloc(1, size_of_data);

        if(!node) {
                perror("Error with calloc()");
                return EXIT_PROGRAM;
        }

		/* Attach new node on heap to doubly linked list */
        if(!list->num_nodes) { /* Making first node in list */
                list->head = node;
                list->tail = node;
        } else if (!before) { /* Inserting at list head */
                node->prior = list->head;
                list->head->next = node;
                list->head = node;
        } else if (!before->prior) { /* Inserting at list tail */
                node->next = list->tail;
                list->tail->prior = node;
                list->tail = node;
        } else { /* Inserting between two nodes */
                before->prior->next = node;
                node->prior = before->prior;
                node->next = before;
                before->prior = node;
        }
        (list->num_nodes)++;
		*return_node = node;
        return SUCCESS;

}


uint8_t 
ll_delete(ll *list, ll_node *node) {
/* Delete the node 'node' from doubly linked list 'list' 
 *
 * Returns SUCCESS or EXIT_PROGRAM */

        if(!list) {
                fprintf(stderr, "Error: Pointer to a linked list must be"
                                 "supplied to linked list delete function\n");
                return EXIT_PROGRAM;
        }
        if(!node) {
                fprintf(stderr, "Error: Pointer to a linked list node must be"
                                 "supplied to linked list delete function\n");
                return EXIT_PROGRAM;
        }
        if(list->num_nodes == 0) {
                fprintf(stderr,"Error: Performing node delete on empty linked list\n");
                return EXIT_PROGRAM;
        }
        if(list->num_nodes == 1) { /* Deleting only node */
                list->head = 0;
                list->tail = 0;
        } else if(!node->prior) { /* Deleting at tail */
                list->tail = node->next;
                node->next->prior = 0;
        } else if(!node->next) { /* Deleting at head */
                list->head = node->prior;
                node->prior->next = 0;
        } else { /* Deleting between two nodes */
                node->prior->next = node->next;
                node->next->prior = node->prior;
        }
       free(node);
       (list->num_nodes)--;
       return SUCCESS;

}


uint8_t 
/* TODO might be a bug here, check */
ll_new(ll **new_ll) {
         *new_ll = calloc(1, sizeof(ll));
         if(!*new_ll) {
                 perror("Error with calloc()");
                 return EXIT_PROGRAM;
        }
        return SUCCESS;
}
