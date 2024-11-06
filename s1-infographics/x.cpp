#include <graphics.h>
#include <math.h>
#include <conio.h>

void drawHeart(int xCenter, int yCenter, int size, int color)
{
    float angle;
    int x, y;

    for (angle = 0; angle < M_PI * 2; angle += 0.01)
    {
        x = xCenter + (int)(size * 16 * pow(sin(angle), 3));
        y = yCenter - (int)(size * (13 * cos(angle) - 5 * cos(2 * angle) - 2 * cos(3 * angle) - cos(4 * angle)));
        putpixel(x, y, color);
    }
}

int main()
{
    int g = DETECT, m;
    initgraph(&g, &m, (char *)"");

    int xCenter = 320; // X coordinate for center of heart
    int yCenter = 240; // Y coordinate for center of heart
    int size = 10;     // Size multiplier for the heart

    setbkcolor(RED);
    cleardevice();

    drawHeart(xCenter, yCenter, size, WHITE);

    getch();
    closegraph();
    return 0;
}
