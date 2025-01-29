#include <graphics.h>
#include <iostream>
#include <cmath>
#include <math.h>

using namespace std;

void init_graph()
{
    int g = DETECT, m;
    initgraph(&g, &m, (char *)"");
    setbkcolor(BLACK);
    cleardevice();
}

void close_graph()
{
    getch();
    closegraph();
}

void drawReperexy()
{
    int centerX = getmaxx() / 2;
    int centerY = getmaxy() / 2;

    // Set color for axes
    setcolor(WHITE);

    // Draw X-axis
    line(0, centerY, getmaxx(), centerY);

    // Draw Y-axis
    line(centerX, 0, centerX, getmaxy());

    // Label the axes
    outtextxy(centerX + 5, 5, (char *)"Y+");
    outtextxy(centerX + 5, getmaxy() - 15, (char *)"Y-");
    outtextxy(getmaxx() - 15, centerY - 15, (char *)"X+");
    outtextxy(5, centerY - 15, (char *)"X-");
}

void drawReperexyz()
{
    int centerX = getmaxx() / 2;
    int centerY = getmaxy() / 2;
    int centerZ = 0;

    // Set color for axes
    setcolor(WHITE);

    // Draw X-axis (horizontal line)
    line(0, centerY, getmaxx(), centerY);

    // Draw Y-axis (vertical line)
    line(centerX, 0, centerX, getmaxy());

    // Draw Z-axis
    line(centerX, centerY, centerX - 200, centerY + 200);

    // Label the axes
    outtextxy(centerX + 5, 5, (char *)"Y+");
    outtextxy(centerX + 5, getmaxy() - 15, (char *)"Y-");
    outtextxy(getmaxx() - 15, centerY - 15, (char *)"X+");
    outtextxy(5, centerY - 15, (char *)"X-");
    outtextxy(centerX - 215, centerY + 215, (char *)"Z+");
}

void putlines_v1(int xp[], int yp[])
{
    int xOffset = getmaxx() / 2;
    int yOffset = getmaxy() / 2;

    line(xp[0] + xOffset, yOffset - yp[0], xp[1] + xOffset, yOffset - yp[1]);
    line(xp[1] + xOffset, yOffset - yp[1], xp[2] + xOffset, yOffset - yp[2]);
    line(xp[2] + xOffset, yOffset - yp[2], xp[3] + xOffset, yOffset - yp[3]);
    line(xp[3] + xOffset, yOffset - yp[3], xp[0] + xOffset, yOffset - yp[0]);

    line(xp[4] + xOffset, yOffset - yp[4], xp[5] + xOffset, yOffset - yp[5]);
    line(xp[5] + xOffset, yOffset - yp[5], xp[6] + xOffset, yOffset - yp[6]);
    line(xp[6] + xOffset, yOffset - yp[6], xp[7] + xOffset, yOffset - yp[7]);
    line(xp[7] + xOffset, yOffset - yp[7], xp[4] + xOffset, yOffset - yp[4]);

    line(xp[0] + xOffset, yOffset - yp[0], xp[4] + xOffset, yOffset - yp[4]);
    line(xp[1] + xOffset, yOffset - yp[1], xp[5] + xOffset, yOffset - yp[5]);
    line(xp[2] + xOffset, yOffset - yp[2], xp[6] + xOffset, yOffset - yp[6]);
    line(xp[3] + xOffset, yOffset - yp[3], xp[7] + xOffset, yOffset - yp[7]);
}

void putlines_v2(int xp[], int yp[])
{
    // Front square
    line(xp[0], yp[0], xp[1], yp[1]);
    line(xp[1], yp[1], xp[2], yp[2]);
    line(xp[2], yp[2], xp[3], yp[3]);
    line(xp[3], yp[3], xp[0], yp[0]);

    // Back square
    line(xp[4], yp[4], xp[5], yp[5]);
    line(xp[5], yp[5], xp[6], yp[6]);
    line(xp[6], yp[6], xp[7], yp[7]);
    line(xp[7], yp[7], xp[4], yp[4]);

    // Connecting front and back
    line(xp[0], yp[0], xp[4], yp[4]);
    line(xp[1], yp[1], xp[5], yp[5]);
    line(xp[2], yp[2], xp[6], yp[6]);
    line(xp[3], yp[3], xp[7], yp[7]);
}