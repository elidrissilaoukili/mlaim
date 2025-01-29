#include <graphics.h>
#include <math.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

// Function to draw the 3D repere (axes)
void drawAxes()
{
    int centerX = SCREEN_WIDTH / 2;
    int centerY = SCREEN_HEIGHT / 2;

    // Draw X-axis (red)
    setcolor(RED);
    line(centerX - 300, centerY, centerX + 300, centerY);
    outtextxy(centerX + 310, centerY, (char *)"X");

    // Draw Y-axis (green)
    setcolor(GREEN);
    line(centerX, centerY - 300, centerX, centerY + 300);
    outtextxy(centerX, centerY - 310, (char *)"Y");

    // Draw Z-axis (blue)
    setcolor(BLUE);
    line(centerX, centerY, centerX - 200, centerY + 200);
    outtextxy(centerX - 210, centerY + 210, (char *)"Z");
}

// Function to draw a cube using a specific projection
void drawCube(int x[8], int y[8], int z[8], int projectionType)
{
    int projectedX[8], projectedY[8];
    float d = 300; // Distance for perspective projection

    int centerX = SCREEN_WIDTH / 2;  // Center of the screen (X-axis)
    int centerY = SCREEN_HEIGHT / 2; // Center of the screen (Y-axis)

    // Project each point based on the selected projection type
    for (int i = 0; i < 8; i++)
    {
        switch (projectionType)
        {
        case 1: // Orthogonal (XY Plane)
            projectedX[i] = centerX + x[i];
            projectedY[i] = centerY - y[i];
            break;
        case 2: // Orthogonal (XZ Plane - Top View)
            projectedX[i] = centerX + x[i];
            projectedY[i] = centerY - z[i];
            break;
        case 3: // Isometric
            projectedX[i] = centerX + (int)(x[i] * cos(M_PI / 4) - z[i] * sin(M_PI / 4));
            projectedY[i] = centerY - (int)(x[i] * sin(M_PI / 4) / sqrt(2) + y[i] * sqrt(2) / 2 - z[i] * sin(M_PI / 4) / sqrt(2));
            break;
        case 4: // Perspective
            projectedX[i] = centerX + (int)(x[i] / (1 + z[i] / d));
            projectedY[i] = centerY - (int)(y[i] / (1 + z[i] / d));
            break;
        }
    }

    // Connect the points to form the cube
    for (int i = 0; i < 4; i++)
    {
        line(projectedX[i], projectedY[i], projectedX[(i + 1) % 4], projectedY[(i + 1) % 4]);                 // Bottom face
        line(projectedX[i + 4], projectedY[i + 4], projectedX[(i + 1) % 4 + 4], projectedY[(i + 1) % 4 + 4]); // Top face
        line(projectedX[i], projectedY[i], projectedX[i + 4], projectedY[i + 4]);                             // Connect bottom to top
    }
    line(projectedX[0], projectedY[0], projectedX[4], projectedY[4]); // Edge from (0) to (4)
}

// Function for Shape 1: Orthogonal projection (XY plane)
void drawShape1()
{
    cleardevice(); // Clear the screen
    drawAxes();

    int x[8] = {-50, 50, 50, -50, -50, 50, 50, -50};
    int y[8] = {-50, -50, 50, 50, -50, -50, 50, 50};
    int z[8] = {50, 50, 50, 50, -50, -50, -50, -50};

    drawCube(x, y, z, 1);
    outtextxy(20, 20, (char *)"Shape 1: Orthogonal Projection (XY Plane)");
}

// Function for Shape 2: Orthogonal projection (XZ plane)
void drawShape2()
{
    cleardevice(); // Clear the screen
    drawAxes();

    int x[8] = {-50, 50, 50, -50, -50, 50, 50, -50};
    int y[8] = {-50, -50, 50, 50, -50, -50, 50, 50};
    int z[8] = {50, 50, 50, 50, -50, -50, -50, -50};

    drawCube(x, y, z, 2);
    outtextxy(20, 20, (char *)"Shape 2: Orthogonal Projection (XZ Plane)");
}

// Function for Shape 3: Isometric projection
void drawShape3()
{
    cleardevice(); // Clear the screen
    drawAxes();

    int x[8] = {-50, 50, 50, -50, -50, 50, 50, -50};
    int y[8] = {-50, -50, 50, 50, -50, -50, 50, 50};
    int z[8] = {50, 50, 50, 50, -50, -50, -50, -50};

    drawCube(x, y, z, 3);
    outtextxy(20, 20, (char *)"Shape 3: Isometric Projection");
}

// Function for Shape 4: Perspective projection
void drawShape4()
{
    cleardevice(); // Clear the screen
    drawAxes();

    int x[8] = {-50, 50, 50, -50, -50, 50, 50, -50};
    int y[8] = {-50, -50, 50, 50, -50, -50, 50, 50};
    int z[8] = {50, 50, 50, 50, -50, -50, -50, -50};

    drawCube(x, y, z, 4);
    outtextxy(20, 20, (char *)"Shape 4: Perspective Projection");
}

int main()
{
    int gd = DETECT, gm;
    initgraph(&gd, &gm, (char *)"");

    // Set screen dimensions for graphics window
    initwindow(SCREEN_WIDTH, SCREEN_HEIGHT);

    // Draw shapes one by one
    // drawShape1();
    // getch();

    // drawShape2();
    // getch();

    // drawShape3();
    // getch();

    drawShape4();
    getch();

    closegraph();
    return 0;
}
