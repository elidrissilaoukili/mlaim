#include "./lib.h"

void perspectiveProjectionMatrix(int x, int y, int z, int D, int &xp, int &yp)
{
    float P[4][4] = {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, -1.0f / D},
        {0, 0, 1.0f / D, 0}};

    // Homogeneous coordinates for the 3D point
    float point[4] = {float(x), float(y), float(z), 1};

    // Multiply the point by the perspective projection matrix
    float result[4] = {0, 0, 0, 0};
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            result[i] += P[i][j] * point[j];

    // Perform perspective divide to get 2D screen coordinates
    xp = result[0] / result[3];
    yp = result[1] / result[3];
}

void perspectiveProjectionMatrix2(int x, int y, int z, int D, int &xp, int &yp)
{
    int n1 =0, n2 = 0, n3 = 1;
    
    float P[4][4] = {
        {1 + x, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, -1.0f / D},
        {0, 0, 1.0f / D, 0}};

    // Homogeneous coordinates for the 3D point
    float point[4] = {float(x), float(y), float(z), 1};

    // Multiply the point by the perspective projection matrix
    float result[4] = {0, 0, 0, 0};
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            result[i] += P[i][j] * point[j];

    // Perform perspective divide to get 2D screen coordinates
    xp = result[0] / result[3];
    yp = result[1] / result[3];
}

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

        // perspectiveProjectionMatrix(vertices[i][0], vertices[i][1], vertices[i][2], D, xp[i], yp[i]);
        perspectiveProjectionMatrix2(vertices[i][0], vertices[i][1], vertices[i][2], D, xp[i], yp[i]);
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

    drawReperexyz();
    drawCube(x, y, z, size, D);

    close_graph();

    return 0;
}
