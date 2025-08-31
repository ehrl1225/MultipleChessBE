//
// Created by 정주신 on 24. 5. 7.
//

#include "Location.h"

int absolute(int n){
    if (n<0){
        return n*-1;
    }else{
        return n;
    }
}

void Location::move(int x, int y) {
    this->x = x;
    this->y = y;
}

void Location::move(Location location) {
    this->x = location.x;
    this->y = location.y;
}


bool Location::equals(int x, int y) {
    if (this->x == x && this->y == y){
        return true;
    }else{
        return false;
    }
}

bool Location::equals(Location location) {
    return equals(location.get_x(), location.get_y());
}

int Location::get_x() {
    return this->x;
}

int Location::get_y() {
    return this->y;
}

Location::Location(int x, int y) {
    this->x = x;
    this->y = y;
}

Location::Location() {
    x = 0;
    y = 0;
}


bool Location::isDiagonalFrom(int x, int y) {
    if (!isInBoard(x, y)){
        return false;
    }
    int diff_x = absolute(get_x() - x);
    int diff_y = absolute(get_y() - y);
    if (diff_x == diff_y){
        return true;
    }else{
        return false;
    };
}

bool Location::isDiagonalFrom(Location location) {
    return isDiagonalFrom(location.get_x(), location.get_y());
}

int Location::distanceFrom(int x, int y) {
    if (!isInBoard(x, y)){
        return false;
    }
    int diff_x = absolute(get_x() - x);
    int diff_y = absolute(get_y() - y);
    if (diff_x > diff_y){
        return diff_x;
    }else{
        return diff_y;
    }
}

int Location::distanceFrom(Location location) {
    return distanceFrom(location.get_x(), location.get_y());
}

bool Location::isStraitFrom(int x, int y) {
    if (!isInBoard(x, y)){
        return false;
    }
    if (get_x() == x){
        return true;
    }else if (get_y() == y){
        return true;
    }else{
        return false;
    }
}

bool Location::isStraitFrom(Location location) {
    return isStraitFrom(location.get_x(), location.get_y());
}

bool Location::isKnightFrom(int x, int y) {
    if (!isInBoard(x, y)){
        return false;
    }
    if (isStraitFrom(x,y)){
        return false;
    }
    if (isDiagonalFrom(x, y)){
        return false;
    }
    if (distanceFrom(x, y) == 2){
        return true;
    }
}

bool Location::isKnightFrom(Location location) {
    return isKnightFrom(location.get_x(), location.get_y());
}

bool Location::isInBoard(int x, int y) {
    if (x>0 && x<=8 && y>0 &&y<=8){
        return true;
    }else{
        return false;
    }
}

bool Location::isInBoard(Location location) {
    return isInBoard(location.get_x(), location.get_y());
}

bool Location::isInBoard() {
    return isInBoard(x, y);
}

std::vector<Location> Location::diagonalLocations() {
    std::vector<Location> locations;

    for (int distance = 1 ; distance<8 ; distance++){
        int negative_x = x - distance;
        int positive_x = x + distance;
        int negative_y = y - distance;
        int positive_y = y + distance;
        Location locs[4];
        locs[0] = Location(negative_x, negative_y);
        locs[1] = Location(negative_x, positive_y);
        locs[2] = Location(positive_x, negative_y);
        locs[3] = Location(positive_x, positive_y);
        for (Location loc : locs){
            if (loc.isInBoard()){
                locations.push_back(loc);
            }
        }
    }
    return locations;
}

std::vector<Location> Location::straitLocations() {
    std::vector<Location> locations;
    for (int x = 1; x<=8; x++){
        if (x != this->x){
            Location loc = Location(x, y);
            locations.push_back(loc);
        }
    }
    for (int y = 1; y<=8;y++){
        if (y != this->y){
            Location loc = Location(x, y);
            locations.push_back(loc);
        }
    }
    return locations;
}

std::vector<Location> Location::knightLocations() {
    std::vector<Location> locations;
    Location locs[8];
    locs[0] = Location(x-1, y+2);
    locs[1] = Location(x+1, y+2);
    locs[2] = Location(x-2, y+1);
    locs[3] = Location(x-2, y-1);
    locs[4] = Location(x-1, y-2);
    locs[5] = Location(x+1, y-2);
    locs[6] = Location(x+2, y+1);
    locs[7] = Location(x+2, y-1);
    for (Location loc : locs){
        if (loc.isInBoard()){
            locations.push_back(loc);
        }
    }
    return locations;
}

std::vector<Location> Location::oneDistanceLocations() {
    std::vector<Location> locations;
    Location locs[8];
    locs[0] = Location(x+1, y+1);
    locs[1] = Location(x+1, y);
    locs[2] = Location(x+1, y-1);
    locs[3] = Location(x, y-1);
    locs[4] = Location(x-1, y-1);
    locs[5] = Location(x-1, y);
    locs[6] = Location(x-1, y+1);
    locs[7] = Location(x, y+1);

    for (Location loc : locs){
        if (loc.isInBoard()){
            locations.push_back(loc);
        }
    }
    return locations;
}

static void clearVector(std::vector<Location> location) {
    location.clear();
    std::vector<Location>().swap(location);
}

Location &Location::copy() {
    Location loc = Location(x,y);
    return loc;
}

void Location::moveRelative(int dx, int dy) {
    this->x += dx;
    this->y += dy;
}

bool Location::contains(std::vector<Location> locations, Location location) {
    bool result = false;
    for (Location loc:locations) {
        if (loc.equals(location)) {
            result = true;
            break;
        }
    }
    return result;
}

