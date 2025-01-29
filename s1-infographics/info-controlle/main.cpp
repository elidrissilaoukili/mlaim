#include "./lib.h"

float cube[8][3] = {
    {0, 0, 0},       // A
    {200, 0, 0},     // B   this one
    {200, 100, 0},   // C
    {0, 100, 0},     // D   this one
    {0, 0, 100},     // E
    {200, 0, 100},   // F   this one
    {200, 100, 100}, // G   
    {0, 100, 100},   // H   this one
};

void projectCavalier(float x, float y, float z, int &x2d, int &y2d)
{
    x2d = x + z * cos(45 * M_PI / 180);
    y2d = y + z * sin(45 * M_PI / 180);
}

void drawCube(int x, int y, int z, int size, int n1, int n2, int n3, int D)
{
    int vertices[8][3] = {
        {x, y, z},                // A
        {x + size, y, z},         // B
        {x + size, y + D, z},     // C
        {x, y + D, z},            // D
        {x, y, z + D},            // E
        {x + size, y, z + D},     // F
        {x + size, y + D, z + D}, // G
        {x, y + D, z + D}};       // H

    int zOffset = size / 32;
    int xp[8], yp[8];
    for (int i = 0; i < 8; i++)
    {
        xp[i] = cube[i][0] + cube[i][2] / zOffset;
        yp[i] = cube[i][1] + cube[i][2] / zOffset;
        projectCavalier(cube[i][0], cube[i][1], cube[i][2], xp[i], yp[i]);
    }

    putlines_v1(xp, yp);
}

void rotateCube(float angleX, float angleY)
{
    for (int i = 0; i < 8; i++)
    {
        float x = cube[i][0];
        float z = cube[i][2];
        cube[i][0] = x * cos(angleY) - z * sin(angleY);
        cube[i][2] = x * sin(angleY) + z * cos(angleY);

        float y = cube[i][1];
        z = cube[i][2];
        cube[i][1] = y * cos(angleX) - z * sin(angleX);
        cube[i][2] = y * sin(angleX) + z * cos(angleX);
    }
}

// void rotateCube(int x, int y, int z, int size, int n1, int n2, int n3, int D, float angleB, float angleH)
// {
//     int cube[8][3] = {
//         {x, y, z},                // A
//         {x + size, y, z},         // B
//         {x + size, y + D, z},     // C
//         {x, y + D, z},            // D
//         {x, y, z + D},            // E
//         {x + size, y, z + D},     // F
//         {x + size, y + D, z + D}, // G
//         {x, y + D, z + D}};       // H

//     for (int i = 0; i < 8; i++)
//     {
//         // Rotate around B-axis
//         cube[i][0] = x * cos(angleH) - z * sin(angleH);
//         cube[i][2] = x * sin(angleH) + z * cos(angleH);

//         // Rotate around H-axis
//         cube[i][1] = y * cos(angleB) - z * sin(angleB);
//         cube[i][2] = y * sin(angleB) + z * cos(angleB);
//     }
// }

void bezierCurve(int x[], int y[], int n)
{
    for (float t = 0; t <= 1; t += 0.001)
    {
        float xt = 0, yt = 0;

        // Calculate Bézier curve using the Bernstein polynomial
        for (int i = 0; i < n; i++)
        {
            float coefficient = pow(1 - t, n - 1 - i) * pow(t, i) * tgamma(n) / (tgamma(i + 1) * tgamma(n - i));
            xt += coefficient * x[i];
            yt += coefficient * y[i];
        }

        int xOffset = getmaxx() / 2;
        int yOffset = getmaxy() / 2;
        putpixel(round(xt + xOffset), round(yOffset - yt), WHITE);
    }
}

int main()
{
    init_graph();
    // drawReperexyz();
    drawReperexy();
    int n1 = 0, n2 = 0, n3 = 1, size = 200;
    int D = 100;

    int x = 0, y = 0, z = 0;

    float angleB = 0, angleH = 0;

    // while (!kbhit())
    // {
    //     cleardevice();
    //     rotateCube(x, y, z, size, n1, n2, n3, D, angleB, angleH);
    //     rotateCube(angleB, angleH);
    //     drawCube(x, y, z, size, n1, n2, n3, D);
    //     angleB += 0.02;
    //     angleH += 0.03;
    //     delay(500);
    // }

    drawCube(x, y, z, size, n1, n2, n3, D);

    // {200, 0, 0},     // B   this one
    // {200, 0, 100},   // F   this one
    // {0, 100, 100},   // H   this one
    // {0, 100, 0},     // D   this one

    int xx[] = {0, 200, 100, 0};
    int yy[] = {0, 100, 200, 0};
    int n = sizeof(xx) / sizeof(xx[0]);
    bezierCurve(xx, yy, n);

    close_graph();

    return 0;
}