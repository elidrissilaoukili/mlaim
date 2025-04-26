#include <graphics.h>
#include <conio.h>
#include <math.h>

// Function to simulate 3D to 2D projection
void project3DTo2D(int x, int y, int z, int *x2d, int *y2d) {
    // Simple perspective projection
    int focalLength = 500;
    *x2d = (int)(x * focalLength / (z + focalLength));
    *y2d = (int)(y * focalLength / (z + focalLength));
}

void drawPyramid() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, (char *)""); // Initialize graphics mode
    
    // Vertices of the pyramid in 3D
    int vertices[4][3] = {
        {0, 0, 0},    // A (0, 0, 0)
        {100, 0, 0},  // B (100, 0, 0)
        {0, 100, 0},  // C (0, 100, 0)
        {0, 0, 100}   // D (0, 0, 100)
    };
    
    int x2d[4], y2d[4];
    
    // Project 3D points to 2D
    for (int i = 0; i < 4; i++) {
        project3DTo2D(vertices[i][0], vertices[i][1], vertices[i][2], &x2d[i], &y2d[i]);
    }
    
    // Drawing the edges of the pyramid
    // Face ABD
    line(x2d[0] + 300, y2d[0] + 240, x2d[1] + 300, y2d[1] + 240); // AB
    line(x2d[0] + 300, y2d[0] + 240, x2d[3] + 300, y2d[3] + 240); // AD
    line(x2d[1] + 300, y2d[1] + 240, x2d[3] + 300, y2d[3] + 240); // BD
    
    // Face ACD
    line(x2d[0] + 300, y2d[0] + 240, x2d[2] + 300, y2d[2] + 240); // AC
    line(x2d[0] + 300, y2d[0] + 240, x2d[3] + 300, y2d[3] + 240); // AD
    line(x2d[2] + 300, y2d[2] + 240, x2d[3] + 300, y2d[3] + 240); // CD
    
    // Base ABC
    line(x2d[0] + 300, y2d[0] + 240, x2d[1] + 300, y2d[1] + 240); // AB
    line(x2d[1] + 300, y2d[1] + 240, x2d[2] + 300, y2d[2] + 240); // BC
    line(x2d[2] + 300, y2d[2] + 240, x2d[0] + 300, y2d[0] + 240); // CA
    
    // Face BCD
    line(x2d[1] + 300, y2d[1] + 240, x2d[2] + 300, y2d[2] + 240); // BC
    line(x2d[2] + 300, y2d[2] + 240, x2d[3] + 300, y2d[3] + 240); // CD
    line(x2d[1] + 300, y2d[1] + 240, x2d[3] + 300, y2d[3] + 240); // BD
    
    getch();
    closegraph();
}

int main() {
    drawPyramid();
    return 0;
}
