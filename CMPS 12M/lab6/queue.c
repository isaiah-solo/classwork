//
// Isaiah Solomon
// icsolomo
// CMPS 12B
// queue.c
// This file contains functions for a queue
//

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "queue.h"

#define STUBPRINTF(...) fprintf(stderr, __VA_ARGS__);

/* Internal implementation definitions */
struct queue_node {
	queue_item_t item;
	struct queue_node *link;
};

typedef struct queue_node queue_node;

struct queue {
	queue_node *front;
	queue_node *rear;
};

/* Functions */

queue *queue_new(void) {
	queue *new_queue = malloc(sizeof(queue));
	new_queue->front = NULL;
	new_queue->rear = NULL;
	return new_queue;
}

void queue_free(queue *this) {
	assert(queue_isempty(this));
	free(this);
	this = NULL;
}

void queue_insert(queue *this, queue_item_t item) {
	queue_node *temp = malloc(sizeof(queue_node));
	temp->item = item;
	temp->link = NULL;
	if (queue_isempty(this)) {
		this->front = temp;
		this->rear = temp;
	} else {
		this->rear->link = temp;
		this->rear = temp;
	}
}

queue_item_t queue_remove(queue *this) {
	bool check = false;
	if (this->front == this->rear)
		check = true;
	assert(!queue_isempty(this));
	queue_node *pointer = this->front->link;
	queue_item_t temp_item = this->front->item;
	free(this->front);
	this->front = pointer;
	if (check == true)
		this->front = NULL;
	return temp_item;
}

bool queue_isempty(queue *this) {
	return this->front == NULL;
}
