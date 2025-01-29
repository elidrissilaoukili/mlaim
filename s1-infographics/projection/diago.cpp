#include "./lib.h"

// Function to draw a 3D Cube
void drawCube(int x, int y, int z, int size, int D)
{
    // Cube vertices (8 corners of the cube)
    int vertices[8][3] = {
        {x, y, z},
        {x + size, y, z},
        {x + size, y + size, z},
        {x, y + size, z},
        {x, y, z + size},
        {x + size, y, z + size},
        {x + size, y + size, z + size},
        {x, y + size, z + size}};

    // Project the vertices onto the 2D plane
    int zOffset = size / 32;
    int xp[8], yp[8];
    for (int i = 0; i < 8; i++)
    {
        xp[i] = vertices[i][0] + vertices[i][2] / zOffset;
        yp[i] = (vertices[i][1] + vertices[i][2] / zOffset);
    }

    // Draw the edges of the cube by connecting the projected vertices
    // Front square
    line(xp[0] + getmaxx() / 2, getmaxy() / 2 - yp[0], xp[1] + getmaxx() / 2, getmaxy() / 2 - yp[1]);
    line(xp[1] + getmaxx() / 2, getmaxy() / 2 - yp[1], xp[2] + getmaxx() / 2, getmaxy() / 2 - yp[2]);
    line(xp[2] + getmaxx() / 2, getmaxy() / 2 - yp[2], xp[3] + getmaxx() / 2, getmaxy() / 2 - yp[3]);
    line(xp[3] + getmaxx() / 2, getmaxy() / 2 - yp[3], xp[0] + getmaxx() / 2, getmaxy() / 2 - yp[0]);

    // Back square
    line(xp[4] + getmaxx() / 2, getmaxy() / 2 - yp[4], xp[5] + getmaxx() / 2, getmaxy() / 2 - yp[5]);
    line(xp[5] + getmaxx() / 2, getmaxy() / 2 - yp[5], xp[6] + getmaxx() / 2, getmaxy() / 2 - yp[6]);
    line(xp[6] + getmaxx() / 2, getmaxy() / 2 - yp[6], xp[7] + getmaxx() / 2, getmaxy() / 2 - yp[7]);
    line(xp[7] + getmaxx() / 2, getmaxy() / 2 - yp[7], xp[4] + getmaxx() / 2, getmaxy() / 2 - yp[4]);

    // Connecting front and back squares (edges of the cube)
    line(xp[0] + getmaxx() / 2, getmaxy() / 2 - yp[0], xp[4] + getmaxx() / 2, getmaxy() / 2 - yp[4]);
    line(xp[1] + getmaxx() / 2, getmaxy() / 2 - yp[1], xp[5] + getmaxx() / 2, getmaxy() / 2 - yp[5]);
    line(xp[2] + getmaxx() / 2, getmaxy() / 2 - yp[2], xp[6] + getmaxx() / 2, getmaxy() / 2 - yp[6]);
    line(xp[3] + getmaxx() / 2, getmaxy() / 2 - yp[3], xp[7] + getmaxx() / 2, getmaxy() / 2 - yp[7]);
}

int main()
{
    init_graph();
    drawReperexyz();

    int x = 0, y = 0, z = 300;
    int size = 100;
    int D = 500;

    drawCube(x, y, z, size, D);

    close_graph();

    return 0;
}
