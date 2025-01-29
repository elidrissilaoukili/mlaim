#include <graphics.h>
#include <math.h>
#include <time.h>
#include <conio.h>
#include <stdio.h>
#include <algorithm>
#define pi 3.142857142

void initialisation_graph()
{
    int g = DETECT, m;
    initgraph(&g, &m, (char *)"");
    initwindow(1200, 900, (char *)"projection");
}

void tracer2(int x, int y, int color)
{
    x = x + getmaxx() / 2;
    y = getmaxy() / 2 - y;
    putpixel(x, y, color);
}

void polynomiale_ligne2(int xd, int yd, int xf, int yf)
{

    int x, y;
    int dx = xf - xd;
    int dy = yf - yd;
    float a, b;

    int px = (xd > xf) ? -1 : 1;
    int py = (yd > yf) ? -1 : 1;

    tracer2(xd, yd, 100);
    tracer2(xf, yf, 100);

    int dx1 = abs(dx);
    int dy1 = abs(dy);

    if (dx1 > dy1)
    {
        a = (float)dy / dx;
        b = yd - xd * a;
        for (x = xd + px; x != xf; x = x + px)
        {
            y = a * x + b;
            tracer2(x, y, WHITE);
        }
    }
    else
    {
        a = (float)dx / dy;
        b = xd - yd * a;
        for (y = yd + py; y != yf; y = y + py)
        {
            x = a * y + b;
            tracer2(x, y, WHITE);
        }
    }
}
void repere()
{
    cleardevice();
    setcolor(WHITE);
    int centerX = getmaxx() / 2;
    int centerY = getmaxy() / 2;
    int centerZ = 0;
    polynomiale_ligne2(0, 0, 550, 0);
    outtextxy(centerX + 500, centerY, (char *)"X");

    polynomiale_ligne2(0, 0, 0, 450);
    outtextxy(centerX, 70, (char *)"Y");

    polynomiale_ligne2(0, 0, -550, -450);
    outtextxy(centerX - 400, centerY + 200, (char *)"Z");
}

void Produit_Matricielle_8x4(double firstMatrix[8][4], double transformation[4][4], double result[8][4])
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            result[i][j] = 0;
            for (int k = 0; k < 4; k++)
            {
                result[i][j] += firstMatrix[i][k] * transformation[k][j];
            }
        }
    }
}

// Dessin du cube
void cube1(double cube[8][4], double k, double h, double f, double angle)
{

    double T[4][4] = {{1, 0, 0, 0}, {0, 1, 0, 0}, {f * cos(angle), f * sin(angle), 0, 0}, {0, 0, 0, 1}};
    double C[8][4];
    Produit_Matricielle_8x4(cube, T, C);

    outtextxy(C[0][0] + k, h - C[0][1], (char *)"A");
    outtextxy(C[1][0] + k, h - C[1][1], (char *)"B");
    outtextxy(C[2][0] + k, h - C[2][1], (char *)"C");
    outtextxy(C[3][0] + k, h - C[3][1], (char *)"D");
    outtextxy(C[4][0] + k, h - C[4][1], (char *)"H");
    outtextxy(C[5][0] + k, h - C[5][1], (char *)"E");
    outtextxy(C[6][0] + k, h - C[6][1], (char *)"F");
    outtextxy(C[7][0] + k, h - C[7][1], (char *)"G");

    line(C[0][0] + k, h - C[0][1], C[1][0] + k, h - C[1][1]);
    line(C[1][0] + k, h - C[1][1], C[2][0] + k, h - C[2][1]);
    line(C[2][0] + k, h - C[2][1], C[3][0] + k, h - C[3][1]);
    line(C[3][0] + k, h - C[3][1], C[0][0] + k, h - C[0][1]);
    line(C[5][0] + k, h - C[5][1], C[6][0] + k, h - C[6][1]);
    line(C[6][0] + k, h - C[6][1], C[7][0] + k, h - C[7][1]);
    line(C[7][0] + k, h - C[7][1], C[4][0] + k, h - C[4][1]);
    line(C[4][0] + k, h - C[4][1], C[5][0] + k, h - C[5][1]);
    line(C[2][0] + k, h - C[2][1], C[7][0] + k, h - C[7][1]);
    line(C[3][0] + k, h - C[3][1], C[4][0] + k, h - C[4][1]);
    line(C[1][0] + k, h - C[1][1], C[6][0] + k, h - C[6][1]);
    line(C[0][0] + k, h - C[0][1], C[5][0] + k, h - C[5][1]);
}

