#include "Point.cpp"

void tracer(int xd, int yd)
{
    xd = xd + 320;
    yd = 240 - yd;
    putpixel(xd, yd, WHITE);
}

void draw(Point p)
{
    tracer(p.x, p.y);
}

Point rotation(Point C, double teta)
{
    int x_rot, y_rot;

    x_rot = C.x * cos(teta) - C.y * sin(teta);
    y_rot = C.x * sin(teta) + C.y * cos(teta);

    return {x_rot, y_rot};
}

void ellipse_poly(Point C, int a, int b, double teta = 0)
{
    int x, y;
    Point p;

    for (double i = 0; i < 2 * M_PI; i = i + 0.03)
    {
        x = a * cos(i * M_PI);
        y = b * sin(i * M_PI);

        p = Point(x, y);

        p = rotation(p, teta);

        tracer(p.x + C.x, p.y + C.y);

        x++;
    }
}

int main()
{
    int g = DETECT, m;
    initgraph(&g, &m, NULL);

    Point Ce, O = {0, 0};
    Ce = {0, 0};
    int a = 100;
    int b = 50;
    double teta = double(M_PI / 4);
    for (int i = 0; i < 8; i++, teta += M_PI / 4)
    {
        Ce = rotation({0, 150}, teta);
        draw(Ce);
        ellipse_poly(Ce, a, b, teta);
    }

    getch();
    closegraph();

    return 0;
}