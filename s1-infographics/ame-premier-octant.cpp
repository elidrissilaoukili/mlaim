#include <stdio.h>
#include <conio.h>
#include <graphics.h>
#include <math.h>

void init_graph()
{
    int g = DETECT, m;
    initgraph(&g, &m, NULL);
}

void repere()
{
    cleardevice();
    setbkcolor(9);
    setcolor(5);
    line(0, 240, 640, 240);
    line(320, 0, 320, 480);
}

void pixel(int x, int y, int color)
{
    x = x + 320;
    y = 240 - y;
    putpixel(x, y, color);
}

void first_octant(int xd, int yd, int xf, int yf)
{
    int dx, dy, x, y, s;
    x = xd;
    y = yd;

    dx = xf - xd;
    dy = yf - yd;

    s = 2 * dy - dx;

    int i = 1;

    while (i <= dx)
    {
        if (s > 0)
        {
            x = x + 1;
            y = y + 1;
            s = s + 2 * (dy - dx);
            pixel(x, y, 5);
        }
        else
        {
            x = x + 1;
            s = s + 2 * dy;
            pixel(x, y, 15);
        }
        i++;
    }
}

int main()
{
    int xd, yd, xf, yf;
    xd = 100;
    yd = 100;
    xf = 300;
    yf = 200;

    init_graph();
    repere();

    first_octant(xd, yd, xf, yf);

    getch();
    closegraph();

    return 0;
}