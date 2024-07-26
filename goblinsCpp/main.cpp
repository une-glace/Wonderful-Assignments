#include <iostream>
#include <fstream>
#include "field.h"
#include "units.h"
#include "engine.h"

using namespace std;

int main(int argc, char* argv[])
{

    string filename = "map1.txt";


    Vector<Unit*> units;
    Field* f = loadMap(cin, units);
    if (f == nullptr) {
        cout << "Failed to load map!" << endl;
        return 0;
    }
    //cout << *f << endl;



    //play(*f, ifs, ofs);
    play(*f, cin, cout,units);

    delete f;
    for (int i = 0; i < units.size(); i++) {delete units[i];}

    return 0;
}
