#include <stdio.h>
#include <cmath>
#include <graphics.h>

void tracer(int xd, int yd)
{
    xd = xd + 320;
    yd = 240 - yd;
    putpixel(xd, yd, WHITE);
}

void reper()
{
    setcolor(GREEN);
    line(0, 240, 640, 240);
    line(320, 0, 320, 480);
}

void drawEllipse(int h, int k, int a, int b)
{
    int x = 0, y;

    while (x <= a)
    {
        y = b * sqrt(1 - (pow(x, 2) / pow(a, 2)));

        tracer(x + h, y + k);
        tracer(-x + h, y + k);
        tracer(-x + h, -y + k);
        tracer(x + h, -y + k);

        tracer(x + h, y + k);
        tracer(-x + h, y + k);
        tracer(-x + h, -y + k);
        tracer(x + h, -y + k);

        x++;
    }
}

int main()
{
    int g = DETECT, m;
    initgraph(&g, &m, NULL);
    reper();

    int h = 100, k = 100; // Circle center
    int a = 50;
    int b = 100;
    drawEllipse(h, k, a, b);

    getch();
    closegraph();

    return 0;
}
