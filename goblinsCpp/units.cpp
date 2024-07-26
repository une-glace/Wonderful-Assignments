#include "units.h"
#include "field.h"
#include <cassert>
using namespace std;

/* Unit */

// Constructor
Unit::Unit(bool sd, int row, int col, char dir, int m, GoblinType type, int vision) : side(sd), row(row), col(col), direction(dir), move(m), type(type), vision(vision)
{
    assert (row >= 0 && col >= 0);
}


// Get the side
bool Unit::getSide() const
{
    return side;
}

// Get and set the row[col] of a unit
int Unit::getRow() const
{
    return row;
}

int Unit::getCol() const
{
    return col;
}

void Unit::setRow(int r)
{
    row = r;
}

void Unit::setCol(int c)
{
    col = c;
}

// Get and set the direction and move of a goblin
char Unit::getDirection() const
{
    return direction;
}

void Unit::setDirection(char dir)
{
    direction = dir;
}

int Unit::getMove() const
{
    return move;
}

// Get the goblin type
GoblinType Unit::getType() const
{
    return type;
}

// Get the vision of a goblin
int Unit::getVision() const
{
    return vision;
}

// Get the symbol of the unit
string getUnitSymbol(const Unit& u){
    bool side = u.getSide();
    if (side){
        return "@@";
    }
    else if (u.getType() == Patrol){
        return "PG";
    }
    else {
        return "TG";
    }

}


