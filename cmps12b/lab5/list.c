// Isaiah Solomon
// icsolomo
// 1344680
// CMPS 12B
// list.c
// Builds and works with a singly-linked list

#include <stdio.h>
#include <stdlib.h>

// A node in a singly-linked list
struct node {
    int value;
    struct node *next;
};

// Head of the linked list
struct node *head;

// Insert a value into linked list
void list_insert(int v) {
    struct node *n;
    n = (struct node *)malloc(sizeof(struct node));
    n->value = v;
    n->next = head;
    head = n;
}

// Insert two values at once into linked list
void list_insert2(int a, int b) {
    struct node *u, *v;
	u = (struct node *)malloc(sizeof(struct node));
	v = (struct node *)malloc(sizeof(struct node));
    v->value = b;
    v->next = head;
    u->value = a;
    u->next = v;
    head = u;
}

// Remove an element from linked list
void list_remove(int v) {
    struct node *n = head;
	struct node *temp;
    while(n && n->next && n->next->value != v) {
        n = n->next;
    }
    if(n && n->next && n->next->value == v) {
		temp = n->next;
        n->next = n->next->next;
		free(temp);
    }
}

// Print out all values in linked list
void list_printall(void) {
    struct node *p = head;
    int n = 0;
    while(p) {
        printf("%d ", p->value);
        p = p->next;
        n++;
        if(n > 100) break;
    }
    printf("\n");
}

// Deallocate all memory used in linked list
void list_destroy(void) {
	struct node *pointer1 = head;
	while (pointer1) {
		struct node *pointer2 = pointer1;
		pointer1 = pointer1->next;
		free(pointer2);
	}
}

int main(int argc, char *argv[]) {
    printf("Test linked lists\n");
    list_printall(); // Should print nothing
    list_insert(42);
    list_insert2(17, 10);
    list_insert(18);
    list_remove(10);
    list_printall(); // Should print 18 17 42
    // Cleanup memory
    list_destroy();
    return 0;
}
