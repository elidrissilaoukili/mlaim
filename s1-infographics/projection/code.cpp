#include <graphics.h>
#include <iostream>
#include <cmath>

using namespace std;

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

int main()
{
    // Initialize graphics
    int g = DETECT, m;
    initgraph(&g, &m, (char *)"");
    setbkcolor(RED);

    drawRepere();

    // Plane properties: normal vector and D
    int n1 = 0, n2 = 0, n3 = 1; // Plane normal (Z = d plane)
    int D = 100;                // Distance of the plane from origin

    // 3D Point P
    int x = 50, y = 50, z = 200;

    // Projected point coordinates
    int xp, yp;
    perspectiveProjection(x, y, z, n1, n2, n3, D, xp, yp);

    // Display original point
    putpixel(x + getmaxx() / 2, getmaxy() / 2 - y, WHITE);
    outtextxy(x + getmaxx() / 2, getmaxy() / 2 - y, (char *)"Original");

    // Display projected point
    putpixel(xp + getmaxx() / 2, getmaxy() / 2 - yp, RED);
    outtextxy(xp + getmaxx() / 2, getmaxy() / 2 - yp, (char *)"Projected");

    // Wait for user input and close graphics
    getch();
    closegraph();

    return 0;
}
