#include "../lib.h"

void cyrcle_trigono(int h, int k, int r)
{
    int x, y;
    float a = 0;

    while (a <= (M_PI / 4))
    {
        x = r * cos(a);
        y = r * sin(a);

        tracer(x + h, y + k);
        tracer(-x + h, -y + k);
        tracer(-x + h, y + k);
        tracer(x + h, -y + k);

        tracer(y + h, x + k);
        tracer(-y + h, -x + k);
        tracer(-y + h, x + k);
        tracer(y + h, -x + k);

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

    close_graph();

    return 0;
}