#include <stdio.h>
#include <graphics.h>
#include <time.h>
#include <math.h>

void drawarro(int xcenter, int ycenter, int radius);
void init_graph();
void drawClockFrame(int x_center, int y_center, int Rayon);
void horloge(int x_center, int y_center, int Rayon);

int main()
{
    init_graph();
    horloge(320, 240, 200);
    return 0;
}

void drawarro(int xcenter, int ycenter, int radius)
{
    for (int i = 0; i < 12; i++)
    {
        float angle = i * M_PI / 6;

        int x1, x2, y1, y2;
        x1 = xcenter + (int)(radius * cos(angle));
        y1 = ycenter - (int)(radius * sin(angle));
        x2 = xcenter + (int)((radius - 10) * cos(angle));
        y2 = ycenter - (int)((radius - 10) * sin(angle));

        line(x1, y1, x2, y2);
    }
}

void init_graph()
{
    int g = DETECT, m;
    initgraph(&g, &m, (char *)"");
    setbkcolor(RED);
}

void drawClockFrame(int x_center, int y_center, int Rayon)
{
    setcolor(WHITE);
    setlinestyle(0, 0, 2);
    circle(x_center, y_center, Rayon);
    drawarro(x_center, y_center, Rayon);
}

void horloge(int x_center, int y_center, int Rayon)
{
    while (!kbhit())
    {
        cleardevice();
        time_t t = time(NULL);
        struct tm currentTime = *localtime(&t);

        int angle = 90;
        int hourAngle = angle - currentTime.tm_hour % 12 * 30 - (currentTime.tm_min / 2);
        int minuteAngle = angle - currentTime.tm_min * 6;
        int secondAngle = angle - currentTime.tm_sec * 6;

        int hourHandLength = Rayon * 0.5;
        int minuteHandLength = Rayon * 0.8;
        int secondHandLength = Rayon * 0.9;

        int hourX = (int)(x_center + hourHandLength * cos(hourAngle * (M_PI / 180)));
        int hourY = (int)(y_center - hourHandLength * sin(hourAngle * (M_PI / 180)));
        int minuteX = (int)(x_center + minuteHandLength * cos(minuteAngle * (M_PI / 180)));
        int minuteY = (int)(y_center - minuteHandLength * sin(minuteAngle * (M_PI / 180)));
        int secondX = (int)(x_center + secondHandLength * cos(secondAngle * (M_PI / 180)));
        int secondY = (int)(y_center - secondHandLength * sin(secondAngle * (M_PI / 180)));

        setcolor(YELLOW);
        line(x_center, y_center, hourX, hourY);
        setcolor(WHITE);
        line(x_center, y_center, minuteX, minuteY);
        setcolor(LIGHTBLUE);
        line(x_center, y_center, secondX, secondY);

        drawClockFrame(x_center, y_center, Rayon);
        delay(1000);
    }
}