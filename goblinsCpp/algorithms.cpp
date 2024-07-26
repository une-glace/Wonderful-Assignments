#include "algorithms.h"
#include <Vector.h>
#include <Grid.h>
#include <cassert>
#include <Queue.h>
using namespace std;


bool coordOrder(int row1, int col1, int row2, int col2){
    if (row1 < row2) return true;
    else if (row1 == row2) return col1 < col2;
    return false;
}

bool ReachableCoord(const Field&field, int row, int col){
    if (field.getUnit(row,col) != NULL){
        return false;
    }
    else {
        switch (field.getTerrain(row,col)){
            case Terrain::PLAIN : return true;
            default: return false;
        }
    }
}

typedef struct {
    point p;
    int distance;
    Vector<point> path;
} record;

std::pair<int, Vector<point>> searchShortestPath(const Field & field, int row, int col, int trow, int tcol){
    Vector<point> shortest_path;

    Queue<record> que;
    Grid<int> steps(field.getHeight(),field.getWidth(),-1);

    point source = point(row, col);
    point target = point(trow, tcol);

    assert(source!=target);

    que.enqueue(record{source, 0, Vector<point>()});

    int dir[4][2]={0,1,1,0,0,-1,-1,0};

    while (!que.isEmpty()){
        auto rec = que.dequeue();
        // update the steps for this point
        point p = rec.p;

        if(steps[p.first][p.second] == -1){
            // it is the first time to reach this point
            steps[p.first][p.second] = rec.distance;
            if(p == target){
                shortest_path = rec.path;
            }
        }
        else {
            assert(steps[p.first][p.second] <= rec.distance);

            // if this point can be reached with shorter steps then do nothing
            if (steps[p.first][p.second] < rec.distance) {
                if (p == target) break; // it is impossible to get a shorter path any more, so break
                else continue;
            }

            // find a shorter path to the target, select a path
            if(p == target){
                // update return paths
                if (steps[p.first][p.second] == rec.distance){
                    // we use the path whose end point is the least ordered
                    int path_size = shortest_path.size();
                    assert(path_size > 0);
                    // the last element in the path is the target
                    int end = path_size - 1;
                    if (end == 0) {
                        // source and target are adjoint
                        continue;
                    }
                    if (coordOrder(rec.path[end-1].first, rec.path[end-1].second, shortest_path[end-1].first, shortest_path[end-1].second)) {
                        shortest_path = rec.path;
                    }
                }
                else {
                    steps[p.first][p.second] = rec.distance;
                    shortest_path = rec.path;
                }
                continue;
            }
        }
        
        // enqueue four directions of p to the queue
        for(int i=0;i<4;++i){
            int x=p.first+dir[i][0];
            int y=p.second+dir[i][1];
            point next = point(x,y);
            if(field.inBounds(x,y) && (next == target || ReachableCoord(field, x, y))){
                auto new_path = rec.path;
                new_path.add(next); 
                que.enqueue(record{next, rec.distance + 1, new_path});
            }
        }
    }
    return pair<int, Vector<point>>(steps[trow][tcol], shortest_path);
}
