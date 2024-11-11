#include <stdio.h>
#include <graphics.h>
#include <time.h>
#include <math.h>

void init_graph()
{
    int g = DETECT, m;
    initgraph(&g, &m, (char *)"");
    setbkcolor(RED);
}

void close_graph()
{
    getch();
    closegraph();
}

void repere()
{
    cleardevice();
    setbkcolor(9);
    setcolor(5);
    line(0, 240, 640, 240);
    line(320, 0, 320, 480);
}

// draw
void tracer(int xd, int yd)
{
    xd = xd + 320;
    yd = 240 - yd;
    putpixel(xd, yd, WHITE);
}

// draw circle with bresnham algorithm
void draw_circle(int h, int k, int r)
{
    int s, x, y;

    s = 3 - 2 * r;
    x = 0;
    y = r;

    while (x < y)
    {
        if (s > 0)
        {
            s = s + 4 * (x - y) + 10;
            y--;
        }
        else
        {
            s = s + 4 * x + 6;
        }
        x++;

        tracer(x + h, y + k);
        tracer(-x + h, -y + k);
        tracer(-x + h, y + k);
        tracer(x + h, -y + k);

        tracer(y + h, x + k);
        tracer(-y + h, -x + k);
        tracer(-y + h, x + k);
        tracer(y + h, -x + k);
    }
}

// draw line with bresnham algorithm
void draw_line(int x1, int y1, int x2, int y2)
{
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (1)
    {
        tracer(x1, y1);

        if (x1 == x2 && y1 == y2)
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