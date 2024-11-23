#include <iostream>
#include <stdio.h>
#include <cmath>
#include <graphics.h>

void tracer(int xd, int yd)
{
    xd = xd + 320;
    yd = 240 - yd;
    putpixel(xd, yd, WHITE);
}

void draw(int x, int y)
{
    tracer(x, y);
}

void rotation(int x, int y, double teta, int &x_rot, int &y_rot)
{
    x_rot = x * cos(teta) - y * sin(teta);
    y_rot = x * sin(teta) + y * cos(teta);
}

void ellipse_poly(int cx, int cy, int a, int b, double teta)
{
    int x, y, x_rot, y_rot;

    for (double i = 0; i < 2 * M_PI; i += 0.03)
    {
        x = a * cos(i * M_PI);
        y = b * sin(i * M_PI);

        rotation(x, y, teta, x_rot, y_rot);

        tracer(x_rot + cx, y_rot + cy);
    }
}

void flower(int cx, int cy, int r, int a, int b, int petal_number, int color_c, int color_petal)
{
    double teta;
    int ce_x, ce_y, rotated_x, rotated_y;

    ce_x = r + a;
    ce_y = 0;

    for (int i = 0; i <= petal_number; i++)
    {
        teta = i * 2 * M_PI / petal_number;
        rotation(r + a, 0, teta, rotated_x, rotated_y);
        ce_x = rotated_x;
        ce_y = rotated_y;
        ellipse_poly(cx + ce_x, cy + ce_y, a, b, teta);
    }

    for (int i = 0; i < r; i++)
    {
        circle(320, 190, i);
    }
}

int main()
{
    int g = DETECT, m;
    initgraph(&g, &m, NULL);

    flower(0, 50, 20, 40, 20, 8, 9, 5);
    flower(150, 150, 20, 40, 20, 8, 9, 5);

    getch();
    closegraph();

    return 0;
}
