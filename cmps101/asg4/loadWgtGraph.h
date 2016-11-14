// Isaiah Solomon
// icsolomo

/* loadWgtGraph.h
 * (what is the purpose of this file?)
 * The purpose of this file is to take in input from a file or stdin and create a list out of it.
 */

#ifndef C101loadWgtGraph
#define C101loadWgtGraph

/** initialize graph
 */
EdgeList* initEdges (int n);

/** load graph
 */
EdgeList* loadEdges (int* n, int* m, int* o, int argc, char** argv);

/** print graph
 */
void print_list (EdgeList* adj_list, int n, int m);

/** transpose
 */
EdgeList* transposeGraph(EdgeList* orig_graph, int n);

/** fix graph
 */
EdgeList* fix_graph (EdgeList* orig_graph, int n);


#endif