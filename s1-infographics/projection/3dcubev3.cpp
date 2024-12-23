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

// The oblique projection
void obliqueProjection(int x, int y, int z, float alpha, int scale, int &xp, int &yp)
{
    // Calculate the 2D projection
    xp = x + z * scale * cos(alpha); // Skew x based on z
    yp = y + z * scale * sin(alpha); // Skew y based on z
}

void drawCubeOblique(int x, int y, int z, int size, float alpha, int scale)
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

    // Project the vertices using oblique projection
    int zOffset = size / 32;
    int xp[8], yp[8];
    for (int i = 0; i < 8; i++)
    {

        xp[i] = vertices[i][0] + vertices[i][2] / zOffset;
        yp[i] = (vertices[i][1] + vertices[i][2] / zOffset);

        obliqueProjection(vertices[i][0], vertices[i][1], vertices[i][2], alpha, scale, xp[i], yp[i]);
    }

    // Draw the edges of the cube
    // Front face
    line(xp[0] + getmaxx() / 2, getmaxy() / 2 - yp[0], xp[1] + getmaxx() / 2, getmaxy() / 2 - yp[1]);
    line(xp[1] + getmaxx() / 2, getmaxy() / 2 - yp[1], xp[2] + getmaxx() / 2, getmaxy() / 2 - yp[2]);
    line(xp[2] + getmaxx() / 2, getmaxy() / 2 - yp[2], xp[3] + getmaxx() / 2, getmaxy() / 2 - yp[3]);
    line(xp[3] + getmaxx() / 2, getmaxy() / 2 - yp[3], xp[0] + getmaxx() / 2, getmaxy() / 2 - yp[0]);

    // Back face
    line(xp[4] + getmaxx() / 2, getmaxy() / 2 - yp[4], xp[5] + getmaxx() / 2, getmaxy() / 2 - yp[5]);
    line(xp[5] + getmaxx() / 2, getmaxy() / 2 - yp[5], xp[6] + getmaxx() / 2, getmaxy() / 2 - yp[6]);
    line(xp[6] + getmaxx() / 2, getmaxy() / 2 - yp[6], xp[7] + getmaxx() / 2, getmaxy() / 2 - yp[7]);
    line(xp[7] + getmaxx() / 2, getmaxy() / 2 - yp[7], xp[4] + getmaxx() / 2, getmaxy() / 2 - yp[4]);

    // Connecting edges
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
    setbkcolor(BLACK);
    cleardevice();

    // Draw repere (coordinate axes)
    drawRepere();

    // Cube properties: position (centered), size, and oblique projection parameters
    int x = -50, y = -50, z = 50, size = 100;
    float alpha = M_PI / 4; // 45 degrees (angle for skewing)
    int scale = 2;          // Scale factor for skew

    // Draw the skewed cube
    drawCubeOblique(x, y, z, size, alpha, scale);

    // Wait for user input and close graphics
    getch();
    closegraph();

    return 0;
}
