#include <graphics.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

void init_graph(int width, int height)
{
    initwindow(width, height);
}

void close_graph()
{
    getch();
    closegraph();
}

// Fonction pour afficher les axes 3D
void draw_reper()
{
    int maxx = getmaxx();
    int maxy = getmaxy();

    int centrex = maxx / 2;
    int centrey = maxy / 2;

    int length = 350;

    // Axe Y
    int Yx = centrex + length;
    int Yy = centrey;
    setcolor(3);
    line(centrex, centrey, Yx, Yy);
    char Y[3] = {'Y'};
    outtextxy(Yx - 20, Yy + 5, Y);

    // Axe Z
    int Zx = centrex;
    int Zy = centrey - length;
    setcolor(2);
    line(centrex, centrey, Zx, Zy);
    char Z[3] = {'Z'};
    outtextxy(Zx - 20, Zy + 5, Z);

    // Axe X
    int Xx = centrex - (length * sin(M_PI / 4));
    int Xy = centrey + (length * cos(M_PI / 4));
    setcolor(15);
    line(centrex, centrey, Xx, Xy);
    char X[3] = {'X'};
    outtextxy(Xx + 10, Xy - 5, X);

    // Centre
    char O[3] = {' '};
    outtextxy(centrex + 5, centrey + 10, O);

    int Cx = centrex;
    int Cy = centrey - length;
    char C[27] = {'E', 'L', 'I', 'D', 'R', 'I', 'S', 'S', 'I', ' ', 'L', 'A', 'O', 'U', 'K', 'I', 'L', 'I', ' ', 'M', 'O', 'H', 'A', 'M', 'M', 'E', 'D'};
    setcolor(13);
    outtextxy(Cx - 400, Cy + 5, C);
}

// Fonction pour multiplier deux matrices (10x4 et 4x4)
void Produit_Matrice(double A[10][4], double B[4][4], double resultat[10][4])
{
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            resultat[i][j] = 0;
            for (int k = 0; k < 4; k++)
            {
                resultat[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

// Fonction pour multiplier deux matrices (4x4 et 4x4)
void Produit_Matrice4_4(double A[4][4], double B[4][4], double resultat[4][4])
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            resultat[i][j] = 0;
            for (int k = 0; k < 4; k++)
            {
                resultat[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

// Fonction pour appliquer une translation
void T_I(double points[10][4], double Tx, double Ty, double Tz)
{
    double M_Translation[4][4] = {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {Tx, Ty, Tz, 1}};

    double temp[10][4];
    Produit_Matrice(points, M_Translation, temp);

    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            points[i][j] = temp[i][j];
        }
    }
}

// Fonction pour appliquer une translation
void T_I_(double points[10][4], double Tx, double Ty, double Tz)
{
    double M_Translation[4][4] = {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {-Tx, -Ty, -Tz, 1}};

    double temp[10][4];
    Produit_Matrice(points, M_Translation, temp);

    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            points[i][j] = temp[i][j];
        }
    }
}

// Fonction pour appliquer une rotation autour de l'axe Z
void R_Z(double points[10][4], double angle)
{
    double R_Z[4][4] = {
        {cos(angle), sin(angle), 0, 0},
        {-sin(angle), cos(angle), 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}};

    double temp[10][4];
    Produit_Matrice(points, R_Z, temp);

    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            points[i][j] = temp[i][j];
        }
    }
}

void AfficherPoints(double points[10][4], int color)
{
    int centrex = getmaxx() / 2;
    int centrey = getmaxy() / 2;

    setcolor(color);
    for (int i = 0; i < 10; i++)
    { // puisque J n'est un point de cube
        int x = centrex + (int)points[i][0];
        int y = centrey - (int)points[i][1];

        // Dessiner un petit cercle pour chaque point
        circle(x, y, 3);

        // Afficher le label du point
        char label[3];
        sprintf(label, "%c", 'A' + i);
        outtextxy(x - 10, y - 16, label);
    }
}

