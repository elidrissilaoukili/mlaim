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

// Function to project 3D to 2D
void drawLine3D(float x1, float y1, float z1, float x2, float y2, float z2)
{
    int scale = 100;
    int x1_2D = (int)(x1 * scale + 300);
    int y1_2D = (int)(-y1 * scale + 300);
    int x2_2D = (int)(x2 * scale + 300);
    int y2_2D = (int)(-y2 * scale + 300);
    line(x1_2D, y1_2D, x2_2D, y2_2D);
}

// Function to draw the 3D reference frame (axes)
void drawAxes(int centerX, int centerY)
{
    setcolor(RED);
    line(centerX, centerY, centerX + 150, centerY); // X-axis (Red)
    outtextxy(centerX + 155, centerY, (char *)"X");

    setcolor(GREEN);
    line(centerX, centerY, centerX, centerY - 150); // Y-axis (Green)
    outtextxy(centerX, centerY - 155, (char *)"Y");

    setcolor(BLUE);
    line(centerX, centerY, centerX - 100, centerY + 100); // Z-axis (Blue)
    outtextxy(centerX - 110, centerY + 120, (char *)"Z");
}

// Function to draw the pyramid
void drawPyramid(float vertices[4][3])
{
    int edges[9][2] = {
        {0, 1}, {0, 2}, {0, 3}, // Base connections
        {1, 3}, {1, 2}, {2, 3}, // Triangle sides
        {1, 2}, {1, 3}, {2, 3} // Additional edges (BC, BD, CD)
    };
    for (int i = 0; i < 9; i++)
    {
        int v1 = edges[i][0];
        int v2 = edges[i][1];
        drawLine3D(vertices[v1][0], vertices[v1][1], vertices[v1][2],
                   vertices[v2][0], vertices[v2][1], vertices[v2][2]);
    }
}

// Function to apply transformation matrix
void applyTransformation(float vertices[4][3], float matrix[4][4])
{
    for (int i = 0; i < 4; i++)
    {
        float x = vertices[i][0];
        float y = vertices[i][1];
        float z = vertices[i][2];

        vertices[i][0] = matrix[0][0] * x + matrix[0][1] * y + matrix[0][2] * z + matrix[0][3];
        vertices[i][1] = matrix[1][0] * x + matrix[1][1] * y + matrix[1][2] * z + matrix[1][3];
        vertices[i][2] = matrix[2][0] * x + matrix[2][1] * y + matrix[2][2] * z + matrix[2][3];
    }
}

int main()
{
    int g = DETECT, m;
    initgraph(&g, &m, (char *)"");

    // Define pyramid vertices (A, B, C, D)
    float pyramid[4][3] = {
        {0, 0, 0}, // A
        {0, 1, 0}, // B (Y-axis)
        {1, 0, 0}, // C (X-axis)
        {0, 0, 1}  // D (Z-axis)
    };

    // Apply rotations to adjust orientation
    for (int i = 0; i < 4; i++)
    {
        rotateX(&pyramid[i][1], &pyramid[i][2], 90);  // Rotate to align AD with Z-axis
        rotateY(&pyramid[i][0], &pyramid[i][2], -45); // Align AC with X-axis
    }

    int centerX = 300, centerY = 300;
    drawAxes(centerX, centerY);
    setcolor(WHITE);
    drawPyramid(pyramid);
    delay(2000);

    // Transformation matrix: Reflection in XY plane
    float T[4][4] = {
        {-1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, -1, 0},
        {0, 0, 0, 1}};

    applyTransformation(pyramid, T);
    cleardevice();
    drawAxes(centerX, centerY);
    setcolor(YELLOW);
    drawPyramid(pyramid);

    getch();
    closegraph();
    return 0;
}
