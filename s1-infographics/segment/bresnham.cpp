#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>

void drawLine(int x1, int y1, int x2, int y2, int color)
{
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (1)
    {
        putpixel(x1, y1, color);  // Plot the pixel at (x1, y1)

        if (x1 == x2 && y1 == y2) // Stop if endpoint is reached
            break;

        int e2 = 2 * err;
        if (e2 > -dy)
        {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx)
        {
            err += dx;
            y1 += sy;
        }
    }
}

void init_graph()
{
    int g = DETECT, m;
    initgraph(&g, &m, NULL);
}

int main()
{
    init_graph();

    drawLine(400, 400, 200, 200, WHITE); // Example line

    getch();
    closegraph();
    return 0;
}
