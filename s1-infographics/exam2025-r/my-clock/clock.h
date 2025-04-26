#include "functions.h"

void clock_segments(int x_center, int y_center, int radius)
{
    for (int i = 0; i < 12; i++)
    {
        float angle = i * M_PI / 6;

        int xd, xf, yd, yf;
        xd = x_center + (radius * cos(angle));
        yd = y_center - (radius * sin(angle));

        xf = x_center + ((radius - 20) * cos(angle));
        yf = y_center - ((radius - 20) * sin(angle));

        draw_line(xd, yd, xf, yf);
    }
}

void clock_small_segments(int x_center, int y_center, int radius)
{
    for (int i = 0; i < 60; i++)
    {
        float angle = i * M_PI / 30;

        int xd, xf, yd, yf;
        xd = x_center + (radius * cos(angle));
        yd = y_center - (radius * sin(angle));

        xf = x_center + ((radius - 5) * cos(angle));
        yf = y_center - ((radius - 5) * sin(angle));

        draw_line(xd, yd, xf, yf);
    }
}

void draw_numbers(int x_center, int y_center, int radius)
{
    for (int i = 1; i <= 12; i++)
    {
        float angle = -i * M_PI / 6 + M_PI / 2;

        int x_text = (x_center + (radius - 30) * cos(angle)) + 320;
        int y_text = 240 - (y_center + (radius - 30) * sin(angle));

        char number[3];
        sprintf(number, "%d", i);

        outtextxy(x_text, y_text, number);
    }
}

void clock_frame(int x_center, int y_center, int radius)
{
    setcolor(WHITE);
    setlinestyle(0, 0, 2);
    draw_circle(x_center, y_center, radius);
    clock_segments(x_center, y_center, radius);
    clock_small_segments(x_center, y_center, radius);
    draw_numbers(x_center, y_center, radius);
}

void my_clock(int x_center, int y_center, int radius)
{
    clock_frame(x_center, y_center, radius);
}
