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

void clock_frame(int x_center, int y_center, int radius)
{
    setcolor(WHITE);
    setlinestyle(0, 0, 2);
    draw_circle(x_center, y_center, radius);
    clock_segments(x_center, y_center, radius);
    clock_small_segments(x_center, y_center, radius);
}

void my_clock(int x_center, int y_center, int radius)
{
    while (!kbhit())
    {
        cleardevice();
        time_t t = time(NULL);
        struct tm current_time = *localtime(&t);

        int angle = 90;
        int hour_angle = angle - current_time.tm_hour % 12 * 30 - (current_time.tm_min);
        int minute_angle = angle - current_time.tm_min * 6;
        int second_angle = angle - current_time.tm_sec * 6;

        int hourHandLength = radius * 0.5;
        int minuteHandLength = radius * 0.8;
        int secondHandLength = radius * 0.9;

        int hour_xf = (x_center + hourHandLength * cos(hour_angle * (M_PI / 180)));
        int hour_yf = (y_center + hourHandLength * sin(hour_angle * (M_PI / 180)));

        int minute_xf = (x_center + minuteHandLength * cos(minute_angle * (M_PI / 180)));
        int minute_yf = (y_center + minuteHandLength * sin(minute_angle * (M_PI / 180)));

        int second_xf = (x_center + secondHandLength * cos(second_angle * (M_PI / 180)));
        int second_yf = (y_center + secondHandLength * sin(second_angle * (M_PI / 180)));

        setcolor(YELLOW);
        draw_line(x_center, y_center, hour_xf, hour_yf);
        setcolor(RED);
        draw_line(x_center, y_center, minute_xf, minute_yf);
        setcolor(BLUE);
        draw_line(x_center, y_center, second_xf, second_yf);

        clock_frame(x_center, y_center, radius);
        delay(1000);
    }
}