#include <string>
#include <cassert>
#include <iomanip>
#include "field.h"
#include "units.h"
using namespace std;

/* Battle field */

// Constructor
Field::Field(int h, int w)
        : units(h, w), terrains(h, w)
{
    assert (h > 0 && h <=20 && w > 0 && w <= 20);
}

// Get the height of the field
int Field::getHeight() const
{
    return units.numRows();
}

// Get the width of the field
int Field::getWidth() const
{
    return units.numCols();
}

// In bounds check
bool Field::inBounds(int row, int col) const
{
    return units.inBounds(row, col);
}

// Get the unit at row and col
Unit* Field::getUnit(int row, int col) const
{
    assert (units.inBounds(row, col));

    return units[row][col];
}

// Set a new unit at row and col
// Return false if the coordinate is already occupied
bool Field::setUnit(int row, int col, Unit* u)
{
    assert (units.inBounds(row, col));

    if (units[row][col] == nullptr) {
        units[row][col] = u;
        return true;
    }
    else
        return false;
}

// Remove the unit at row and col
// Return false if there is no unit at that coordinate
bool Field::removeUnit(int row, int col)
{
    assert(units.inBounds(row, col));

    if (units[row][col] != nullptr) {
        units[row][col] = nullptr;
        return true;
    }
    else
        return false;
}

// Move a unit to a different coordinate
// Return false if the source does not exist or
// the target coordinate is already occupied or
bool Field::moveUnit(int srow, int scol, int trow, int tcol)
{
    assert(units.inBounds(srow, scol) && units.inBounds(trow, tcol));

    if (units[srow][scol] == nullptr)
        return false;
    if (units[trow][tcol] != nullptr){
        return false;
    }
    else {
        units[trow][tcol] = units[srow][scol];
        units[srow][scol] = nullptr;
        return true;
    }
}

// Get and set the terrain of a square
Terrain Field::getTerrain(int row, int col) const
{
    assert (terrains.inBounds(row, col));

    return terrains[row][col];
}

void Field::setTerrain(int row, int col, Terrain t)
{
    assert (terrains.inBounds(row, col));

    terrains[row][col] = t;
}

// Reclaim all the units
Field::~Field()
{
    // Implement it if your want to delete the units via [field] instead of [units]
}



// Display the symbol for terrains
string getTerrainSymbol(Terrain t)
{
    switch (t) {
        case PLAIN:
            return "  ";
        case WATER:
            return "~~";
        case MOUNTAIN:
            return "/\\";
    }


    assert(false);
}

// Print the horizontal line
void printHLine(ostream& os, int n)
{
    os << "  ";
    for (int i = 0; i < n; i++)
        os << "+--";
    os << "+" << endl;
}

// Display field
ostream& operator<<(ostream& os, const Field& field)
{
    int height = field.getHeight();
    int width = field.getWidth();

    // Print the x coordinates
    os << "  ";
    for (int i = 0; i < width; i++)
        os << ' ' << setw(2) << i;
    os << endl;

    printHLine(os, width);
    for (int i = 0; i < height; i++) {
        os << setw(2) << i;
        for (int j = 0; j < width; j++) {
            os << '|';
            Unit* u = field.getUnit(i,j);
            if (u != nullptr)
                os << getUnitSymbol(*u);
            else {
                string ts = getTerrainSymbol(field.getTerrain(i,j));
                os << ts;
            }
        }
        os << '|' << endl;
        printHLine(os, width);
    }
    os << endl;
    return os;
}
