#include <graphics.h>
#include <math.h>
#include <iostream>
using namespace std;

// Define cube vertices
float cube[8][3] = {
    {0, 0, 0},       // A
    {200, 0, 0},     // B
    {200, 100, 0},   // C
    {0, 100, 0},     // D
    {0, 0, 100},     // E
    {200, 0, 100},   // F
    {200, 100, 100}, // G
    {0, 100, 100},   // H
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

void projectCavalier(float x, float y, float z, int &x2d, int &y2d)
{
    x2d = x + z * cos(45 * M_PI / 180);
    y2d = y + z * sin(45 * M_PI / 180);
}

// Draw cube
void drawCube(int centerX, int centerY)
{
    // Project 3D points to 2D
    int projected[8][2];
    for (int i = 0; i < 8; i++)
    {
        projected[i][0] = centerX + cube[i][0];
        projected[i][1] = centerY + cube[i][1];
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

    // Draw edges
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

    while (!kbhit())
    {
        cleardevice();
        rotateCube(angleX, angleY);
        drawCube(getmaxx() / 2, getmaxy() / 2);
        angleX += 0.02; // Rotate around X-axis
        angleY += 0.03; // Rotate around Y-axis
        delay(500);
    }

    closegraph();
    return 0;
}
