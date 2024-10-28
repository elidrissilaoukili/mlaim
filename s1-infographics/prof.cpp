#include <stdio.h>
#include <conio.h>
#include <math.h>
#include <graphics.h>

void init_graph()
{
    int g = DETECT, m;
    initgraph(&g, &m, const_cast<char*>("C:\\TC\\BGI"));  // Cast to avoid the warning
}

void repere()
{
    cleardevice();
    setbkcolor(9);
    setcolor(5);
    line(0, 240, 640, 240); // X-axis
    line(320, 0, 320, 480); // Y-axis
}

void pixel(int x, int y, int color)
{
    x = x + 320; // Center the x-coordinate
    y = 240 - y; // Invert the y-coordinate
    putpixel(x, y, color);
}

// Generalized Bresenham's Algorithm
void bresenham(int xd, int yd, int xf, int yf)
{
    int dx = abs(xf - xd);
    int dy = abs(yf - yd);

    int sx = (xf >= xd) ? 1 : -1; // Step direction for x
    int sy = (yf >= yd) ? 1 : -1; // Step direction for y

    int err = dx - dy;
    int e2;

    while (xd != xf || yd != yf)
    {
        pixel(xd, yd, 15); // Draw the pixel (15 = white color)

        e2 = 2 * err;
        if (e2 > -dy)
        {
            err -= dy;
            xd += sx;
        }
        if (e2 < dx)
        {
            err += dx;
            yd += sy;
        }
    }
}

int main()
{
    int xd, yd, xf, yf;

    printf("Enter xd, yd: \n");
    scanf("%d %d", &xd, &yd);

    printf("Enter xf, yf: \n");
    scanf("%d %d", &xf, &yf);

    init_graph();
    repere();
    bresenham(xd, yd, xf, yf); // Corrected function name
    getch(); // Wait for a key press to close the graphics window
    closegraph(); // Close the graphics mode
}
