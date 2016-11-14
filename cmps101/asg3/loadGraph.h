// Isaiah Solomon
// icsolomo

/* loadGraph.h
 * (what is the purpose of this file?)
 */

#ifndef C101loadGraph
#define C101loadGraph

/** load graph
 */
IntList* loadGraph(int* n, int* m, int argc, char** argv);

/** print graph
 */
void print_list (IntList* adj_list, int n, int m);

/** transpose
 */
IntList* transposeGraph(IntList* orig_graph, int n);

/** fix graph
 */
IntList* fix_graph (IntList* orig_graph, int n);

/** cycle
 */
int hasCycle(IntList* orig_graph, int n);

int hasCycleLen(IntList* orig_graph, int n, int sofar, int v);

#endif