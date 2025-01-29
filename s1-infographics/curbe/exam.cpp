#include <graphics.h>
#include <conio.h>
#include <math.h>

// Fonction de Bernstein
float bernstein(int k, int n, float t) {
    // Calcul du coefficient binomial
    float coeff = 1;
    for (int i = 0; i < k; ++i) {
        coeff *= (n - i) / (i + 1.0);
    }
    for (int i = 0; i < n - k; ++i) {
        coeff *= (t - i) / (n - i);
    }
    return coeff * pow(t, k) * pow(1 - t, n - k);
}

// Calcul de la courbe de Bézier
void bezier(int n, int points[][2], int num_points) {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, (char*)"");

    for (float t = 0.0; t <= 1.0; t += 0.01) {
        float x = 0, y = 0;

        // Calcul de la somme des fonctions de Bernstein pour chaque point
        for (int k = 0; k < num_points; ++k) {
            float b = bernstein(k, n, t);
            x += b * points[k][0];
            y += b * points[k][1];
        }

        // Tracer le point de la courbe
        putpixel(x, y, WHITE);
    }

    getch();
    closegraph();
}

int main() {
    // Définition des points de contrôle (E, D, C, G, B)
    int points[5][2] = {
        {100, 100}, // E
        {150, 200}, // D
        {200, 50},  // C
        {250, 300}, // G
        {300, 100}  // B
    };

    // Tracer la courbe de Bézier avec n=4 et les 5 points
    bezier(4, points, 5);

    return 0;
}
