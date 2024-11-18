#include <iostream>
#include <stdio.h>
#include <cmath>
#include <graphics.h>

class Point
{
public:
    int x, y;

    Point()
    {
        this->x = 0;
        this->y = 0;
    }

    Point(int x, int y)
    {
        this->x = x;
        this->y = y;
    }
};

