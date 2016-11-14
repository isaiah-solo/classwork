#include <stdio.h>
#include <stdlib.h>

struct node {
    int value;
    struct node *next;
};

int main(int argc, char *argv[]) {
    struct node *ptr = malloc(sizeof(struct node));
    ptr = malloc(sizeof(struct node));
    ptr->value = 19;
    ptr->next = NULL;
    printf("%p -> node{ %d, %p }\n", ptr, ptr->value, ptr->next);
    free(ptr);
    return EXIT_SUCCESS;
}
