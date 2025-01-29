#include "./lib.h"

// Function to apply cabinet projection
void projectCabinet(float x, float y, float z, int &x2d, int &y2d)
{
    x2d = x + z * cos(100 * M_PI / 180) / 2; // z/2 * cos(100)
    y2d = y + z * sin(100 * M_PI / 180) / 2; // z/2 * sin(100)
}

// Function to draw a 3D cube using cabinet projection
void drawCube(int x, int y, int z, int size)
{
    int zOffset = size / 32;

    // Cube vertices (8 corners)
    int vertices[8][3] = {
        {x, y, z},
        {x + size, y, z},
        {x + size, y + size, z},
        {x, y + size, z},
        {x, y, z + size},
        {x + size, y, z + size},
        {x + size, y + size, z + size},
        {x, y + size, z + size}};

    // Project vertices onto the 2D plane
    int xp[8], yp[8];
    for (int i = 0; i < 8; i++)
    {
        xp[i] = vertices[i][0] + vertices[i][2] / zOffset + getmaxx() / 2;
        yp[i] = getmaxy() / 2 - (vertices[i][1] + vertices[i][2] / zOffset);

        projectCabinet(vertices[i][0], vertices[i][1], vertices[i][2], xp[i], yp[i]);
    }

    // Center the cube on the screen
    int xOffset = getmaxx() / 2;
    int yOffset = getmaxy() / 2;

    // Draw edges of the cube
    // Front face

    line(xp[0] + xOffset, yOffset - yp[0], xp[1] + xOffset, yOffset - yp[1]);
    line(xp[1] + xOffset, yOffset - yp[1], xp[2] + xOffset, yOffset - yp[2]);
    line(xp[2] + xOffset, yOffset - yp[2], xp[3] + xOffset, yOffset - yp[3]);
    line(xp[3] + xOffset, yOffset - yp[3], xp[0] + xOffset, yOffset - yp[0]);

    // Back face
    line(xp[4] + xOffset, yOffset - yp[4], xp[5] + xOffset, yOffset - yp[5]);
    line(xp[5] + xOffset, yOffset - yp[5], xp[6] + xOffset, yOffset - yp[6]);
    line(xp[6] + xOffset, yOffset - yp[6], xp[7] + xOffset, yOffset - yp[7]);
    line(xp[7] + xOffset, yOffset - yp[7], xp[4] + xOffset, yOffset - yp[4]);

    // Connecting edges
    line(xp[0] + xOffset, yOffset - yp[0], xp[4] + xOffset, yOffset - yp[4]);
    line(xp[1] + xOffset, yOffset - yp[1], xp[5] + xOffset, yOffset - yp[5]);
    line(xp[2] + xOffset, yOffset - yp[2], xp[6] + xOffset, yOffset - yp[6]);
    line(xp[3] + xOffset, yOffset - yp[3], xp[7] + xOffset, yOffset - yp[7]);
}

int main()
{
    init_graph();

    drawReperexy();

    int x = 50, y = 50, z = 0;
    int size = 100;
    drawCube(x, y, z, size);

    close_graph();
    return 0;
}
