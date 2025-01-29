#include <graphics.h> // Inclure la bibliothèque pour les fonctions graphiques
#include <math.h>     // Bibliothèque mathématique
#include <stdio.h>    // Bibliothèque d'entrée/sortie (optionnelle)

// Fonction pour normaliser un vecteur 3D
void normalize(int v[3], float result[3])
{
    float magnitude = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    if (magnitude == 0)
        magnitude = 1;
    result[0] = v[0] / magnitude;
    result[1] = v[1] / magnitude;
    result[2] = v[2] / magnitude;
}

// Fonction pour construire la matrice de transformation T
void T(float a, float b, float c, float t[4][4])
{
    float A = sqrt((a * a) + (b * b) + (c * c));
    float B = sqrt((b * b) + (c * c));

    if (A == 0)
        A = 1;
    if (B == 0)
        B = 1;

    t[0][0] = B / A;
    t[0][1] = 0;
    t[0][2] = a / A;
    t[0][3] = 0;
    t[1][0] = -(a * b) / (A * B);
    t[1][1] = c / B;
    t[1][2] = b / A;
    t[1][3] = 0;
    t[2][0] = -(c * a) / (A * B);
    t[2][1] = -b / B;
    t[2][2] = c / A;
    t[2][3] = 0;
    t[3][0] = 0;
    t[3][1] = 0;
    t[3][2] = 0;
    t[3][3] = 1;
}

// Fonction pour calculer l'inverse de la matrice T
void T_inverse(float a, float b, float c, float t[4][4])
{
    float A = sqrt((a * a) + (b * b) + (c * c));
    float B = sqrt((b * b) + (c * c));

    if (A == 0)
        A = 1;
    if (B == 0)
        B = 1;

    t[0][0] = B / A;
    t[0][1] = -(a * b) / (A * B);
    t[0][2] = -(c * a) / (A * B);
    t[0][3] = 0;
    t[1][0] = 0;
    t[1][1] = c / B;
    t[1][2] = -b / B;
    t[1][3] = 0;
    t[2][0] = a / A;
    t[2][1] = b / A;
    t[2][2] = c / A;
    t[2][3] = 0;
    t[3][0] = 0;
    t[3][1] = 0;
    t[3][2] = 0;
    t[3][3] = 1;
}

// Multiplie deux matrices 4x4
void multiply(float T[4][4], float T_inv[4][4], float result[4][4])
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            result[i][j] = 0;
            for (int k = 0; k < 4; k++)
                result[i][j] += T[i][k] * T_inv[k][j];
        }
    }
}

// Multiplie un vecteur par une matrice
void vect(float v[1][4], float result[4][4], float vect_res[1][4])
{
    for (int j = 0; j < 4; j++)
    {
        vect_res[0][j] = 0;
        for (int k = 0; k < 4; k++)
            vect_res[0][j] += v[0][k] * result[k][j];
    }
}

// Dessine une ligne entre deux points
void drawLine(float x1, float y1, float x2, float y2, int color)
{
    setcolor(color);
    line(x1, y1, x2, y2);
}

// Fonction de rendu principal
void display()
{
    // Tracer les axes
    drawLine(0, getmaxy() / 2, getmaxx(), getmaxy() / 2, WHITE);
    drawLine(getmaxx() / 2, 0, getmaxx() / 2, getmaxy(), WHITE);

    int v[3] = {100, 200, 30};
    int n[3] = {-50, 150, 75};

    float T_v[4][4], T_n[4][4], T_combined[4][4];
    float v_vect[1][4] = {{100, 200, 30, 1}};
    float n_vect[1][4] = {{-50, 150, 75, 1}};
    float v_result[1][4] = {0};

    float v_normalized[3], n_normalized[3];
    normalize(v, v_normalized);
    normalize(n, n_normalized);

    T(v_normalized[0], v_normalized[1], v_normalized[2], T_v);
    T_inverse(n_normalized[0], n_normalized[1], n_normalized[2], T_n);
    multiply(T_v, T_n, T_combined);
    vect(v_vect, T_combined, v_result);

    drawLine(320, 240, 320 + v[0], 240 - v[1], RED);
    drawLine(320, 240, 320 + n[0], 240 - n[1], BLUE);
    drawLine(320, 240, 320 + v_result[0][0] / 2, 240 - v_result[0][1] / 2, YELLOW);
}

int main()
{
    int gd = DETECT, gm;
    initgraph(&gd, &gm, NULL); // Initialiser la fenêtre graphique

    display(); // Appeler la fonction d'affichage

    getch();      // Attendre une touche pour fermer la fenêtre
    closegraph(); // Fermer la fenêtre graphique
    return 0;
}