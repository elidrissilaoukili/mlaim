#include <stdio.h>
#include <graphics.h>
#include <time.h>
#include <math.h>

void init_graph()
{
    int g = DETECT, m;
    initgraph(&g, &m, (char *)"");
    setbkcolor(RED);
}

void close_graph()
{
    getch();
    closegraph();
}

int main()
{
    init_graph();

    line(100, 100, 200, 200);

    close_graph();
    return 0;
}