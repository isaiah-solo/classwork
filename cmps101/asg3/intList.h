// Isaiah Solomon
// icsolomo

/* intList.h
 * (what is the purpose of this file?)
 * The purpose of this file is to list the functions that are in IntList.c and other important structs.
 */

#ifndef C101IntList
#define C101IntList
/* Multiple typedefs for the same type are an error in C. */

typedef struct IntListNode * IntList;

struct IntListNode
{
   IntList next;
   int value;
};

/** intNil denotes the empty IntList */
extern const IntList intNil;

/* Access functions
 * (what are the preconditions?)
 * The preconditions for both intFirst and intRest is a functioning IntList that has at least one element.
 * This is needed because both functions access the elements using the first element, so at least the first
 * is needed.
 */

/** first
 */
int intFirst(IntList oldL);

/** rest
 */
IntList intRest(IntList oldL);

/* Constructors
 * (what are the preconditions and postconditions?)
 * The preconditions for intCons is a functioning IntList that has at least one element and an integer
 * that is to be added to the IntList as a new element. The element should be greater than 1 and less
 * than n (provided in the input files). This is so that both the "to" and "from" are within range.
 * 
 * The postcondition is a functioning IntList with the new element newE added.
 */

/** cons
 */
IntList intCons(int newE, IntList oldL);

#endif
