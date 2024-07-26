#include <sstream>
#include <iomanip>
#include <cassert>
#include "queue.h"
#include "engine.h"
#include "units.h"
#include "vector.h"
#include "stack.h"
#include "algorithms.h"
#include "field.h"
using namespace std;

// Fireball
void Fireball(Field& field, int row, int col, char direction, Vector<Unit*>& units){
    assert(direction == 'W' || direction == 'A' || direction == 'S' || direction == 'D');
    if (direction == 'W'){
        for (int i = row-1; i >= 0; i--){
            if (field.getTerrain(i, col) == MOUNTAIN){
                break;
            }
            else if (field.getUnit(i, col) != nullptr){
                field.removeUnit(i, col);
                for (int j = 0; j < units.size(); j++){
                    if (units[j] != nullptr && units[j] -> getRow() == i && units[j] -> getCol() == col){
                        delete units[j];
                        units[j] = nullptr;
                        break;
                    }
                }
                break;
            }
            else {
                continue;
            }
        }
    }
    else if (direction == 'A'){
        for (int i = col-1; i >= 0; i--){
            if (field.getTerrain(row, i) == MOUNTAIN){
                break;
            }
            else if (field.getUnit(row, i) != nullptr){
                field.removeUnit(row, i);
                for (int j = 0; j < units.size(); j++){
                    if (units[j] != nullptr && units[j] -> getRow() == row && units[j] -> getCol() == i){
                        delete units[j];
                        units[j] = nullptr;
                        break;
                    }
                }
                break;
            }
            else {
                continue;
            }
        }
    }
    else if (direction == 'S'){
        for (int i = row+1; i < field.getHeight(); i++){
            if (field.getTerrain(i, col) == MOUNTAIN){
                break;
            }
            else if (field.getUnit(i, col) != nullptr){
                field.removeUnit(i, col);
                for (int j = 0; j < units.size(); j++){
                    if (units[j] != nullptr && units[j] -> getRow() == i && units[j] -> getCol() == col){
                        delete units[j];
                        units[j] = nullptr;
                        break;
                    }
                }
                break;
            }
            else {
                continue;
            }
        }
    }
    else if (direction == 'D'){
        for (int i = col+1; i < field.getWidth(); i++){
            if (field.getTerrain(row, i) == MOUNTAIN){
                break;
            }
            else if (field.getUnit(row, i) != nullptr){
                field.removeUnit(row, i);
                for (int j = 0; j < units.size(); j++){
                    if (units[j] != nullptr && units[j] -> getRow() == row && units[j] -> getCol() == i){
                        delete units[j];
                        units[j] = nullptr;
                        break;
                    }
                }
                break;
            }
            else {
                continue;
            }
        }
    }
}

