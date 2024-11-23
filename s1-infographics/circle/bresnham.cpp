#include "../lib.h"

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