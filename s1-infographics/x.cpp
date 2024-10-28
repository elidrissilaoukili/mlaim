#include <stdio.h>
#include <graphics.h> // For graphical functions

void drawLine(int x0, int y0, int x1, int y1) {
    int dx = x1 - x0;
    int dy = y1 - y0;

    int p = 2 * dy - dx; // Initial decision parameter
    int twoDy = 2 * dy;
    int twoDyDx = 2 * (dy - dx);
    
    int x = x0;
    int y = y0;

    putpixel(x, y, WHITE); // Draw the initial point

    while (x < x1) {
        x++;
        if (p < 0) {
            p += twoDy;
        } else {
            y++;
            p += twoDyDx;
        }
        putpixel(x, y, WHITE); // Draw the point on the screen
    }
}

int main() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, NULL); // Initialize the graphics system

    int x0 = -500, y0 = -900; // Starting point
    int x1 = 800, y1 = 300; // Ending point

    drawLine(x0, y0, x1, y1); // Draw a line in the first octant

    getch(); // Wait for a key press
    closegraph(); // Close the graphics window
    return 0;
}
