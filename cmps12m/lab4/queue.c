// Isaiah Solomon
// icsolomo
// 1344680
// CMPS 12B
// queue.c
// Creates a queue and implements unit tests to test the queue

#include <stdio.h>
#include <stdlib.h>

#include "minunit.h"

#define MAXSIZE 100

int tests_run = 0; // keep track of number of unit tests run

typedef enum q_status {
    /* Enumerated status codes for queue operations */
    q_success = 0,
    q_failure
} q_status;

/**** Private variables for queue ****/
q_status q_init(void);
q_status q_insert(int value);
q_status q_remove(int *value);
q_status q_peek(int *value);
q_status q_destroy(void);
int q_is_empty(void);

int queue[MAXSIZE];
int *pointer = NULL;
int front = -1;
int back = -1;

/**** Functions on queues ****/

q_status q_init(void) {
	if (pointer == NULL) {
		pointer = &queue[0];
		front = -1;
		back = -1;
		for (int i = 0; i < MAXSIZE; i++)
			queue[i] = 0;
		return q_success;
	}
	return q_failure;
}

q_status q_insert(int value) {
	if (pointer == NULL) {
		q_destroy();
		return q_failure;
	}
		
	*pointer = value;
	if (front == -1)						// First
		front = 0;
	if (pointer == &queue[99])				// Pointer
		pointer = &queue[0];
	else
		pointer++;
	if (back == MAXSIZE - 1 || back == -1)	// Back
		back = 0;
	else
		back++;
	return q_success;
}

q_status q_remove(int *value) {
	if (q_is_empty() == 1 || value == NULL) {
		q_destroy();
		return q_failure;
	}
		
	pointer = &queue[front];				// Pointer
	*value = *pointer;
	*pointer = 0;
	if (back == MAXSIZE - 1)
		pointer = &queue[0];
	else
		pointer = &queue[back] + 1;
	if (front == MAXSIZE - 1)				// Front
		front = 0;
	else
		front++;
	return q_success;
}

q_status q_peek(int *value) {
	if (q_is_empty() == 1 || value == NULL) {
		value = NULL;
		q_destroy();
		return q_failure;
	}
	
	pointer = &queue[front];
	*value = *pointer;
	if (back == MAXSIZE - 1)
		pointer = &queue[0];
	else
		pointer = &queue[back] + 1;
	return q_success;
}

q_status q_destroy(void) {
	if (pointer == NULL) {
		return q_failure;
	}
		
	pointer = NULL;
	front = -1;
	back = -1;
	for (int i = 0; i < MAXSIZE; i++)
		queue[i] = 0;
	return q_success;
}

int q_is_empty(void) {
	if (pointer == NULL || (front == -1 && back == -1) || pointer == &queue[front])
		return 1;
	else
		return 0;
}

/**** Unit tests ****/

char * test_lifecycle(void) {
    mu_assert("init", q_init() == q_success);
    mu_assert("destroy", q_destroy() == q_success);
    return NULL;
}

char * test_one_insert(void) {
    mu_assert("init", q_init() == q_success);
    mu_assert("insert", q_insert(7) == q_success);
    mu_assert("destroy", q_destroy() == q_success);
    return NULL;
}

char * test_one_insert_remove(void) {
    int v;
    mu_assert("init", q_init() == q_success);
    mu_assert("insert", q_insert(16) == q_success);
    mu_assert("remove", q_remove(&v) == q_success);
    mu_assert("value", v == 16);
    mu_assert("destroy", q_destroy() == q_success);
    return NULL;
}

char * test_two_insert_remove(void) {
    int v;
    mu_assert("init", q_init() == q_success);
    mu_assert("insert", q_insert(8) == q_success);
    mu_assert("insert", q_insert(91) == q_success);
    mu_assert("remove", q_remove(&v) == q_success);
    mu_assert("value", v == 8);
    mu_assert("remove", q_remove(&v) == q_success);
    mu_assert("value", v == 91);
    mu_assert("destroy", q_destroy() == q_success);
    return NULL;
}

char * test_multi_insert_remove(void) {
    int v;
    mu_assert("init", q_init() == q_success);
    mu_assert("insert", q_insert(8) == q_success);
    mu_assert("insert", q_insert(91) == q_success);
    for(int i = 0; i < 1000; i++) {
        mu_assert("insert", q_insert(8) == q_success);
        mu_assert("insert", q_insert(91) == q_success);
        mu_assert("remove", q_remove(&v) == q_success);
        mu_assert("value", v == 8);
        mu_assert("remove", q_remove(&v) == q_success);
        mu_assert("value", v == 91);
    }
    mu_assert("remove", q_remove(&v) == q_success);
    mu_assert("value", v == 8);
    mu_assert("destroy", q_destroy() == q_success);
    return NULL;
}

char * test_indexed_insert_remove(void) {
    int v;
    mu_assert("init", q_init() == q_success);
    for(int i = 0; i < 30; i++) {
        mu_assert("insert", q_insert(i) == q_success);
    }
    for(int i = 0; i < 1000; i++) {
        mu_assert("remove", q_remove(&v) == q_success);
        mu_assert("value", v == i);
        mu_assert("insert", q_insert(30 + i) == q_success);
    }
    mu_assert("destroy", q_destroy() == q_success);
    return NULL;
}

char * test_too_many_remove(void) {
    int v;
    mu_assert("init", q_init() == q_success);
    mu_assert("remove", q_remove(&v) == q_failure);
    return NULL;
}

