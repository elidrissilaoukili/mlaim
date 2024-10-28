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
    setbkcolor(15);
    setcolor(9);
    line(0, 240, 640, 240);
    line(320, 0, 320, 480);
}

void tracer(int xd, int yd)
{
    xd = xd + 320;
    yd = 240 - yd;
    putpixel(xd, yd, 5);
}

void polynomial(int xd, int yd, int xf, int yf)
{
    int x, y, dx, dy, px, py;
    float a, b;

    dx = xf - xd;
    dy = yf - yd;

    px = (xd > xf) ? -1 : 1;
    py = (yd > yf) ? -1 : 1;

    tracer(xd, yd);
    tracer(xf, yf);

    if (abs(dx) >= abs(dy))
    {
        a = (float)dy / dx;
        b = yd - xd * a;

        for (x = xd + px; x != xf; x = x + px)
        {
            y = a * x + b;
            tracer(x, y);
        }
    }
    else
    {
        a = (float)dx / dy;
        b = xd - yd * a;
        for (y = yd + py; y != yf; y = y + py)
        {
            x = a * y + b;
            tracer(x, y);
        }
    }
}

int main()
{
    int xd, yd, xf, yf;
    xd = 300;
    yd = 200;
    xf = 100;
    yf = 100;

    init_graph();
    repere();

    polynomial(xd, yd, xf, yf);

    getch();
    closegraph();

    return 0;
}