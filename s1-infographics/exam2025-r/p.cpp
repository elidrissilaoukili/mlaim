#include <graphics.h>
#include <stdio.h>
#include <math.h>

// Function to apply the transformation matrix
void applyTransformation(int *x, int *y, int *z, float matrix[4][4]) {
    float newX = matrix[0][0] * (*x) + matrix[0][1] * (*y) + matrix[0][2] * (*z) + matrix[0][3];
    float newY = matrix[1][0] * (*x) + matrix[1][1] * (*y) + matrix[1][2] * (*z) + matrix[1][3];
    float newZ = matrix[2][0] * (*x) + matrix[2][1] * (*y) + matrix[2][2] * (*z) + matrix[2][3];

    *x = (int)newX;
    *y = (int)newY;
    *z = (int)newZ;
}

// Function to convert 3D points to 2D for visualization
void projectPoint(int x, int y, int z, int &px, int &py, int centerX, int centerY) {
    px = centerX + x;
    py = centerY - y;  // Inverted Y-axis for correct drawing
}

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

// Function to draw the edges of the pyramid
void drawPyramid(int vertices[4][3], int centerX, int centerY) {
    int px[4], py[4];

    // Project all 3D points to 2D
    for (int i = 0; i < 4; i++) {
        projectPoint(vertices[i][0], vertices[i][1], vertices[i][2], px[i], py[i], centerX, centerY);
    }

    // Draw edges of the pyramid
    setcolor(WHITE);
    line(px[0], py[0], px[1], py[1]); // A-B
    line(px[0], py[0], px[2], py[2]); // A-C
    line(px[0], py[0], px[3], py[3]); // A-D
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
    
    drawAxes(centerX, centerY);

    // Define the original pyramid vertices
    int vertices[4][3] = {
        {0, 0, 0},    // A (0, 0, 0)
        {100, 0, 0},  // B (100, 0, 0)
        {0, 100, 0},  // C (0, 100, 0)
        {0, 0, 100}   // D (0, 0, 100)
    };

    // Draw the original pyramid
    drawPyramid(vertices, centerX, centerY);
    delay(2000); // Pause for 2 seconds before transformation

    // Define the transformation matrix (Symmetry about the plane [A, D, B])
    float T[4][4] = {
        {-1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, -1, 0},
        {0, 0, 0, 1}
    };

    // Apply transformation to each vertex
    for (int i = 0; i < 4; i++) {
        applyTransformation(&vertices[i][0], &vertices[i][1], &vertices[i][2], T);
    }

    cleardevice(); 
    drawPyramid(vertices, centerX, centerY);
    drawAxes(centerX, centerY);

    getch();  // Wait for key press before closing
    closegraph();
    return 0;
}
