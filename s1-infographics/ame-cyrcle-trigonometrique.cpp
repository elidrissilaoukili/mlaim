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
    putpixel(xd, yd, 11);
}

void cyrcle_trigono(int h, int k, int r)
{
    int x, y;
    float a = 0;

    while (a <= (M_PI / 4))
    {
        x = r * cos(a);
        y = r * sin(a);

        tracer(x + h, y + k);
        tracer(-x + h, y + k);
        tracer(x + h, -y + k);
        tracer(-x + h, -y + k);
        tracer(y + h, x + k);
        tracer(-y + h, x + k);
        tracer(y + h, -x + k);
        tracer(-y + h, -x + k);

        a += M_PI / 50;
    }
}

int main()
{
    int h = 0, k = 0;
    int r = 100;

    init_graph();
    repere();

    cyrcle_trigono(h, k, r);

    getch();
    closegraph();

    return 0;
}