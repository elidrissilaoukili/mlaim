#include <graphics.h>
#include <stdio.h>
#include <math.h>

// Projection: Convert 3D coordinates to 2D (simple perspective projection)
void projectPoint(float x, float y, float z, int *screenX, int *screenY) {
    float d = 200; // Distance of projection plane
    *screenX = (int)(x + 300); // Shift to center
    *screenY = (int)(300 - y - z / 2); // Simple depth effect
}

// Function to apply transformation matrix
void applyTransformation(float *x, float *y, float *z, float matrix[4][4]) {
    float newX = matrix[0][0] * (*x) + matrix[0][1] * (*y) + matrix[0][2] * (*z) + matrix[0][3];
    float newY = matrix[1][0] * (*x) + matrix[1][1] * (*y) + matrix[1][2] * (*z) + matrix[1][3];
    float newZ = matrix[2][0] * (*x) + matrix[2][1] * (*y) + matrix[2][2] * (*z) + matrix[2][3];

    *x = newX;
    *y = newY;
    *z = newZ;
}

// Function to draw a 3D line projected onto 2D
void drawLine3D(float x1, float y1, float z1, float x2, float y2, float z2) {
    int sx1, sy1, sx2, sy2;
    projectPoint(x1, y1, z1, &sx1, &sy1);
    projectPoint(x2, y2, z2, &sx2, &sy2);
    line(sx1, sy1, sx2, sy2);
}

int main() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, NULL);

    // Define original pyramid points
    float Ax = 100, Ay = 100, Az = 100;
    float Bx = 200, By = 100, Bz = 100;
    float Cx = 150, Cy = 200, Cz = 100;
    float Dx = 150, Dy = 150, Dz = 200;

    // Draw the original pyramid
    setcolor(WHITE);
    drawLine3D(Ax, Ay, Az, Bx, By, Bz);
    drawLine3D(Bx, By, Bz, Cx, Cy, Cz);
    drawLine3D(Cx, Cy, Cz, Ax, Ay, Az);
    drawLine3D(Ax, Ay, Az, Dx, Dy, Dz);
    drawLine3D(Bx, By, Bz, Dx, Dy, Dz);
    drawLine3D(Cx, Cy, Cz, Dx, Dy, Dz);

    // Define transformation matrix (symmetry across plane)
    float T[4][4] = {
        {-1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, -1, 0},
        {0, 0, 0, 1}
    };

    // Apply transformation
    applyTransformation(&Ax, &Ay, &Az, T);
    applyTransformation(&Bx, &By, &Bz, T);
    applyTransformation(&Cx, &Cy, &Cz, T);
    applyTransformation(&Dx, &Dy, &Dz, T);

    // Draw transformed pyramid
    setcolor(RED);
    drawLine3D(Ax, Ay, Az, Bx, By, Bz);
    drawLine3D(Bx, By, Bz, Cx, Cy, Cz);
    drawLine3D(Cx, Cy, Cz, Ax, Ay, Az);
    drawLine3D(Ax, Ay, Az, Dx, Dy, Dz);
    drawLine3D(Bx, By, Bz, Dx, Dy, Dz);
    drawLine3D(Cx, Cy, Cz, Dx, Dy, Dz);

    // Wait for user to close
    getch();
    closegraph();
    return 0;
}
