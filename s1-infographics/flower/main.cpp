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

void flower(Point C, int r, int a, int b, int petal_number)
{
    double teta = 0;

    Point Ce, O = {0, 0};
    Ce = {r + a, 0};

    for (int i = 0; i <= petal_number; i++)
    {
        ellipse_poly(Ce, a, b, teta);
        teta = i * 2 * M_PI / petal_number;
        Ce = rotation({r + a, 0}, teta);
    }

    for (int i = 0; i < r; i++)
    {
        setbkcolor(GREEN);
        circle(320, 240, i);
    }
}

int main()
{
    int g = DETECT, m;
    initgraph(&g, &m, NULL);

    flower({0, 50}, 20, 40, 20, 8);
    // flower({150, 150}, 20);

    getch();
    closegraph();

    return 0;
}