#include <graphics.h>
#include <math.h>
#include <iostream>
using namespace std;

// Define cube vertices
float cube[8][3] = {
    {0, 0, 0},     // A
    {100, 0, 0},   // B
    {100, 50, 0},  // C
    {0, 50, 0},    // D
    {0, 0, 20},    // E
    {100, 0, 20},  // F
    {100, 50, 20}, // G
    {0, 50, 20},   // H
};

// Rotation function
void rotateCube(float angleX, float angleY)
{
    for (int i = 0; i < 8; i++)
    {
        // Rotate around Y-axis
        float x = cube[i][0];
        float z = cube[i][2];
        cube[i][0] = x * cos(angleY) - z * sin(angleY);
        cube[i][2] = x * sin(angleY) + z * cos(angleY);

        // Rotate around X-axis
        float y = cube[i][1];
        z = cube[i][2];
        cube[i][1] = y * cos(angleX) - z * sin(angleX);
        cube[i][2] = y * sin(angleX) + z * cos(angleX);
    }
}

// Draw 3D axes
void drawAxes(int centerX, int centerY)
{
    setcolor(RED);
    line(centerX, centerY, centerX + 150, centerY); // X-axis
    outtextxy(centerX + 155, centerY, (char *)"X");

    setcolor(GREEN);
    line(centerX, centerY, centerX, centerY - 150); // Y-axis
    outtextxy(centerX, centerY - 155, (char *)"Y");

    setcolor(BLUE);
    line(centerX, centerY, centerX + 100, centerY + 100); // Z-axis (diagonal)
    outtextxy(centerX + 105, centerY + 105, (char *)"Z");
}

// Draw cube
void drawCube(int centerX, int centerY)
{
    // Project 3D points to 2D
    int projected[8][2];
    for (int i = 0; i < 8; i++)
    {
        projected[i][0] = centerX + cube[i][0];
        projected[i][1] = centerY - cube[i][1]; // Invert Y-axis for proper graphics alignment
    }

    // Define cube edges
    int edges[12][2] = {
        {0, 1}, {1, 2}, {2, 3}, {3, 0}, // Front face
        {4, 5},
        {5, 6},
        {6, 7},
        {7, 4}, // Back face
        {0, 4},
        {1, 5},
        {2, 6},
        {3, 7} // Connections
    };

    setcolor(WHITE);
    for (int i = 0; i < 12; i++)
    {
        line(
            projected[edges[i][0]][0],
            projected[edges[i][0]][1],
            projected[edges[i][1]][0],
            projected[edges[i][1]][1]);
    }
}

// Main program
int main()
{
    int gd = DETECT, gm;
    initgraph(&gd, &gm, (char *)"");

    float angleX = 0, angleY = 0;
    int centerX = getmaxx() / 2, centerY = getmaxy() / 2;

    while (!kbhit())
    {
        cleardevice();
        drawAxes(centerX, centerY);
        rotateCube(angleX, angleY);
        drawCube(centerX, centerY);
        angleX += 0.02;
        angleY += 0.03;
        delay(500);
    }

    closegraph();
    return 0;
}
