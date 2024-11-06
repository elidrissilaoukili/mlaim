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
    putpixel(xd, yd, WHITE);
}

void cyrcle(int h, int k, int r)
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

int main()
{
    int g = DETECT, m;
    initgraph(&g, &m, NULL);

    int h = 100, k = 100;   // center
    int r = 100;            // radius

    repere();
    cyrcle(h, k, r);

    getch();
    closegraph();

    return 0;
}