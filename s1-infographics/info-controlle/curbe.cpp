#include "./lib.h"

float cube[8][3] = {
    {0, 0, 0},       // A
    {200, 0, 0},     // B   this one
    {200, 100, 0},   // C
    {0, 100, 0},     // D   this one
    {0, 0, 100},     // E
    {200, 0, 100},   // F   this one
    {200, 100, 100}, // G   this one
    {0, 100, 100},   // H
};

void projectCabinet(float x, float y, float z, int &x2d, int &y2d)
{
    x2d = x + z * cos(100 * M_PI / 180) / 2;
    y2d = y + z * sin(100 * M_PI / 180) / 2;
}

void bezierCurve(int x[], int y[], int n)
{
    for (float t = 0; t <= 1; t += 0.001)
    {
        float xt = 0, yt = 0;

        // Calculate Bézier curve using the Bernstein polynomial
        for (int i = 0; i < n; i++)
        {
            float coefficient = pow(1 - t, n - 1 - i) * pow(t, i) * tgamma(n) / (tgamma(i + 1) * tgamma(n - i));
            xt += coefficient * x[i];
            yt += coefficient * y[i];
        }

        putpixel(round(xt), round(yt), WHITE);
    }
}

int main()
{
    init_graph();

    // {200, 0, 0}     // B   this one
    // {0, 100, 0}     // D   this one
    // {200, 0, 100}   // F   this one
    // {200, 100, 100} // G   this one

    int x[] = {200, 0, 200, 200};
    int y[] = {0, 100, 0, 100};
    int n = sizeof(x) / sizeof(x[0]);

    bezierCurve(x, y, n);

    close_graph();
    return 0;
}
