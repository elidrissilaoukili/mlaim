#include <graphics.h>
#include <iostream>
#include <cmath>

using namespace std;

// Function to draw the coordinate axes (repere) in the center
void drawRepere()
{
    int centerX = getmaxx() / 2; // Center X-coordinate
    int centerY = getmaxy() / 2; // Center Y-coordinate

    // Set color for axes
    setcolor(WHITE);

    // Draw X-axis (horizontal line)
    line(0, centerY, getmaxx(), centerY);

    // Draw Y-axis (vertical line)
    line(centerX, 0, centerX, getmaxy());

    // Label the axes
    outtextxy(centerX + 5, 5, (char *)"Y+");               // Positive Y direction
    outtextxy(centerX + 5, getmaxy() - 15, (char *)"Y-");  // Negative Y direction
    outtextxy(getmaxx() - 15, centerY - 15, (char *)"X+"); // Positive X direction
    outtextxy(5, centerY - 15, (char *)"X-");              // Negative X direction
}

// Function to project point P onto the plane
void perspectiveProjection(int x, int y, int z, int n1, int n2, int n3, int D, int &xp, int &yp)
{
    // Calculate scaling factor (lambda) for the projection
    float lambda = float(D) / (n1 * x + n2 * y + n3 * z);

    // Projected point coordinates
    xp = lambda * x; // X coordinate
    yp = lambda * y; // Y coordinate
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
    int xp[8], yp[8];
    for (int i = 0; i < 8; i++)
    {
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

int main()
{
    // Initialize graphics
    int g = DETECT, m;
    initgraph(&g, &m, (char *)"");
    setbkcolor(BLACK); // Set background color
    cleardevice();

    // Draw repere (coordinate axes)
    drawRepere();

    // Cube properties: position, size, and plane properties
    int x = 50, y = 50, z = 200, size = 50;
    int n1 = 0, n2 = 0, n3 = 1; // Plane normal (Z = d plane)
    int D = 100;                // Distance of the plane from origin

    // Draw the 3D cube
    drawCube(x, y, z, size, n1, n2, n3, D);

    // Wait for user input and close graphics
    getch();
    closegraph();

    return 0;
}