/////////////////////////////////////translation////////////////////////////////////////////////////////////////////////////
void translation_cube(double cube[8][4], double tx, double ty, double tz)
{

    double T[4][4] = {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {tx, ty, tz, 1}};

    double C[8][4];

    Produit_Matricielle_8x4(cube, T, C);

    cube1(C, getmaxx() / 2, getmaxy() / 2, 0.5, pi / 6);
}
////////////////////////////////////rotation//////////////////////////////////////////////////////////////////////////////////

void rotation_x_cube(double cube[8][4], double angle)
{
    double Rx[4][4] = {
        {1, 0, 0, 0},
        {0, cos(angle), sin(angle), 0},
        {0, -sin(angle), cos(angle), 0},
        {0, 0, 0, 1}};

    double C[8][4];
    Produit_Matricielle_8x4(cube, Rx, C);
    cube1(C, getmaxx() / 2, getmaxy() / 2, 0.5, pi / 6);
}

void rotation_y_cube(double cube[8][4], double angle)
{
    double Ry[4][4] = {
        {cos(angle), 0, -sin(angle), 0},
        {0, 1, 0, 0},
        {sin(angle), 0, cos(angle), 0},
        {0, 0, 0, 1}};

    double C[8][4];
    Produit_Matricielle_8x4(cube, Ry, C);
    cube1(C, getmaxx() / 2, getmaxy() / 2, 0.5, pi / 6);
}

void rotation_z_cube(double cube[8][4], double angle)
{
    double Rz[4][4] = {
        {cos(angle), sin(angle), 0, 0},
        {-sin(angle), cos(angle), 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}};
    double C[8][4];
    Produit_Matricielle_8x4(cube, Rz, C);
    cube1(C, getmaxx() / 2, getmaxy() / 2, 0.5, pi / 6);
}
////////////////////////////////////symetrie////////////////////////////////////////////////////////////////////////////::
void symetrie_xy_cube(double cube[8][4])
{
    double S[4][4] = {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, -1, 0},
        {0, 0, 0, 1}};

    double C[8][4];
    Produit_Matricielle_8x4(cube, S, C);

    cube1(C, getmaxx() / 2, getmaxy() / 2, 0.5, pi / 6);
}

void symetrie_yz_cube(double cube[8][4])
{
    double S[4][4] = {
        {-1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}};

    double C[8][4];
    Produit_Matricielle_8x4(cube, S, C);

    cube1(C, getmaxx() / 2, getmaxy() / 2, 0.5, pi / 6);
}

void symetrie_xz_cube(double cube[8][4])
{
    double S[4][4] = {
        {1, 0, 0, 0},
        {0, -1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}};

    double C[8][4];
    Produit_Matricielle_8x4(cube, S, C);

    cube1(C, getmaxx() / 2, getmaxy() / 2, 0.5, pi / 6);
}
///////////////////////////////:://projection perspective////////////////////////////////////////////////////////////////
void perspective_projection_cube(double cube[8][4], double d)
{

    double P[4][4] = {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 1 / d},
        {0, 0, 0, 0}};

    double C[8][4];
    Produit_Matricielle_8x4(cube, P, C);

    for (int i = 0; i < 8; i++)
    {
        if (C[i][3] != 0)
        {
            C[i][0] = C[i][0] / C[i][3];
            C[i][1] = C[i][1] / C[i][3];
            C[i][2] = C[i][2] / C[i][3];
            C[i][3] = 1;
        }
    }

    cube1(C, getmaxx() / 2, getmaxy() / 2, 0.5, pi / 6);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////:
