// Isaiah Solomon
// icsolomo

/* edgeList.h
 * (what is the purpose of this file?)
 * The purpose of this file is to list the functions that are in EdgeList.c and other important structs.
 */

#ifndef C101EdgeList
#define C101EdgeList
/* Multiple typedefs for the same type are an error in C. */

typedef struct EdgeListNode * EdgeList;

typedef struct EdgeInfoNode
{
   int to;
   double wgt;
} EdgeInfo;

struct EdgeListNode
{
   EdgeList next;
   EdgeInfo value;
};

/** intNil denotes the empty EdgeList */
extern const EdgeList edgeNil;

/* Access functions
 * (what are the preconditions?)
 * The preconditions for both edgeFirst and edgeRest is a functioning EdgeList that has at least one element.
 * This is needed because both functions access the elements using the first element, so at least the first
 * is needed.
 */

/** first
 * oldL != edgeNil
 */
EdgeInfo edgeFirst(EdgeList oldL);

/** rest
 * oldL != edgeNil
 */ 
EdgeList edgeRest(EdgeList oldL);

/* Constructors
 * (what are the preconditions and postconditions?)
 * The preconditions for edgeCons is a functioning EdgeList that has at least one element and an integer
 * that is to be added to the EdgeList as a new element. The element should be greater than 1 and less
 * than n (provided in the input files). This is so that both the "to" and "from" are within range.
 * 
 * The postcondition is a functioning EdgeList with the new element newE added.
 */

/** cons
 */
EdgeList edgeCons(int newE, EdgeList oldL);

#endif
