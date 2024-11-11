#include "clock.h"

int main()
{
    init_graph();

    int h = 0, k = 0;
    int r = 150;
    my_clock(h, k, r);

    close_graph();

    return 0;
}