void alignerVecteurAvecZ(double cube[8][4], double a, double b, double c)
{
    double A = sqrt(a * a + b * b + c * c);
    double B = sqrt(b * b + c * c);

    double T[4][4] = {
        {B / A, 0, a / A, 0},
        {-a * b / (A * B), c / B, b / A, 0},
        {-c * a / (A * B), -b / B, c / A, 0},
        {0, 0, 0, 1}};

    double C[8][4];
    Produit_Matricielle_8x4(cube, T, C);
    cube1(C, getmaxx() / 2, getmaxy() / 2, 0.5, pi / 6);
}
void alignerVecteurAvecZ_Transpose(double cube[8][4], double a, double b, double c)
{

    double A = sqrt(a * a + b * b + c * c);
    double B = sqrt(b * b + c * c);

    double T[4][4] = {
        {B / A, -a * b / (A * B), -c * a / (A * B), 0},
        {0, c / B, -b / B, 0},
        {a / A, b / A, c / A, 0},
        {0, 0, 0, 1}};

    double C[8][4];
    Produit_Matricielle_8x4(cube, T, C);
    cube1(C, getmaxx() / 2, getmaxy() / 2, 0.5, pi / 6);
}
///////////////////////////////////////////vecteur avec vecteur /////////////////////////////////////////////////////////::
void tracerVecteur(double x, double y, double z, int couleur)
{
    // Transformation des coordonn�es 3D en coordonn�es 2D
    int x2d = x * 50 + getmaxx() / 2; // Mise � l'�chelle pour visibilit�
    int y2d = getmaxy() / 2 - y * 50; // Inverser y pour correspondre au rep�re

    // Origine au centre de l'�cran
    int origine_x = getmaxx() / 2;
    int origine_y = getmaxy() / 2;

    setcolor(couleur);
    setlinestyle(SOLID_LINE, 0, 3);

    line(origine_x, origine_y, x2d, y2d);

    double longueur = sqrt(x * x + y * y + z * z);
    double angle = atan2(y, x);
    int fleche_x1 = x2d - 10 * cos(angle + pi / 6);
    int fleche_y1 = y2d - 10 * sin(angle + pi / 6);
    int fleche_x2 = x2d - 10 * cos(angle - pi / 6);
    int fleche_y2 = y2d - 10 * sin(angle - pi / 6);

    line(x2d, y2d, fleche_x1, fleche_y1);
    line(x2d, y2d, fleche_x2, fleche_y2);
}
void alignerVVersN(double cube[8][4], double vx, double vy, double vz, double nx, double ny, double nz)
{
    double Av = sqrt(vx * vx + vy * vy + vz * vz);
    double Bv = sqrt(vy * vy + vz * vz);
    tracerVecteur(vx, vy, vz, WHITE);
    tracerVecteur(nx, ny, nz, YELLOW);
    cube1(cube, getmaxx() / 2, getmaxy() / 2, 0.5, pi / 6);
    double Tv[4][4] = {
        {Bv / Av, 0, vx / Av, 0},
        {-vx * vy / (Av * Bv), vz / Bv, vy / Av, 0},
        {-vz * vx / (Av * Bv), -vy / Bv, vz / Av, 0},
        {0, 0, 0, 1}};
    double C[8][4];
    Produit_Matricielle_8x4(cube, Tv, C);
    cube1(C, getmaxx() / 2, getmaxy() / 2, 0.5, pi / 6);

    double An = sqrt(nx * nx + ny * ny + nz * nz);
    double Bn = sqrt(ny * ny + nz * nz);

    double Tn_transpose[4][4] = {
        {Bn / An, -nx * ny / (An * Bn), -nz * nx / (An * Bn), 0},
        {0, nz / Bn, -ny / Bn, 0},
        {nx / An, ny / An, nz / An, 0},
        {0, 0, 0, 1}};

    double D[8][4];
    Produit_Matricielle_8x4(C, Tn_transpose, D);

    cube1(D, getmaxx() / 2, getmaxy() / 2, 0.5, pi / 6);
}
////////////////////////////////////main //////////////////////////////////////////////////////////////////////////////////

int main()
{
    initialisation_graph();
    repere();
    double k = getmaxy() / 2;
    double h = getmaxx() / 2;

    double A[8][4] = {
        {0, 0, 0, 1},
        {200, 0, 0, 1},
        {200, 200, 0, 1},
        {0, 200, 0, 1},
        {0, 200, -200, 1},
        {0, 0, -200, 1},
        {200, 0, -200, 1},
        {200, 200, -200, 1}};
    while (1)
    {

        rotation_x_cube(A, pi / 2);
    }
    // translation_cube(A, 100, 100 , 100);
    // symetrie_xz_cube(A);
    // perspective_projection_cube(A,500);
    // alignerVecteurAvecZ(A, 3, 4, 5);
    // alignerVecteurAvecZ_Transpose(A, 3, 4, 5);

    alignerVVersN(A, 1, 2, 3, 3, 2, 1);

    getch();
    closegraph();
}