char * test_insert_too_many_remove(void) {
    int v;
    mu_assert("init", q_init() == q_success);
    for(int i = 0; i < 30; i++) {
        mu_assert("insert", q_insert(i) == q_success);
    }
    for(int i = 0; i < 30; i++) {
        mu_assert("remove", q_remove(&v) == q_success);
        mu_assert("value", v == i);
    }
    mu_assert("remove", q_remove(&v) == q_failure);
    return NULL;
}

char * test_peek(void) {
    int v;
    mu_assert("init", q_init() == q_success);
    mu_assert("insert", q_insert(18) == q_success);
    mu_assert("peek", q_peek(&v) == q_success);
    mu_assert("value", v == 18);
    mu_assert("destroy", q_destroy() == q_success);
    return NULL;
}

char * test_peek_two(void) {
    int v;
    mu_assert("init", q_init() == q_success);
    mu_assert("insert", q_insert(18) == q_success);
    mu_assert("insert", q_insert(42) == q_success);
    mu_assert("peek", q_peek(&v) == q_success);
    mu_assert("value", v == 18);
    mu_assert("destroy", q_destroy() == q_success);
    return NULL;
}

char * test_peek_deep(void) {
    int v;
    mu_assert("init", q_init() == q_success);
    for(int i = 0; i < 30; i++) {
        mu_assert("insert", q_insert(i) == q_success);
    }
    for(int i = 0; i < 1000; i++) {
        mu_assert("peek", q_peek(&v) == q_success);
        mu_assert("value", v == i);
        mu_assert("remove", q_remove(&v) == q_success);
        mu_assert("value", v == i);
        mu_assert("insert", q_insert(30 + i) == q_success);
    }
    mu_assert("destroy", q_destroy() == q_success);
    return NULL;
}

char * test_peek_empty(void) {
    int v;
    mu_assert("init", q_init() == q_success);
    mu_assert("peek", q_peek(&v) == q_failure);
    return NULL;
}

char * test_peek_two_empty(void) {
    int v;
    mu_assert("init", q_init() == q_success);
    mu_assert("insert", q_insert(18) == q_success);
    mu_assert("insert", q_insert(42) == q_success);
    mu_assert("remove", q_remove(&v) == q_success);
    mu_assert("remove", q_remove(&v) == q_success);
    mu_assert("peek", q_peek(&v) == q_failure);
    return NULL;
}

char * test_remove_null(void) {
    mu_assert("init", q_init() == q_success);
    mu_assert("insert", q_insert(18) == q_success);
    mu_assert("peek", q_remove(NULL) == q_failure);
    return NULL;
}

char * test_peek_null(void) {
    mu_assert("init", q_init() == q_success);
    mu_assert("insert", q_insert(18) == q_success);
    mu_assert("peek", q_peek(NULL) == q_failure);
    return NULL;
}

char * test_check_integers(void) {
    int v;
    mu_assert("init", q_init() == q_success);
    mu_assert("insert", q_insert(1) == q_success);
    mu_assert("insert", q_insert(2) == q_success);
    mu_assert("insert", q_insert(3) == q_success);
	mu_assert("remove", q_remove(&v) == q_success);
    mu_assert("value", v == 1);
	mu_assert("remove", q_remove(&v) == q_success);
    mu_assert("value", v == 2);
	mu_assert("remove", q_remove(&v) == q_success);
    mu_assert("value", v == 3);
	mu_assert("destroy", q_destroy() == q_success);
    return NULL;
}

char * test_is_empty(void) {
    int v;
    mu_assert("init", q_init() == q_success);
	mu_assert("empty", q_is_empty() == 1);
    mu_assert("insert", q_insert(1) == q_success);
	mu_assert("remove", q_remove(&v) == q_success);
    mu_assert("value", v == 1);
	mu_assert("empty", q_is_empty() == 1);
	mu_assert("destroy", q_destroy() == q_success);
    return NULL;
}

char * test_is_not_empty(void) {
    mu_assert("init", q_init() == q_success);
    mu_assert("insert", q_insert(1) == q_success);
	mu_assert("empty", q_is_empty() == 0);
	mu_assert("destroy", q_destroy() == q_success);
    return NULL;
}

char * all_tests(void) {
    /* Run all unit tests */
    mu_run_test(test_lifecycle);
    mu_run_test(test_one_insert);
    mu_run_test(test_one_insert_remove);
    mu_run_test(test_two_insert_remove);
    mu_run_test(test_multi_insert_remove);
    mu_run_test(test_indexed_insert_remove);
    mu_run_test(test_too_many_remove);
    mu_run_test(test_insert_too_many_remove);
    mu_run_test(test_peek);
    mu_run_test(test_peek_two);
    mu_run_test(test_peek_deep);
    mu_run_test(test_peek_empty);
    mu_run_test(test_peek_two_empty);
    mu_run_test(test_remove_null);
    mu_run_test(test_peek_null);
	mu_run_test(test_check_integers);
	mu_run_test(test_is_empty);
	mu_run_test(test_is_not_empty);
    return NULL;
}

/**** Main unit test runner ****/

int main(int argc, char *argv[]) {
    printf("Queue Module unit tests\n");
    char *result = all_tests();
    if (result) {
        printf("FAILURE at %s\n", result);
    } else {
        printf("ALL TESTS PASS\n");
    }
    printf("Tests run: %d\n", tests_run);
    return EXIT_SUCCESS;
}
