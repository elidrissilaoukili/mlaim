#include <stdio.h>
#include <conio.h>
#include <math.h>
#include <graphics.h>

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

void tracer(int xd, int yd)
{
    xd = xd + 320;
    yd = 240 - yd;
    putpixel(xd, yd, 5);
}

void cyrcle(int h, int k, int r)
{
    int s, i, x, y;

    x = 0;
    y = r;
    s = 3 - 2 * r;

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
        tracer(-x + h, y + k);
        tracer(x + h, -y + k);
        tracer(-x + h, -y + k);
        tracer(y + h, x + k);
        tracer(-y + h, x + k);
        tracer(y + h, -x + k);
        tracer(-y + h, -x + k);
    }
}

int main()
{
    int h = 100, k = 100;
    int r = 100;

    init_graph();
    repere();

    cyrcle(h, k, r);

    int hh = -100, kk = -100;
    int rr = 50;
    cyrcle(hh, kk, rr);

    getch();
    closegraph();

    return 0;
}