// Tornado
void Tornado(Field& field, int row, int col, Vector<Unit*>& units){
    for (int i = -1; i < 2; i++){
        for (int j = -1; j < 2; j++){
            if (i == 0 && j == 0){
                continue;
            }
            int r = row + i;
            int c = col + j;
            int tgr = r + i;
            int tgc = c + j;

            if (!field.inBounds(r, c)){
                continue;
            }

            Unit* u = field.getUnit(r, c);
            if (u == nullptr || !field.inBounds(tgr, tgc)){
                continue;
            }
            else if (field.getTerrain(tgr, tgc) == MOUNTAIN){
                continue;
            }
            else if (field.getTerrain(tgr, tgc) == WATER){
                field.removeUnit(r, c);
                for (int k = 0; k < units.size(); k++){
                    if (units[k] == u){
                        units[k] = nullptr;
                    }
                }
            }
            else if (field.getUnit(tgr, tgc) == nullptr){
                field.moveUnit(r, c, tgr, tgc);
                u -> setRow(tgr);
                u -> setCol(tgc);
            }
            else{
                Stack<Unit*> s;
                while (field.inBounds(tgr, tgc) && field.getUnit(tgr, tgc) != nullptr){
                    s.push(field.getUnit(r, c));
                    s.push(field.getUnit(tgr, tgc));
                    tgr += i;
                    tgc += j;
                }
                if (!field.inBounds(tgr, tgc) || field.getTerrain(tgr, tgc) == MOUNTAIN){
                    continue;
                }
                else if (field.getTerrain(tgr, tgc) == WATER){
                    if (!s.isEmpty()){
                        Unit* u_line = s.pop();
                        int srow = u_line -> getRow();
                        int scol = u_line -> getCol();
                        for (int k = 0; k < units.size(); k++){
                            if (units[k] == u_line){
                                delete u_line;
                                units[k] = nullptr;
                            }
                        }
                        field.removeUnit(srow, scol);
                    }
                    while (!s.isEmpty()){
                        Unit* u_line = s.pop();
                        int srow = u_line -> getRow();
                        int scol = u_line -> getCol();
                        field.moveUnit(srow, scol, srow+i, scol+j);
                        u_line -> setRow(srow+i);
                        u_line -> setCol(scol+j);
                        for (int m = 0; m < units.size(); m++){
                            if (units[m] == u_line){
                                units[m] = u_line;
                                break;
                            }
                        }
                    }
                }
                else{
                    while (!s.isEmpty()){
                        Unit* u_line = s.pop();
                        int srow = u_line -> getRow();
                        int scol = u_line -> getCol();
                        field.moveUnit(srow, scol, srow+i, scol+j);
                        u_line -> setRow(srow+i);
                        u_line -> setCol(scol+j);
                        for (int m = 0; m < units.size(); m++){
                            if (units[m] == u_line){
                                units[m] = u_line;
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
}

// Load map and units
Field* loadMap(istream& is, Vector<Unit*>& units)
{
    //Fill in your code here
    //先将地图的五个参数读入
    int height, width;
    is >> height >> width;
    if (!(height > 0 && height <=20 && width > 0 && width <= 20)){
        return nullptr;
    }
    int NT, NM, NG;
    is >> NT >> NM >> NG;
    if (!(NT >= 0 && NT <= height * width && NM >= 0 && NM <= height * width && NG >= 0 && NG <= height * width)){
        return nullptr;
    }

    //创建一个Field对象，并且把field的地形都设置为PLAIN
    Field* field = new Field(height, width);
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            field -> setTerrain(i, j, PLAIN);
        }
    }
    //将地形的参数读入，并且设置地形
    for (int i = 0; i < NT; i++){
        int row, col;
        is >> row >> col;
        char c;
        is >> c;
        Terrain t;
        if (c == 'W'){
            t = WATER;
        }
        else if (c == 'M'){
            t = MOUNTAIN;
        }
        else{
            return nullptr;
        }
        field -> setTerrain(row, col, t);
    }

    //将法师的参数读入，并且设置法师
    for (int i = 0; i < NM; i++){
        int row, col;
        is >> row >> col;
        Unit* u = new Unit(true, row, col);
        units.add(u);
        field -> setUnit(row, col, u);
    }
    //将哥布林的参数读入，并且设置哥布林
    for (int i = 0; i < NG; i++){
        int row, col;
        is >> row >> col;
        string type;
        is >> type;
        if (type == "PG") {
            char direction;
            is >> direction;
            int move;
            is >> move;
            Unit *u = new Unit(false, row, col, direction, move, Patrol);
            units.add(u);
            field->setUnit(row, col, u);
        }
        else if (type == "TG"){
            int vision;
            is >> vision;
            Unit* u = new Unit(false, row, col, ' ', 0, Tracking, vision);
            units.add(u);
            field -> setUnit(row, col, u);
        }
    }

    return field;
}


// Main loop for playing the game
void play(Field& field, istream& is, ostream& os, Vector<Unit*>& units){

    int numTurns = 1;
    while (is){
        os << "Turn " << numTurns << " begins:" << endl;
        // Print the new map
        os << field << endl;

        // Check winning
        // Fill in your code here
        int height;
        height = field.getHeight();
        int width;
        width = field.getWidth();

        int numMage = 0;
        int numGoblin = 0;
        for (int i = 0; i < units.size(); i++){
            if (units[i] == nullptr){
                continue;
            }
            else if (units[i] -> getSide() == true){
                numMage++;
            }
            else{
                numGoblin++;
            }
        }
        if (numMage == 0){
            os << "You Lose!" << endl;
            break;
        }
        else if (numGoblin == 0){
            os << "You Win!" << endl;
            break;
        }

        // unit moves
        // Fill in your code here
        for (int i = 0; i < units.size(); i++){
            Unit* u = units[i];
            if (u == nullptr){
                continue;
            }
            //移动法师
            if (u != nullptr && u -> getSide() == true) {
                int row = u->getRow();
                int col = u->getCol();
                cout << "Please move the unit at (" << row << "," << col << ")" << endl;

                string moves;
                if (numTurns == 1 && i == 0){
                    is.ignore();
                }
                getline(is, moves);

                int step = 0;
                for (int j = 0; j < moves.size() && step < 4; j++){
                    char c = moves[j];
                    if (c == ' '){
                        continue;
                    }
                    //控制法师的移动
                    else if (c == 'W') {
                        if (row - 1 < 0 || field.getUnit(row - 1, col) != nullptr || field.getTerrain(row - 1, col) == MOUNTAIN ||
                            field.getTerrain(row - 1, col) == WATER) {
                            continue;
                        }
                        else {
                            field.moveUnit(row, col, row - 1, col);
                            u->setRow(row - 1);
                            row--;
                        }
                    }
                    else if (c == 'A') {
                        if (col - 1 < 0 || field.getUnit(row, col - 1) != nullptr || field.getTerrain(row, col - 1) == MOUNTAIN ||
                            field.getTerrain(row, col - 1) == WATER) {
                            continue;
                        }
                        else {
                            field.moveUnit(row, col, row, col - 1);
                            u->setCol(col - 1);
                            col--;
                        }
                    }
                    else if (c == 'S') {
                        if (row + 1 >= height || field.getUnit(row + 1, col) != nullptr || field.getTerrain(row + 1, col) == MOUNTAIN ||
                            field.getTerrain(row + 1, col) == WATER) {
                            continue;
                        }
                        else {
                            field.moveUnit(row, col, row + 1, col);
                            u->setRow(row + 1);
                            row++;
                        }
                    }
                    else if (c == 'D') {
                        if (col + 1 >= width || field.getUnit(row, col + 1) != nullptr || field.getTerrain(row, col + 1) == MOUNTAIN ||
                            field.getTerrain(row, col + 1) == WATER) {
                            continue;
                        }
                        else {
                            field.moveUnit(row, col, row, col + 1);
                            u->setCol(col + 1);
                            col++;
                        }
                    }
                    step++;
                    //施法
                    if (moves[j] == 'X' || moves[j] == 'Y'){
                        if (moves[j] == 'X'){
                            j++;
                            if (moves[j] == ' '){
                                j++;
                            }
                            char c = moves[j];
                            Fireball(field, row, col, c, units);
                        }
                        else if (moves[j] == 'Y'){
                            Tornado(field, row, col, units);
                        }
                    }
                    else{
                        continue;
                    }
                }
                cout << field << endl;
            }
            //移动巡逻哥布林
            if (u != nullptr && u -> getSide() == false && u -> getType() == Patrol){
                int row = u->getRow();
                int col = u->getCol();
                char direction = u->getDirection();
                int move = u->getMove();
                bool turn = false;
                for (int i = 0; i < move; i++) {
                    // 判断上下左右有没有法师，有就杀，并且反转，没有就不管
                    if (field.inBounds(row - 1, col) && field.getUnit(row - 1, col) != nullptr && (field.getUnit(row - 1, col))->getSide()) {
                        field.removeUnit(row - 1, col);
                        for (int j = 0; j < units.size(); j++) {
                            if (units[j] != nullptr && units[j]->getRow() == row - 1 && units[j]->getCol() == col) {
                                delete units[j];
                                units[j] = nullptr;
                                break;
                            }
                        }
                        turn = true;
                    }
                    if (field.inBounds(row, col - 1) && field.getUnit(row, col-1) != nullptr && (field.getUnit(row, col - 1))->getSide()) {
                        field.removeUnit(row, col - 1);
                        for (int j = 0; j < units.size(); j++) {
                            if (units[j] != nullptr && units[j]->getRow() == row && units[j]->getCol() == col-1) {
                                delete units[j];
                                units[j] = nullptr;
                                break;
                            }
                        }
                        turn = true;
                    }
                    if (field.inBounds(row + 1, col) && field.getUnit(row+1, col) != nullptr && field.getUnit(row + 1, col)->getSide()) {
                        field.removeUnit(row + 1, col);
                        for (int j = 0; j < units.size(); j++) {
                            if (units[j] != nullptr && units[j]->getRow() == row + 1 && units[j]->getCol() == col) {
                                delete units[j];
                                units[j] = nullptr;
                                break;
                            }
                        }
                        turn = true;
                    }
                    if (field.inBounds(row, col + 1) && field.getUnit(row, col+1) != nullptr && field.getUnit(row, col + 1)->getSide()) {
                        field.removeUnit(row, col + 1);
                        for (int j = 0; j < units.size(); j++) {
                            if (units[j] != nullptr && units[j]->getRow() == row && units[j]->getCol() == col+1) {
                                delete units[j];
                                units[j] = nullptr;
                                break;
                            }
                        }
                        turn = true;
                    }
                    //反转方向
                    if (turn == true) {
                        if (direction == 'W') {
                            u->setDirection('S');
                        }
                        else if (direction == 'A') {
                            u->setDirection('D');
                        }
                        else if (direction == 'S') {
                            u->setDirection('W');
                        }
                        else if (direction == 'D') {
                            u->setDirection('A');
                        }
                        break;
                    }
                    // 目标坐标不符合行进条件
                    if (direction == 'W' && (row - 1 < 0 || field.getUnit(row - 1, col) != nullptr || field.getTerrain(row - 1, col) != PLAIN)) {
                        u->setDirection('S');
                        break;
                    }
                    else if (direction == 'A' && (col - 1 < 0 || field.getUnit(row, col - 1) != nullptr || field.getTerrain(row, col - 1) != PLAIN)) {
                        u->setDirection('D');
                        break;
                    }
                    else if (direction == 'S' && (row + 1 >= field.getHeight() || field.getUnit(row + 1, col) != nullptr || field.getTerrain(row + 1, col) != PLAIN)) {
                        u->setDirection('W');
                        break;
                    }
                    else if (direction == 'D' && (col + 1 >= field.getWidth() || field.getUnit(row, col + 1) != nullptr || field.getTerrain(row, col + 1) != PLAIN)) {
                        u->setDirection('A');
                        break;
                    }
                        // 成功移动
                    else {
                        if (direction == 'W') {
                            field.moveUnit(row, col, row - 1, col);
                            u->setRow(row - 1);
                            row--;
                            if (i == move - 1){
                                if (field.inBounds(row - 1, col) && field.getUnit(row - 1, col) != nullptr && (field.getUnit(row - 1, col))->getSide()) {
                                    field.removeUnit(row - 1, col);
                                    for (int j = 0; j < units.size(); j++) {
                                        if (units[j] != nullptr && units[j]->getRow() == row - 1 && units[j]->getCol() == col) {
                                            delete units[j];
                                            units[j] = nullptr;
                                            break;
                                        }
                                    }
                                    turn = true;
                                }
                                if (field.inBounds(row, col - 1) && field.getUnit(row, col-1) != nullptr && (field.getUnit(row, col - 1))->getSide()) {
                                    field.removeUnit(row, col - 1);
                                    for (int j = 0; j < units.size(); j++) {
                                        if (units[j] != nullptr && units[j]->getRow() == row && units[j]->getCol() == col-1) {
                                            delete units[j];
                                            units[j] = nullptr;
                                            break;
                                        }
                                    }
                                    turn = true;
                                }
                                if (field.inBounds(row + 1, col) && field.getUnit(row+1, col) != nullptr && field.getUnit(row + 1, col)->getSide()) {
                                    field.removeUnit(row + 1, col);
                                    for (int j = 0; j < units.size(); j++) {
                                        if (units[j] != nullptr && units[j]->getRow() == row + 1 && units[j]->getCol() == col) {
                                            delete units[j];
                                            units[j] = nullptr;
                                            break;
                                        }
                                    }
                                    turn = true;
                                }
                                if (field.inBounds(row, col + 1) && field.getUnit(row, col+1) != nullptr && field.getUnit(row, col + 1)->getSide()) {
                                    field.removeUnit(row, col + 1);
                                    for (int j = 0; j < units.size(); j++) {
                                        if (units[j] != nullptr && units[j]->getRow() == row && units[j]->getCol() == col+1) {
                                            delete units[j];
                                            units[j] = nullptr;
                                            break;
                                        }
                                    }
                                    turn = true;
                                }
                                u->setDirection('S');
                            }
                        }
                        else if (direction == 'A') {
                            field.moveUnit(row, col, row, col - 1);
                            u->setCol(col - 1);
                            col--;
                            if (i == move - 1){
                                if (field.inBounds(row - 1, col) && field.getUnit(row - 1, col) != nullptr && (field.getUnit(row - 1, col))->getSide()) {
                                    field.removeUnit(row - 1, col);
                                    for (int j = 0; j < units.size(); j++) {
                                        if (units[j] != nullptr && units[j]->getRow() == row - 1 && units[j]->getCol() == col) {
                                            delete units[j];
                                            units[j] = nullptr;
                                            break;
                                        }
                                    }
                                    turn = true;
                                }
                                if (field.inBounds(row, col - 1) && field.getUnit(row, col-1) != nullptr && (field.getUnit(row, col - 1))->getSide()) {
                                    field.removeUnit(row, col - 1);
                                    for (int j = 0; j < units.size(); j++) {
                                        if (units[j] != nullptr && units[j]->getRow() == row && units[j]->getCol() == col-1) {
                                            delete units[j];
                                            units[j] = nullptr;
                                            break;
                                        }
                                    }
                                    turn = true;
                                }
                                if (field.inBounds(row + 1, col) && field.getUnit(row+1, col) != nullptr && field.getUnit(row + 1, col)->getSide()) {
                                    field.removeUnit(row + 1, col);
                                    for (int j = 0; j < units.size(); j++) {
                                        if (units[j] != nullptr && units[j]->getRow() == row + 1 && units[j]->getCol() == col) {
                                            delete units[j];
                                            units[j] = nullptr;
                                            break;
                                        }
                                    }
                                    turn = true;
                                }
                                if (field.inBounds(row, col + 1) && field.getUnit(row, col+1) != nullptr && field.getUnit(row, col + 1)->getSide()) {
                                    field.removeUnit(row, col + 1);
                                    for (int j = 0; j < units.size(); j++) {
                                        if (units[j] != nullptr && units[j]->getRow() == row && units[j]->getCol() == col+1) {
                                            delete units[j];
                                            units[j] = nullptr;
                                            break;
                                        }
                                    }
                                    turn = true;
                                }
                                u->setDirection('D');
                            }
                        }
                        else if (direction == 'S') {
                            field.moveUnit(row, col, row + 1, col);
                            u->setRow(row + 1);
                            row++;
                            if (i == move - 1){
                                if (field.inBounds(row - 1, col) && field.getUnit(row - 1, col) != nullptr && (field.getUnit(row - 1, col))->getSide()) {
                                    field.removeUnit(row - 1, col);
                                    for (int j = 0; j < units.size(); j++) {
                                        if (units[j] != nullptr && units[j]->getRow() == row - 1 && units[j]->getCol() == col) {
                                            delete units[j];
                                            units[j] = nullptr;
                                            break;
                                        }
                                    }
                                    turn = true;
                                }
                                if (field.inBounds(row, col - 1) && field.getUnit(row, col-1) != nullptr && (field.getUnit(row, col - 1))->getSide()) {
                                    field.removeUnit(row, col - 1);
                                    for (int j = 0; j < units.size(); j++) {
                                        if (units[j] != nullptr && units[j]->getRow() == row && units[j]->getCol() == col-1) {
                                            delete units[j];
                                            units[j] = nullptr;
                                            break;
                                        }
                                    }
                                    turn = true;
                                }
                                if (field.inBounds(row + 1, col) && field.getUnit(row+1, col) != nullptr && field.getUnit(row + 1, col)->getSide()) {
                                    field.removeUnit(row + 1, col);
                                    for (int j = 0; j < units.size(); j++) {
                                        if (units[j] != nullptr && units[j]->getRow() == row + 1 && units[j]->getCol() == col) {
                                            delete units[j];
                                            units[j] = nullptr;
                                            break;
                                        }
                                    }
                                    turn = true;
                                }
                                if (field.inBounds(row, col + 1) && field.getUnit(row, col+1) != nullptr && field.getUnit(row, col + 1)->getSide()) {
                                    field.removeUnit(row, col + 1);
                                    for (int j = 0; j < units.size(); j++) {
                                        if (units[j] != nullptr && units[j]->getRow() == row && units[j]->getCol() == col+1) {
                                            delete units[j];
                                            units[j] = nullptr;
                                            break;
                                        }
                                    }
                                    turn = true;
                                }
                                u->setDirection('W');
                            }
                        }
                        else if (direction == 'D') {
                            field.moveUnit(row, col, row, col + 1);
                            u->setCol(col + 1);
                            col++;
                            if (i == move - 1){
                                if (field.inBounds(row - 1, col) && field.getUnit(row - 1, col) != nullptr && (field.getUnit(row - 1, col))->getSide()) {
                                    field.removeUnit(row - 1, col);
                                    for (int j = 0; j < units.size(); j++) {
                                        if (units[j] != nullptr && units[j]->getRow() == row - 1 && units[j]->getCol() == col) {
                                            delete units[j];
                                            units[j] = nullptr;
                                            break;
                                        }
                                    }
                                    turn = true;
                                }
                                if (field.inBounds(row, col - 1) && field.getUnit(row, col-1) != nullptr && (field.getUnit(row, col - 1))->getSide()) {
                                    field.removeUnit(row, col - 1);
                                    for (int j = 0; j < units.size(); j++) {
                                        if (units[j] != nullptr && units[j]->getRow() == row && units[j]->getCol() == col-1) {
                                            delete units[j];
                                            units[j] = nullptr;
                                            break;
                                        }
                                    }
                                    turn = true;
                                }
                                if (field.inBounds(row + 1, col) && field.getUnit(row+1, col) != nullptr && field.getUnit(row + 1, col)->getSide()) {
                                    field.removeUnit(row + 1, col);
                                    for (int j = 0; j < units.size(); j++) {
                                        if (units[j] != nullptr && units[j]->getRow() == row + 1 && units[j]->getCol() == col) {
                                            delete units[j];
                                            units[j] = nullptr;
                                            break;
                                        }
                                    }
                                    turn = true;
                                }
                                if (field.inBounds(row, col + 1) && field.getUnit(row, col+1) != nullptr && field.getUnit(row, col + 1)->getSide()) {
                                    field.removeUnit(row, col + 1);
                                    for (int j = 0; j < units.size(); j++) {
                                        if (units[j] != nullptr && units[j]->getRow() == row && units[j]->getCol() == col+1) {
                                            delete units[j];
                                            units[j] = nullptr;
                                            break;
                                        }
                                    }
                                    turn = true;
                                }
                                u->setDirection('A');
                            }
                        }
                    }
                }
            }
            //移动追踪哥布林
            else if (u != nullptr && u -> getSide() == false && u-> getType() == Tracking){
                int row = u-> getRow();
                int col = u-> getCol();
                int vision = u-> getVision();
                int probMinDistance;
                int minDistance = 725;
                pair <int, int> tgm;
                pair <int, int> tgt;
                for (int x = row-vision; x <= row+vision; x++){
                    for (int y = col-vision; y <= col+vision; y++){
                        if (!(x == row && y == col) && (abs(row-x)+abs(col-y) <= vision) && field.inBounds(x, y) && field.getUnit(x, y) != nullptr && field.getUnit(x, y) -> getSide()){
                            probMinDistance = searchShortestPath(field, row, col, x, y).first;
                            if (probMinDistance == -1){
                                continue;
                            }
                            else if (probMinDistance < minDistance) {
                                Vector<point> path = searchShortestPath(field, row, col, x, y).second;
                                minDistance = probMinDistance;
                                tgm = make_pair(x, y);
                                tgt = path[path.size()-2];
                            }
                            else if (probMinDistance == minDistance && coordOrder(x, y, tgm.first, tgm.second)){
                                Vector<point> path = searchShortestPath(field, row, col, x, y).second;
                                tgm = make_pair(x, y);
                                tgt = path[path.size()-2];
                            }
                        }
                    }
                }
                //攻击
                if (minDistance != 725){
                    field.moveUnit(row, col, tgt.first, tgt.second);
                    u -> setRow(tgt.first);
                    u -> setCol(tgt.second);
                    field.removeUnit(tgm.first, tgm.second);
                    for (int i = 0; i < units.size(); i++){
                        if (units[i] != nullptr && units[i] -> getRow() == tgt.first-1 && units[i] -> getCol() == tgt.second){
                            delete units[i];
                            units[i] = nullptr;
                            break;
                        }
                        if (units[i] != nullptr && units[i] -> getRow() == tgt.first && units[i] -> getCol() == tgt.second-1){
                            delete units[i];
                            units[i] = nullptr;
                            break;
                        }
                        if (units[i] != nullptr && units[i] -> getRow() == tgt.first+1 && units[i] -> getCol() == tgt.second){
                            delete units[i];
                            units[i] = nullptr;
                            break;
                        }
                        if (units[i] != nullptr && units[i] -> getRow() == tgt.first && units[i] -> getCol() == tgt.second+1){
                            delete units[i];
                            units[i] = nullptr;
                            break;
                        }
                    }
                }

            }
            //列表里这个元素是空指针，就跳过
            else{
                continue;
            }
        }
        numTurns++;
    }
}

