#include "../lib.h"

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
