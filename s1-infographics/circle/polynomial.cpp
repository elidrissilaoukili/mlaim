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
    line(0, 240, 640, 240); // Draw x-axis
    line(320, 0, 320, 480); // Draw y-axis
}

void tracer(int xd, int yd)
{
    xd = xd + 320; // Adjust for screen center
    yd = 240 - yd;
    putpixel(xd, yd, WHITE);
}

void poly(int h, int k, int r)
{
    int x = 0, y;

    while (x <= r / sqrt(2))
    {
        y = sqrt(r * r - x * x);
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
    init_graph();
    repere();

    int h = 100, k = 100; // Circle center
    int r = 100;          // Circle radius
    poly(h, k, r);        // Call the fixed circle function

    getch();
    closegraph();

    return 0;
}
