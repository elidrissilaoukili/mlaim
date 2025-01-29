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
    *z = -tempX * sin
    
    (rad) + (*z) * cos(rad);
}

// Function to draw a line between two points in 3D (with 2D projection)
void drawLine3D(float x1, float y1, float z1, float x2, float y2, float z2)
{
    // Simple perspective projection
    int scale = 100;
    int x1_2D = (int)(x1 * scale + 200);
    int y1_2D = (int)(-y1 * scale + 200);
    int x2_2D = (int)(x2 * scale + 200);
    int y2_2D = (int)(-y2 * scale + 200);
    line(x1_2D, y1_2D, x2_2D, y2_2D);
}

// Function to draw a 3D cube
void drawCube(float vertices[8][3])
{
    int edges[12][2] = {
        {0, 1}, {1, 2}, {2, 3}, {3, 0}, // Bottom face
        {4, 5},
        {5, 6},
        {6, 7},
        {7, 4}, // Top face
        {0, 4},
        {1, 5},
        {2, 6},
        {3, 7} // Vertical edges
    };

    for (int i = 0; i < 12; i++)
    {
        int v1 = edges[i][0];
        int v2 = edges[i][1];
        drawLine3D(vertices[v1][0], vertices[v1][1], vertices[v1][2],
                   vertices[v2][0], vertices[v2][1], vertices[v2][2]);
    }
}

int main()
{
    // Initialize graphics
    int g = DETECT, m;
    initgraph(&g, &m, (char *)"");

    // Define the vertices of a cube
    float cube[8][3] = {
        {0, 0, 0}, {1, 0, 0}, {1, 1, 0}, {0, 1, 0}, // Bottom face
        {0, 0, 1},
        {1, 0, 1},
        {1, 1, 1},
        {0, 1, 1} // Top face
    };

    float angleX = 0, angleY = 0; // Rotation angles

    while (!kbhit())
    { // Loop until a key is pressed
        // Clear the screen
        cleardevice();

        // Rotated cube
        float rotatedCube[8][3];
        for (int i = 0; i < 8; i++)
        {
            rotatedCube[i][0] = cube[i][0];
            rotatedCube[i][1] = cube[i][1];
            rotatedCube[i][2] = cube[i][2];
            rotateX(&rotatedCube[i][1], &rotatedCube[i][2], angleX);
            rotateY(&rotatedCube[i][0], &rotatedCube[i][2], angleY);
        }

        // Draw the rotated cube
        setcolor(RED);
        drawCube(rotatedCube);

        // Update rotation angles
        angleX += 1; // Increment rotation around X-axis
        angleY += 1; // Increment rotation around Y-axis

        // Delay for smooth animation
        delay(30);
    }

    closegraph();
    return 0;
}
