#include "./lib.h"
#include "./line.h"

void perspectiveProjection(int x, int y, int z, int n1, int n2, int n3, int D, int &xp, int &yp)
{
    float lambda = float(D) / (n1 * x + n2 * y + n3 * z);

    xp = lambda * x;
    yp = lambda * y;
}

void obliqueProjection(int x, int y, int z, int &xp, int &yp)
{
    float alpha = M_PI / 4; // 100 degrees (angle for skewing)
    int scale = 2;
    xp = x + z * scale * cos(alpha);
    yp = y + z * scale * sin(alpha);
}

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

void projectCabinet(float x, float y, float z, int &x2d, int &y2d)
{
    x2d = x + z * cos(100 * M_PI / 180) / 2; // z/2 * cos(100)
    y2d = y + z * sin(100 * M_PI / 180) / 2; // z/2 * sin(100)
}

void projectCavalier(float x, float y, float z, int &x2d, int &y2d)
{
    x2d = x + z * cos(45 * M_PI / 180); // z * cos(45)
    y2d = y + z * sin(45 * M_PI / 180); // z * sin(45)
}

void drawCube(int x, int y, int z, int size, int n1, int n2, int n3, int D)
{
    int vertices[8][3] = {
        {x, y, z},
        {x + size, y, z},
        {x + size, y + size, z},
        {x, y + size, z},
        {x, y, z + size},
        {x + size, y, z + size},
        {x + size, y + size, z + size},
        {x, y + size, z + size}};

    int zOffset = size / 32;
    int xp[8], yp[8];
    for (int i = 0; i < 8; i++)
    {
        xp[i] = vertices[i][0] + vertices[i][2] / zOffset;
        yp[i] = (vertices[i][1] + vertices[i][2] / zOffset);

        perspectiveProjection(vertices[i][0], vertices[i][1], vertices[i][2], n1, n2, n3, D, xp[i], yp[i]);

        // obliqueProjection(vertices[i][0], vertices[i][1], vertices[i][2], xp[i], yp[i]);

        // perspectiveProjectionMatrix(vertices[i][0], vertices[i][1], vertices[i][2], D, xp[i], yp[i]);

        // projectCabinet(vertices[i][0], vertices[i][1], vertices[i][2], xp[i], yp[i]);

        // projectCavalier(vertices[i][0], vertices[i][1], vertices[i][2], xp[i], yp[i]);
    }

    putlines_v1(xp, yp);
}

int main()
{
    init_graph();
    drawReperexy();

    int n1 = 0, n2 = 0, n3 = 1, size = 100;
    int D = 100;

    // perspectiveProjection
    // int x = 0, y = 0, z = -200;
    // int x = 50, y = 50, z = -10;

    // obliqueProjection
    // int x = -50, y = -50, z = 50;

    // matrix perspective Projection
    // int x = 0, y = 0, z = 200;

    // cabinet & Cavalier
    int x = 50, y = 50, z = 0;

    drawCube(x, y, z, size, n1, n2, n3, D);

    // lines
    // displayLines(); 

    close_graph();

    return 0;
}
