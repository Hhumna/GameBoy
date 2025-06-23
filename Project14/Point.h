#pragma once

class Point {
public:
    int x, y;

    Point(int x = 0, int y = 0) : x(x), y(y) {}

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }

    void setPosition(int newX, int newY) {
        x = newX;
        y = newY;
    }

    bool isWithinBounds(int width, int height) const {
        return x >= 0 && x < width && y >= 0 && y < height;
    }
};
