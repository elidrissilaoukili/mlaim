#include <graphics.h>
#include <math.h>
#include <iostream>
using namespace std;

// Define cube vertices
float cube[8][3] = {
    {-50, -50, -50}, // 0: Front-top-left
    {50, -50, -50},  // 1: Front-top-right
    {50, 50, -50},   // 2: Front-bottom-right
    {-50, 50, -50},  // 3: Front-bottom-left
    {-50, -50, 50},  // 4: Back-top-left
    {50, -50, 50},   // 5: Back-top-right
    {50, 50, 50},    // 6: Back-bottom-right
    {-50, 50, 50}    // 7: Back-bottom-left
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
        {4, 5}, {5, 6}, {6, 7}, {7, 4}, // Back face
        {0, 4},{1, 5},{2, 6},{3, 7} // Connections
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

    // Fill faces (optional)
    setfillstyle(SOLID_FILL, RED);
    floodfill((projected[0][0] + projected[1][0]) / 2, (projected[0][1] + projected[1][1]) / 2, WHITE);
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
