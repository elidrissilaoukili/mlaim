#include "../lib.h"

// void cube()
// {
//     // top
//     setbkcolor(GREEN);
//     line(200, 100, 500, 100); // h
//     line(100, 200, 400, 200); // h
//     line(200, 100, 100, 200); // v
//     line(500, 100, 400, 200); // v

//     // bottom
//     line(200, 300, 500, 300); // h
//     line(100, 400, 400, 400); // h
//     line(200, 300, 100, 400); // v
//     line(500, 300, 400, 400); // v

//     // front
//     line(100, 200, 100, 400); // v
//     line(400, 200, 400, 400); // v

//     // back
//     line(200, 100, 200, 300); // v
//     line(500, 100, 500, 300); // v
// }

void drawCube() {
    // Define points for the cube
    int topFrontLeftX = 200, topFrontLeftY = 100;
    int topFrontRightX = 500, topFrontRightY = 100;
    int topBackLeftX = 100, topBackLeftY = 200;
    int topBackRightX = 400, topBackRightY = 200;
    int bottomFrontLeftX = 200, bottomFrontLeftY = 300;
    int bottomFrontRightX = 500, bottomFrontRightY = 300;
    int bottomBackLeftX = 100, bottomBackLeftY = 400;
    int bottomBackRightX = 400, bottomBackRightY = 400;

    // Top face
    line(topFrontLeftX, topFrontLeftY, topFrontRightX, topFrontRightY); // Top
    line(topFrontLeftX, topFrontLeftY, topBackLeftX, topBackLeftY);     // Left
    line(topFrontRightX, topFrontRightY, topBackRightX, topBackRightY); // Right
    line(topBackLeftX, topBackLeftY, topBackRightX, topBackRightY);     // Back

    // Bottom face
    line(bottomFrontLeftX, bottomFrontLeftY, bottomFrontRightX, bottomFrontRightY); // Bottom
    line(bottomFrontLeftX, bottomFrontLeftY, bottomBackLeftX, bottomBackLeftY);     // Left
    line(bottomFrontRightX, bottomFrontRightY, bottomBackRightX, bottomBackRightY); // Right
    line(bottomBackLeftX, bottomBackLeftY, bottomBackRightX, bottomBackRightY);     // Back

    // Vertical edges
    line(topFrontLeftX, topFrontLeftY, bottomFrontLeftX, bottomFrontLeftY);
    line(topFrontRightX, topFrontRightY, bottomFrontRightX, bottomFrontRightY);
    line(topBackLeftX, topBackLeftY, bottomBackLeftX, bottomBackLeftY);
    line(topBackRightX, topBackRightY, bottomBackRightX, bottomBackRightY);
}


int main()
{
    init_graph();

    drawCube();

    close_graph();
    return 0;
}