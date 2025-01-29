#include "./lib.h"

// Function to project point P onto the plane
void perspectiveProjection(int x, int y, int z, int n1, int n2, int n3, int D, int &xp, int &yp)
{
    float lambda = float(D) / (n1 * x + n2 * y + n3 * z);

    xp = lambda * x;
    yp = lambda * y;
}

// Function to draw a 3D Cube
void drawCube(int x, int y, int z, int size, int n1, int n2, int n3, int D)
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

        perspectiveProjection(vertices[i][0], vertices[i][1], vertices[i][2], n1, n2, n3, D, xp[i], yp[i]);
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

// Function to draw a 3D cube using orthographic projection
void drawEndViewPoint(int x, int y, int z, int size, int n1, int n2, int n3, int D)
{
    // Define an offset for depth perception (simulated)
    int zOffset = size / 32;

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

    // Map vertices to 2D screen coordinates
    int xp[8], yp[8];
    for (int i = 0; i < 8; i++)
    {
        // Add zOffset to simulate depth
        xp[i] = vertices[i][0] + vertices[i][2] / zOffset + getmaxx() / 2;
        yp[i] = getmaxy() / 2 - (vertices[i][1] + vertices[i][2] / zOffset);

        // perspective
        perspectiveProjection(vertices[i][0], vertices[i][1], vertices[i][2], n1, n2, n3, D, xp[i], yp[i]);
    }

    // Draw the edges of the cube
    // Front face
    line(xp[0], yp[0], xp[1], yp[1]);
    line(xp[1], yp[1], xp[2], yp[2]);
    line(xp[2], yp[2], xp[3], yp[3]);
    line(xp[3], yp[3], xp[0], yp[0]);

    // Back face
    line(xp[4], yp[4], xp[5], yp[5]);
    line(xp[5], yp[5], xp[6], yp[6]);
    line(xp[6], yp[6], xp[7], yp[7]);
    line(xp[7], yp[7], xp[4], yp[4]);

    // Connecting edges
    line(xp[0], yp[0], xp[4], yp[4]);
    line(xp[1], yp[1], xp[5], yp[5]);
    line(xp[2], yp[2], xp[6], yp[6]);
    line(xp[3], yp[3], xp[7], yp[7]);
}

int main()
{
    init_graph();
    drawReperexyz();

    // Draw the 3D cube
    // int x = 0, y = 0, z = 200, size = 100;
    // int n1 = 0, n2 = 0, n3 = 1; // Plane normal (Z = d plane)
    // int D = 100;
    // drawCube(x, y, z, size, n1, n2, n3, D);

    // Draw a 3D cube with orthographic projection
    int x = 100, y = 100, z = -10, size = 100;
    int n1 = 0, n2 = 0, n3 = 1;
    int D = 50;
    drawEndViewPoint(x, y, z, size, n1, n2, n3, D);

    close_graph();

    return 0;
}
