//
// Created by 정주신 on 24. 5. 7.
//

#ifndef MULTIPLECHESS_LOCATION_H
#define MULTIPLECHESS_LOCATION_H
#include <vector>


int absolute(int n);


class Location {
private:
    int x;
    int y;

public:
    void move(int x, int y);
    void move(Location location);
    void moveRelative(int dx, int dy);
    int get_x();
    int get_y();

    bool equals(int x, int y);
    bool equals(Location location);

    Location();
    Location(int x, int y);

    bool isInBoard(int x, int y);
    bool isInBoard(Location location);
    bool isInBoard();
    bool isDiagonalFrom(int x , int y);
    bool isDiagonalFrom(Location location);
    bool isStraitFrom(int x, int y);
    bool isStraitFrom(Location location);
    int distanceFrom(int x, int y);
    int distanceFrom(Location location);
    bool isKnightFrom(int x, int y);
    bool isKnightFrom(Location location);
    Location &copy();
    std::vector<Location> diagonalLocations();
    std::vector<Location> straitLocations();
    std::vector<Location> knightLocations();
    std::vector<Location> oneDistanceLocations();
    static bool contains(std::vector<Location> locations, Location location);
};

static void clearVector(std::vector<Location> location);

#endif //MULTIPLECHESS_LOCATION_H
