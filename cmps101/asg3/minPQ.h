/* minPQ.h (what is the purpose of this file?)
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
*/

/**
*/
MinPQ createPQ(int n, int status[], double priority[], int parent[]);


#endif