void Tracer_cube(double points[10][4], int color)
{
    int centrex = getmaxx() / 2;
    int centrey = getmaxy() / 2;

    // Tracer les ar�tes du cube
    int edges[14][2] = {
        {0, 1}, {1, 2}, {2, 3}, {3, 0}, // Face avant
        {4, 5},
        {5, 6},
        {6, 7},
        {7, 4}, // Face arri�re
        {0, 4},
        {1, 5},
        {2, 6},
        {3, 7}, // Ar�tes de connexion
        {0, 6},
        {8, 9}};

    setcolor(color);
    for (int i = 0; i < 14; i++)
    {
        int p1 = edges[i][0], p2 = edges[i][1];
        int x1 = centrex + (int)points[p1][0];
        int y1 = centrey - (int)points[p1][1];
        int x2 = centrex + (int)points[p2][0];
        int y2 = centrey - (int)points[p2][1];
        line(x1, y1, x2, y2);
    }

    AfficherPoints(points, WHITE);
}

void Projection_Cavalier(double points[10][4], double angle_degre, double f)
{

    double o = angle_degre * (M_PI / 180);
    double M_Cavalier[4][4] = {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {f * cos(o), f * sin(o), 0, 0},
        {0, 0, 0, 1}};

    double cube_proj[10][4];
    Produit_Matrice(points, M_Cavalier, cube_proj);

    Tracer_cube(cube_proj, RED);
}

void Convertir_Points_YZX_vers_XYZ(double points_YZX[10][4], double points_XYZ[10][4], int n)
{
    for (int i = 0; i < n; i++)
    {
        points_XYZ[i][0] = points_YZX[i][2]; // X = ancien X (3e �l�ment)
        points_XYZ[i][1] = points_YZX[i][0]; // Y = ancien Y (1er �l�ment)
        points_XYZ[i][2] = points_YZX[i][1]; // Z = ancien Z (2e �l�ment)
        points_XYZ[i][3] = points_YZX[i][3]; // H reste inchang�
    }
}

void Convertir_Points_XYZ_vers_YZX(double points_XYZ[10][4], double points_YZX[10][4], int n)
{
    for (int i = 0; i < n; i++)
    {
        points_YZX[i][0] = points_XYZ[i][1]; // Y = ancien X (2e �l�ment)
        points_YZX[i][1] = points_XYZ[i][2]; // Z = ancien Y (3e �l�ment)
        points_YZX[i][2] = points_XYZ[i][0]; // X = ancien Z (1er �l�ment)
        points_YZX[i][3] = points_XYZ[i][3]; // H reste inchang�
    }
}

int main()
{

    init_graph(900, 900);
    draw_reper();

    int X = -100 * 2, Y = 50 * 2, Z = 20 * 2;
    int Ix = -50 * 2, Iy = 25 * 2, Iz = 10 * 2;
    int Jx = -100, Jy = 50, Jz = 440;

    double points_YZX[10][4] = {
        {0, 0, 0, 1},    // A 0
        {0, 0, X, 1},    // B 1
        {Y, 0, X, 1},    // C 2
        {Y, 0, 0, 1},    // D 3
        {0, Z, 0, 1},    // E 4
        {0, Z, X, 1},    // F 5
        {Y, Z, X, 1},    // G 6
        {Y, Z, 0, 1},    // H 7
        {Iy, Iz, Ix, 1}, // I 8
        {Jy, Jz, Jx, 1}  // J 9
    };

    Projection_Cavalier(points_YZX, 45, 1.1);
    delay(3000);

    // Tableau pour stocker les points convertis
    double points_XYZ[10][4];

    Convertir_Points_YZX_vers_XYZ(points_YZX, points_XYZ, 10);

    const double speed = 0.001; // Vitesse de rotation (ajuste selon besoin)
    double teta = 0;
    while (teta < 4 * M_PI)
    {

        teta * 180 / M_PI;

        draw_reper();
        T_I_(points_XYZ, -100, 50, 20);
        R_Z(points_XYZ, teta);
        T_I(points_XYZ, -100, 50, 20);

        Convertir_Points_XYZ_vers_YZX(points_XYZ, points_YZX, 10);

        Projection_Cavalier(points_YZX, 45, 1.1);

        delay(100);
        teta += speed;
        cleardevice();
    };

    close_graph();
    return 0;
}
