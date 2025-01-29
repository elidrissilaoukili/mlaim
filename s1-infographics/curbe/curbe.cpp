#include <graphics.h>
#include <cmath>
#include <iostream>

// Function to calculate a point on the Bézier curve
void bezierCurve(int x[], int y[], int n) {
    for (float t = 0; t <= 1; t += 0.001) {  // t goes from 0 to 1 in small steps
        float xt = 0, yt = 0;

        // Calculate Bézier curve using the Bernstein polynomial
        for (int i = 0; i < n; i++) {
            float coefficient = pow(1 - t, n - 1 - i) * pow(t, i) * tgamma(n) / (tgamma(i + 1) * tgamma(n - i));
            xt += coefficient * x[i];
            yt += coefficient * y[i];
        }

        putpixel(round(xt), round(yt), WHITE); // Draw pixel at calculated point
    }
}

int main() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, (char *)"");

    // Define control points (adjust coordinates as needed)
    int x[] = {100, 200, 300};  // x-coordinates
    int y[] = {200, 50, 200};   // y-coordinates
    int n = sizeof(x) / sizeof(x[0]);

    // Draw the Bézier curve
    bezierCurve(x, y, n);

    getch();  // Wait for a key press
    closegraph();
    return 0;
}
