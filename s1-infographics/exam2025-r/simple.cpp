#include <graphics.h>
#include <math.h>
#include <stdio.h>

#define PI 3.14159265

// Function to rotate a point around the X-axis
void rotateX(float *y, float *z, float angle)
{
    float rad = angle * PI / 180.0;
    float tempY = *y;
    *y = tempY * cos(rad) - (*z) * sin(rad);
    *z = tempY * sin(rad) + (*z) * cos(rad);
}

// Function to rotate a point around the Y-axis
void rotateY(float *x, float *z, float angle)
{
    float rad = angle * PI / 180.0;
    float tempX = *x;
    *x = tempX * cos(rad) + (*z) * sin(rad);
    *z = -tempX * sin(rad) + (*z) * cos(rad);
}

// Function to draw a line between two points in 3D (with 2D projection)
void drawLine3D(float x1, float y1, float z1, float x2, float y2, float z2)
{
    // Simple perspective projection
    int scale = 2;
    int x1_2D = (int)(x1 * scale + 200);
    int y1_2D = (int)(-y1 * scale + 200);
    int x2_2D = (int)(x2 * scale + 200);
    int y2_2D = (int)(-y2 * scale + 200);
    line(x1_2D, y1_2D, x2_2D, y2_2D);
}

// Function to apply transformation matrix to a point
void transformPoint(float *x, float *y, float *z, float T[4][4])
{
    float tempX = *x;
    float tempY = *y;
    float tempZ = *z;

    *x = T[0][0] * tempX + T[0][1] * tempY + T[0][2] * tempZ + T[0][3];
    *y = T[1][0] * tempX + T[1][1] * tempY + T[1][2] * tempZ + T[1][3];
    *z = T[2][0] * tempX + T[2][1] * tempY + T[2][2] * tempZ + T[2][3];
}

// Function to draw a 3D pyramid
void drawPyramid(float vertices[4][3], float T[4][4])
{
    int edges[6][2] = {
        {0, 1}, {1, 2}, {2, 0}, // Base
        {0, 3}, {1, 3}, {2, 3}  // Sides
    };

    for (int i = 0; i < 6; i++)
    {
        int v1 = edges[i][0];
        int v2 = edges[i][1];

        float x1 = vertices[v1][0];
        float y1 = vertices[v1][1];
        float z1 = vertices[v1][2];

        float x2 = vertices[v2][0];
        float y2 = vertices[v2][1];
        float z2 = vertices[v2][2];

        // Apply transformation matrix
        transformPoint(&x1, &y1, &z1, T);
        transformPoint(&x2, &y2, &z2, T);

        drawLine3D(x1, y1, z1, x2, y2, z2);
    }
}

// Function to draw the coordinate axes
void drawAxes()
{
    // X-axis
    setcolor(RED);
    drawLine3D(0, 0, 0, 100, 0, 0);

    // Y-axis
    setcolor(GREEN);
    drawLine3D(0, 0, 0, 0, 100, 0);

    // Z-axis
    setcolor(BLUE);
    drawLine3D(0, 0, 0, 0, 0, 100);
}

int main()
{
    // Initialize graphics
    int g = DETECT, m;
    initgraph(&g, &m, (char *)"");

    // Define the vertices of the pyramid
    float vertices[4][3] = {
        {0, 0, 0},    // A (0,0,0) at center
        {0, 0, 100},  // B (0,0,100) along Z-axis
        {100, 0, 0},  // C (100,0,0) along X-axis
        {0, 100, 0}   // D (0,100,0) along Y-axis
    };

    // Define the transformation matrix
    float T[4][4] = {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, -1, 0},
        {0, 0, 0, 1}
    };

    float angleX = 0, angleY = 0; // Rotation angles

    while (!kbhit())
    { // Loop until a key is pressed
        // Clear the screen
        cleardevice();

        // Draw the coordinate axes
        drawAxes();

        // Rotated pyramid
        float rotatedPyramid[4][3];
        for (int i = 0; i < 4; i++)
        {
            rotatedPyramid[i][0] = vertices[i][0];
            rotatedPyramid[i][1] = vertices[i][1];
            rotatedPyramid[i][2] = vertices[i][2];
            rotateX(&rotatedPyramid[i][1], &rotatedPyramid[i][2], angleX);
            rotateY(&rotatedPyramid[i][0], &rotatedPyramid[i][2], angleY);
        }

        // Draw the rotated pyramid with transformation
        setcolor(WHITE);
        drawPyramid(rotatedPyramid, T);

        // Update rotation angles
        angleX += 1; // Increment rotation around X-axis
        angleY += 1; // Increment rotation around Y-axis

        // Delay for smooth animation
        delay(30);
    }

    closegraph();
    return 0;
}