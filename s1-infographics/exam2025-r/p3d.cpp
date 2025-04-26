#include <graphics.h>
#include <stdio.h>
#include <math.h>

// Perspective Projection Constants
const float focalLength = 500;  // Virtual camera distance

// Function to apply 3D perspective projection
void project3D(int x, int y, int z, int &px, int &py, int centerX, int centerY) {
    float depth = focalLength / (focalLength + z);  // Depth scaling (perspective effect)
    px = centerX + x * depth;
    py = centerY - y * depth;  // Inverted Y-axis for correct positioning
}

// Function to draw 3D coordinate axes
void drawAxes(int centerX, int centerY) {
    setcolor(RED);
    line(centerX, centerY, centerX + 150, centerY); // X-axis (Red)
    outtextxy(centerX + 155, centerY, (char *)"X");

    setcolor(GREEN);
    line(centerX, centerY, centerX, centerY - 150); // Y-axis (Green)
    outtextxy(centerX, centerY - 155, (char *)"Y");

    setcolor(BLUE);
    line(centerX, centerY, centerX - 100, centerY + 100); // Z-axis (Blue)
    outtextxy(centerX - 110, centerY + 105, (char *)"Z");
}

// Function to draw the pyramid
void drawPyramid3D(int vertices[4][3], int centerX, int centerY) {
    int px[4], py[4];

    // Project all 3D points to 2D
    for (int i = 0; i < 4; i++) {
        project3D(vertices[i][0], vertices[i][1], vertices[i][2], px[i], py[i], centerX, centerY);
    }

    // Draw pyramid edges
    setcolor(WHITE);
    line(px[0], py[0], px[1], py[1]); // A-B (Z-axis)
    line(px[0], py[0], px[2], py[2]); // A-C (X-axis)
    line(px[0], py[0], px[3], py[3]); // A-D (Y-axis)
    line(px[1], py[1], px[2], py[2]); // B-C  
    line(px[1], py[1], px[3], py[3]); // B-D  
    line(px[2], py[2], px[3], py[3]); // C-D  
}

int main() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, (char*)"");

    int screenWidth = getmaxx();
    int screenHeight = getmaxy();

    int centerX = screenWidth / 2;
    int centerY = screenHeight / 2;

    // Define the updated pyramid vertices
    int vertices[4][3] = {
        {0, 0, 0},    // A (0,0,0) at center
        {0, 0, 100},  // B (0,0,100) along Z-axis
        {100, 0, 0},  // C (100,0,0) along X-axis
        {0, 100, 0}   // D (0,100,0) along Y-axis
    };

    // Draw coordinate axes
    drawAxes(centerX, centerY);

    // Draw the 3D pyramid
    drawPyramid3D(vertices, centerX, centerY);

    getch();  // Wait for key press before closing
    closegraph();
    return 0;
}
