#ifndef ALGORITHMS_H_INCLUDED
#define ALGORITHMS_H_INCLUDED

/**** Algorithms for the game ****/
#include <Grid.h>
#include <Vector.h>
#include "field.h"

using namespace std;

typedef pair<int, int> point;

/** Path finding algorithm **/

// partial order of two points: (row1, col1) < (row2, col2)
bool coordOrder(int row1, int col1, int row2, int col2);

// Given source (row, col) and target (trow, tcol), calculate the
// shortest distance and shortest path from the source to target.
pair<int, Vector<point>> searchShortestPath(const Field & field, int row, int col, int trow, int tcol);

#endif // ALGORITHMS_H_INCLUDED
