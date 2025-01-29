#include <graphics.h> // Graphics library
#include <math.h>     // Math library
#include <stdio.h>    // Standard I/O library

#define SCREEN_CENTER_X getmaxx() / 2
#define SCREEN_CENTER_Y getmaxy() / 2

// Normalize a 3D vector
void normalizeVector(int v[3], float result[3])
{
    float magnitude = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    if (magnitude == 0)
        magnitude = 1; // Avoid division by zero
    result[0] = v[0] / magnitude;
    result[1] = v[1] / magnitude;
    result[2] = v[2] / magnitude;
}

// Build the transformation matrix T
void buildTransformationMatrix(float a, float b, float c, float t[4][4])
{
    float A = sqrt(a * a + b * b + c * c);
    float B = sqrt(b * b + c * c);

    if (A == 0)
        A = 1; // Prevent division by zero
    if (B == 0)
        B = 1;

    t[0][0] = B / A;
    t[0][1] = 0;
    t[0][2] = a / A;
    t[0][3] = 0;
    t[1][0] = -(a * b) / (A * B);
    t[1][1] = c / B;
    t[1][2] = b / A;
    t[1][3] = 0;
    t[2][0] = -(c * a) / (A * B);
    t[2][1] = -b / B;
    t[2][2] = c / A;
    t[2][3] = 0;
    t[3][0] = 0;
    t[3][1] = 0;
    t[3][2] = 0;
    t[3][3] = 1;
}

// Build the inverse of the transformation matrix T
void buildInverseMatrix(float a, float b, float c, float t[4][4])
{
    float A = sqrt(a * a + b * b + c * c);
    float B = sqrt(b * b + c * c);

    if (A == 0)
        A = 1; // Prevent division by zero
    if (B == 0)
        B = 1;

    t[0][0] = B / A;
    t[0][1] = -(a * b) / (A * B);
    t[0][2] = -(c * a) / (A * B);
    t[0][3] = 0;
    t[1][0] = 0;
    t[1][1] = c / B;
    t[1][2] = -b / B;
    t[1][3] = 0;
    t[2][0] = a / A;
    t[2][1] = b / A;
    t[2][2] = c / A;
    t[2][3] = 0;
    t[3][0] = 0;
    t[3][1] = 0;
    t[3][2] = 0;
    t[3][3] = 1;
}

// Multiply two 4x4 matrices
void multiplyMatrices(float m1[4][4], float m2[4][4], float result[4][4])
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            result[i][j] = 0;
            for (int k = 0; k < 4; k++)
                result[i][j] += m1[i][k] * m2[k][j];
        }
    }
}

// Multiply a vector by a matrix
void multiplyVector(float v[1][4], float matrix[4][4], float result[1][4])
{
    for (int j = 0; j < 4; j++)
    {
        result[0][j] = 0;
        for (int k = 0; k < 4; k++)
            result[0][j] += v[0][k] * matrix[k][j];
    }
}

// Draw a line with color from (x1, y1) to (x2, y2)
void drawLineWithColor(float x1, float y1, float x2, float y2, int color)
{
    setcolor(color);
    line(x1, y1, x2, y2);
}


// Main display function
void displayLines()
{
    int v[3] = {100, 200, 30};
    int n[3] = {-50, 150, 75};

    float T_v[4][4], T_n[4][4], T_combined[4][4];
    float v_vect[1][4] = {{100, 200, 30, 1}};
    float n_vect[1][4] = {{-50, 150, 75, 1}};
    float v_result[1][4] = {0};

    float v_normalized[3], n_normalized[3];
    normalizeVector(v, v_normalized);
    normalizeVector(n, n_normalized);

    buildTransformationMatrix(v_normalized[0], v_normalized[1], v_normalized[2], T_v);
    buildInverseMatrix(n_normalized[0], n_normalized[1], n_normalized[2], T_n);
    multiplyMatrices(T_v, T_n, T_combined);
    multiplyVector(v_vect, T_combined, v_result);

    // Draw vectors
    drawLineWithColor(SCREEN_CENTER_X, SCREEN_CENTER_Y, SCREEN_CENTER_X + v[0], SCREEN_CENTER_Y - v[1], RED);                                // Vector v
    drawLineWithColor(SCREEN_CENTER_X, SCREEN_CENTER_Y, SCREEN_CENTER_X + n[0], SCREEN_CENTER_Y - n[1], BLUE);                               // Vector n
    drawLineWithColor(SCREEN_CENTER_X, SCREEN_CENTER_Y, SCREEN_CENTER_X + v_result[0][0] / 2, SCREEN_CENTER_Y - v_result[0][1] / 2, YELLOW); // Resultant vector
}
