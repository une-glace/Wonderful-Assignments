#ifndef UNITS_H_INCLUDED
#define UNITS_H_INCLUDED

#include <memory>
#include <string>
#include "field.h"


enum GoblinType {Patrol, Tracking};


/* Class for units */
class Unit {
public:

    // Constructor
    Unit(bool sd, int row, int col, char dir = ' ', int m = 0, GoblinType type = Patrol, int vision = 0);

    // Check which side the unit belongs to
    bool getSide() const;

    // Get and set row(col) of the unit
    int getRow() const;
    void setRow(int row);

    int getCol() const;
    void setCol(int col);

    // Get the direction and the move of a goblin
    char getDirection() const;
    void setDirection(char dir);

    int getMove() const;

    // Get the goblin type and vision
    GoblinType getType() const;
    int getVision() const;

private:
    // Position in the field
    bool side;
    int row;
    int col;
    char direction;
    int move;
    GoblinType type;
    int vision;
    // add other properties for future tasks
};

std::string getUnitSymbol(const Unit& u);
#endif // UNITS_H_INCLUDED
