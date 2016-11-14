/* minPQ.h (what is the purpose of this file?)
 * The purpose of this file is to do things with the ADT priority queue.
 */

#ifndef C101MinPQ
#define C101MinPQ
/* Multiple typedefs for the same type are an error in C. */

typedef struct MinPQNode * MinPQ;

struct MinPQNode
{
   int n, numPQ, minVertex;
   double oo;
   int* status;
   double* priority;
   int* parent;
};

#define UNSEEN ('u')
#define FRINGE ('f')
#define INTREE ('t')

/* Access functions (what are the preconditions?)
 * The precondition is that a MinPQ exists.
 */

/** isEmpty
*/
int isEmptyPQ(MinPQ pq);

/** getMin
*/
int getMin(MinPQ pq);

/** getStatus
*/
int getStatus(MinPQ pq, int id);

/** getParent
*/
int getParent(MinPQ pq, int id);

/** getPriority
*/
double getPriority(MinPQ pq, int id);

/* Manipulation procedures (what are the preconditions and postconditions?)
 * The preconditions are that the MinPQ must exist. The postcondition is the new MinPQ with 1 less or more size.
 */

/** delMin
*/
void delMin(MinPQ pq);

/** insertPQ
*/
void insertPQ(MinPQ pq, int id, double priority, int par);

/** decreaseKey
*/
void decreaseKey(MinPQ pq, int id, double priority, int par);

/* Constructors (what are the preconditions and postconditions?)
 * The precondition is that the arrays exist and that n is greater than 0. The postcondition is a new MinPQ of size n.
 */

/**
*/
MinPQ createPQ(int n, int status[], double priority[], int parent[]);


#endif

